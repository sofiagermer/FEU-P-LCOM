#include <lcom/lcf.h>
#include <stdint.h>

#include "game.h"

//TIMER
extern unsigned int timer_counter;

//KEYBOARD
extern bool keyboard_done_getting_scancodes;
extern uint8_t scan_code;

//MOUSE
extern bool mouse_last_byte_of_packet;
extern uint8_t packet[];

WhacAMole *load_game()
{
    WhacAMole *new_game = (WhacAMole *)malloc(sizeof(WhacAMole));

    load_background();
    xpm_load(numbers_xpm, XPM_8_8_8_8, &new_game->numbers);
    new_game->cursor = load_cursor(cursor_xpm);
    xpm_load(moles_missed_xpm, XPM_8_8_8_8, &new_game->moles_missed);
    xpm_load(moles_hitted_xpm, XPM_8_8_8_8, &new_game->moles_hitted);
    xpm_load(numbers_moles_xpm, XPM_8_8_8_8, &new_game->numbers_mole);
    xpm_load(table_xpm, XPM_8_8_8_8, &new_game->table);
    //load all moles
    for (int i = 0; i < 6; i++)
    {
        timer_counter = 0;
        Mole *new_mole = createMole(i + 1);
        new_game->moles[i] = new_mole;
    }
    new_game->menu = load_menu();
    new_game->player_settings = load_player_settings();
    new_game->cursor = load_cursor(cursor_xpm);
    new_game->game_state = MAIN_MENU; 


    //para o player
    new_game->num_moles_missed = 0;
    new_game->num_moles_hitted = 0;
    return new_game;
}

int game_main_loop(WhacAMole *new_game)
{
    //WhacAMole* new_game = load_game();
    int ipc_status, r;
    message msg;

    //Subscribing all devices
    if (timer_subscribe_int(&new_game->timer_irq) != OK)
    {
        return 1;
    }
    if (mouse_issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE,EN_DATA_REPORT) != OK) {
        return 1;
    }
    if (mouse_subscribe_int(&new_game->mouse_irq) != OK)
    {
        return 1;
    }
    if (kbd_subscribe_int(&new_game->keyboard_irq) != OK)
    {
        return 1;
    }

    while (new_game->game_state != EXIT && timer_counter / (60 / GAME_FPS) <= GAME_DURATION)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status))
        { // received notification
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE: // hardware interrupt notification
                if (msg.m_notify.interrupts & new_game->keyboard_irq)
                {
                    kbc_ih();
                    if (keyboard_done_getting_scancodes)
                    {
                        GeneralInterrupt(KEYBOARD, new_game);
                        if (keyboard_done_getting_scancodes && scan_code == ESC_BREAK)
                        {
                            new_game->game_state = EXIT;
                        }
                    }
                }

                if (msg.m_notify.interrupts & new_game->timer_irq)
                {
                    timer_int_handler();
                    if (timer_counter % (60 / GAME_FPS) == 0)
                    {
                        GeneralInterrupt(TIMER, new_game);
                    }

                    update_buffer();
                }
                if (msg.m_notify.interrupts & new_game->mouse_irq)
                {
                    mouse_ih();
                    if (mouse_last_byte_of_packet)
                    {
                        GeneralInterrupt(MOUSE, new_game);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    if (timer_counter / (60 / GAME_FPS) > GAME_DURATION){
        new_game->game_state = GAME_OVER;
    }


    //Unsubscribing all devices
    if (kbd_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (timer_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (mouse_unsubscribe_int() != OK) {
        return 1;
    }

    if (issue_command_to_mouse(DIS_DATA_REPORT) != OK) {
        return 1;
    }

    return OK;
}

void GeneralInterrupt(device device, WhacAMole *new_game)
{
    switch (new_game->game_state)
    {
    case MAIN_MENU:
        Main_Menu_interrupt_handler(device, new_game);
        break;
    case PLAYER_SETTINGS:
        Player_Settings_interrupt_handler(device, new_game);
        break;
    case SINGLE_PLAYER:
        Single_Player_interrupt_handler(device, new_game);
        break;
    case MULTI_PLAYER:
        Multi_Player_interrupt_handler(device, new_game);
        break;
    case EXIT:
        //TODO
        break;
    default:
        break;
    }
}

void Player_Settings_interrupt_handler(device device, WhacAMole *new_game) {
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch(device){
        case TIMER:
            draw_menu_player_settings(new_game->player_settings);
            break;
        case KEYBOARD:
            break;
        case MOUSE:
            mouse_parse_packet(packet, &new_packet);
            mouse_event = mouse_get_event(&new_packet);
            if (mouse_over(new_game->player_settings->left_arrow, new_game->player_settings->cursor) && mouse_event.type == LB_RELEASED) {
                move_left_avatar(new_game->player_settings);
            }
            else if (mouse_over(new_game->player_settings->right_arrow, new_game->player_settings->cursor) && mouse_event.type == LB_RELEASED) {
                move_right_avatar(new_game->player_settings);
            }
            else if (mouse_over(new_game->player_settings->start, new_game->player_settings->cursor) && mouse_event.type == LB_RELEASED) {
                new_game->cursor->cursor_image = get_hammer(new_game->player_settings);
                new_game->game_state = SINGLE_PLAYER;
            }
            move_cursor__(&new_packet, new_game->player_settings);
            break;
    }
}


void Main_Menu_interrupt_handler(device device, WhacAMole *new_game){
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch(device){
        case TIMER:
            draw_menu(new_game->menu);
            draw_cursor(new_game->menu->cursor);
            //if (new_game->menu->single_player_button->bright_ && mouse_event.type == LB_RELEASED)
            //    new_game->game_state = PLAYER_SETTINGS;
            break;
        case KEYBOARD:
            break;
        case MOUSE:
            mouse_parse_packet(packet, &new_packet);
            mouse_event = mouse_get_event(&new_packet);
            if (new_game->menu->single_player_button->bright_ && mouse_event.type == LB_RELEASED)
                new_game->game_state = PLAYER_SETTINGS;
            move_cursor(&new_packet, new_game->menu->cursor);
            check_cursor_over_button(new_game->menu);
            break;
    }
}

void Single_Player_interrupt_handler(device device, WhacAMole *new_game)
{
    srand(time(NULL)); // Initialization, should only be called once.
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Position mole_position; 

    switch (device)
    {
    case TIMER:
        draw_background();
        draw_all_moles(new_game);
        draw_table(new_game);
        draw_counter_moles(new_game);
        draw_number_moles_missed(new_game);
        draw_number_moles_hitted(new_game);
        draw_cursor(new_game->cursor);
        show_timer(timer_counter, new_game);
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            //Mole *ml = game->moles[mole_index];
            mole_position = new_game->moles[mole_index]->position;
            //if a mole is going up, update its position
            if (mole_position == UP_1 || mole_position == UP_2 || mole_position == UP_3 || mole_position == UP_4)
            {

                if (new_game->moles[mole_index]->position == UP_4)
                {
                    // Checks if the mole reached to the time limit to be hit
                    //if (game->moles[mole_index]->time_up >= (MOLE_UP_TIME - ((double)(timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * TIME_UP_LIMIT_DECREMENT))
                    if (new_game->moles[mole_index]->time_up >= MOLE_UP_TIME)
                    {
                        new_game->moles[mole_index]->time_up = 0;
                        new_game->moles[mole_index]->position = DOWN_MISSED_4;
                    }
                    else
                        new_game->moles[mole_index]->time_up++;
                }
                else
                    new_game->moles[mole_index]->position++;
                continue;
            }

            //if a mole is going down, update its position
            if (mole_position == DOWN_MISSED_1 || mole_position == DOWN_MISSED_2 || mole_position == DOWN_MISSED_3 || mole_position == DOWN_MISSED_4 || mole_position == DOWN_HIT_1 || mole_position == DOWN_HIT_2 || mole_position == DOWN_HIT_3 || mole_position == DOWN_HIT_4)
            {
                if (mole_position == DOWN_MISSED_1 || mole_position == DOWN_MISSED_2 || mole_position == DOWN_MISSED_3 || mole_position == DOWN_MISSED_4 || mole_position == DOWN_HIT_1 || mole_position == DOWN_HIT_2 || mole_position == DOWN_HIT_3 || mole_position == DOWN_HIT_4)
                {
                    if (new_game->moles[mole_index]->position == DOWN_MISSED_1 || new_game->moles[mole_index]->position == DOWN_HIT_1)
                    {
                        new_game->moles[mole_index]->position = HIDED;
                    }
                    else
                        new_game->moles[mole_index]->position++;
                }
                continue;
            }

            //Variable chance that if the mole is not active to go up
            //if (rand() % 10000 > MOLE_PROBABILITY - (((timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * PROBABILITY_LIMIT_DECREMENT))
            if (rand() % 10000 > MOLE_PROBABILITY)
            {
                new_game->moles[mole_index]->position = UP_1;
            }
        }

        break;

    case KEYBOARD:
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            mole_position = new_game->moles[mole_index]->position;
            if (new_game->moles[mole_index]->kbd_key == kbd_manager(scan_code))
            {
                if (mole_position == UP_4)
                {
                    new_game->moles[mole_index]->time_up = 0;
                    new_game->moles[mole_index]->position = DOWN_HIT_4;
                    break;
                }
            }
        }
        break;
    case MOUSE:   
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, new_game->cursor);
        int pos_mole_hitted = check_over_mole(new_game);
        if(pos_mole_hitted != 7){
            if(mouse_event.type == LB_RELEASED){
                hit_mole(new_game, pos_mole_hitted);
                new_game->num_moles_hitted++;
            }
        }
        else if(pos_mole_hitted == 7){
            if(mouse_event.type == LB_RELEASED){
                new_game->num_moles_missed++;
            }
        }

        break; 
    default:
        break;
    }
}

void Multi_Player_interrupt_handler(device device, WhacAMole *game)
{
}
void show_timer(unsigned int timer_counter, WhacAMole *new_game) {
    uint8_t seconds = timer_counter / 60; // passing from ticks to seconds
    uint8_t minutes = seconds / 60;
    seconds = seconds % 60;

    uint8_t left_seconds_number, right_seconds_number;
    uint8_t left_minutes_number, right_minutes_number;

    left_seconds_number = seconds / 10;
    right_seconds_number = (seconds % 10);

    left_minutes_number = minutes / 10;
    right_minutes_number = (minutes % 10);   
    
    if (timer_counter%3600==0) {
        printf("%d\n", minutes);
        printf("%d\n", left_minutes_number);
        printf("%d\n", right_minutes_number);
    }

    uint32_t *pixmap = (uint32_t *)new_game->numbers.bytes;
    
    //seconds
    int x_timer_position = 275;
    int y_timer_position = 15;
    int number_size = new_game->numbers.width/10;
    vg_draw_part_of_xpm(pixmap, new_game->numbers, x_timer_position, y_timer_position, left_minutes_number*50, left_minutes_number*50+50,0,number_size);
    vg_draw_part_of_xpm(pixmap, new_game->numbers, x_timer_position+1*number_size, y_timer_position, right_minutes_number*50, right_minutes_number*50+50,0,number_size);
    
    vg_draw_part_of_xpm(pixmap, new_game->numbers, x_timer_position+2*number_size, y_timer_position, left_seconds_number*50, left_seconds_number*50+50, 0, number_size);
    vg_draw_part_of_xpm(pixmap, new_game->numbers, x_timer_position+3*number_size, y_timer_position, right_seconds_number*50, right_seconds_number*50+50, 0, number_size);
}

void draw_numbers(WhacAMole *new_game){
    uint32_t *pixmap_teste = (uint32_t *) (new_game->numbers).bytes;
    vg_draw_xpm(pixmap_teste, new_game->numbers, 100, 20);
}

void draw_all_moles(WhacAMole * new_game){
    for (int i = 0; i < 6; i++)
    {
        draw_mole(new_game->moles[i]);
    }
}

void draw_table(WhacAMole * new_game){
    uint32_t* table_map = (uint32_t*) new_game->table.bytes;
    vg_draw_xpm(table_map, new_game->table, 0, 470);

}
void draw_counter_moles(WhacAMole * new_game){
    uint32_t* moles_hitted_map = (uint32_t*) new_game->moles_hitted.bytes;
    vg_draw_xpm(moles_hitted_map, new_game->moles_hitted, 25, 530);

    uint32_t* moles_missed_map = (uint32_t*) new_game->moles_missed.bytes;
    vg_draw_xpm(moles_missed_map, new_game->moles_missed, 25, 555);
}

void draw_number_moles_missed(WhacAMole * new_game){
    uint32_t* moles_numbers_map = (uint32_t*) new_game->numbers_mole.bytes;
    int score = new_game->num_moles_missed;
    int second = score%10;
    int first = score/10;
    int x = new_game->numbers_mole.width;
    if(score == 0){
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 555, x, x+17 , 0, 25);
    }
    else if(score < 10){
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 555 , x + score*17, x + score*17 +17 , 0, 25);
    }
    else{
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 555 , x + first*17, x + first*17 +17 , 0, 25);
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x +17 , 555, x + second*17, x + second*17 +17 , 0, 25);
    }
}

void draw_number_moles_hitted(WhacAMole * new_game){
    uint32_t* moles_numbers_map = (uint32_t*) new_game->numbers_mole.bytes;
    int score = new_game->num_moles_hitted;
    int second = score%10;
    int first = score/10;
    int x = new_game->numbers_mole.width;
    
    if(score == 0){
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 530, x, x+17 , 0, 25);
    }
    else if(score < 10){
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 530 , x + score*17, x + score*17 +17 , 0, 25);
    }
    else{
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x , 530 , x + first*17, x + first*17 +17 , 0, 25);
        vg_draw_part_of_xpm(moles_numbers_map, new_game->numbers_mole, x +17 , 530, x + second*17, x + second*17 +17 , 0, 25);
    }
}

int check_over_mole(WhacAMole * new_game){
    Position mole_position; 
    int xf,yf;
    for (int mole_index = 0; mole_index < 6; mole_index++)
        { 
            mole_position = new_game->moles[mole_index]->position;
            xf = new_game->moles[mole_index]->x + 175;
            yf = new_game->moles[mole_index]->y + 151;
            if (new_game->moles[mole_index]->x  <= new_game->cursor->x && new_game->cursor->x <= xf && new_game->moles[mole_index]->y <= new_game->cursor->y && new_game->cursor->y <= yf)
            {
                if (mole_position == UP_4)
                {
                    return mole_index;
                }
            }
        }
    return 7;
}

void hit_mole(WhacAMole * new_game, int mole_index){
    new_game->moles[mole_index]->time_up = 0;
    new_game->moles[mole_index]->position = DOWN_HIT_4;
}
