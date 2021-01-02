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

//RTC
extern Time rtc_time;
extern Date rtc_date;

static unsigned int exit_time;

WhacAMole *load_game()
{
    srand(time(NULL)); // Initialization, should only be called once.
    WhacAMole *new_game = (WhacAMole *)malloc(sizeof(WhacAMole));
    new_game->game_time = 30;
    xpm_load(numbers_xpm, XPM_8_8_8_8, &new_game->game_time_numbers_font);
    new_game->cursor = load_cursor(cursor_xpm);
    xpm_load(moles_missed_xpm, XPM_8_8_8_8, &new_game->moles_missed);
    xpm_load(moles_hitted_xpm, XPM_8_8_8_8, &new_game->moles_hitted);
    xpm_load(score_numbers_xpm, XPM_8_8_8_8, &new_game->numbers_font);
    xpm_load(table_xpm, XPM_8_8_8_8, &new_game->table);
    xpm_load(clock_icon_xpm, XPM_8_8_8_8, &new_game->clock_icon);
    xpm_load(font_small_xpm, XPM_8_8_8_8, &(new_game->letters_small_font));
    //load all moles
    for (int i = 0; i < 6; i++)
    {
        Mole *new_mole = createMole(i);
        new_game->moles[i] = new_mole;
    }
    new_game->menu = load_menu();
    new_game->player_settings = load_player_settings();
    new_game->player = load_player(new_game->player_settings->avatars[0]);
    new_game->leaderboard = load_leaderboard();
    new_game->cursor = load_cursor(cursor_xpm);
    new_game->game_over = load_game_over();
    new_game->good_bye = load_good_bye();
    new_game->instructions = load_instructions();
    new_game->game_state = MAIN_MENU;
    new_game->running = true;

    new_game->good_bye->x_mole = 0;
    return new_game;
}

GameOver *load_game_over()
{
    GameOver *game_over = (GameOver *)malloc(sizeof(GameOver));

    xpm_load(game_over_xpm, XPM_8_8_8_8, &(game_over->logo_game_over));
    xpm_load(game_over_missed_moles_xpm, XPM_8_8_8_8, &(game_over->missed_moles));
    xpm_load(game_over_hitted_moles_xpm, XPM_8_8_8_8, &(game_over->hitted_moles));
    xpm_load(game_over_numbers_xpm, XPM_8_8_8_8, &(game_over->numbers));

    game_over->cursor = load_cursor(cursor_xpm);
    game_over->num_buttons = 3;
    game_over->buttons = (Button **)malloc(sizeof(Button *) * game_over->num_buttons);
    game_over->buttons[0] = load_button(MAIN_MENU_X, MAIN_MENU_Y, main_menu_normal_xpm, main_menu_active_xpm);
    game_over->buttons[1] = load_button(LEADERB_GM_OV_X, LEADERB_GM_OV_Y, leaderboard_normal_xpm, leaderboard_active_xpm);
    game_over->buttons[2] = load_button(EXIT_GM_OV_X, EXIT_GM_OV_Y, exit_normal_xpm, exit_active_xpm);
    return game_over;
}

Instructions *load_instructions()
{
    Instructions *instructions = (Instructions *)malloc(sizeof(Instructions));

    //xpm_load(instructions_xpm, XPM_8_8_8_8, &(instructions->instructions));
    
    xpm_load(instructions_xpm, XPM_8_8_8_8, &(instructions->instructions));
    return instructions;
}

GoodBye *load_good_bye()
{
    GoodBye *good_bye = (GoodBye *)malloc(sizeof(GoodBye));
    xpm_load(good_bye_message_xpm, XPM_8_8_8_8, &(good_bye->logo));
    xpm_load(good_bye_mole_xpm, XPM_8_8_8_8, &(good_bye->mole));
    xpm_load(credits_xpm, XPM_8_8_8_8, &(good_bye->credits));
    good_bye->x_mole = 0;
    return good_bye;
}

int game_main_loop(WhacAMole *new_game)
{
    int ipc_status, r;
    message msg;
    uint8_t irq_rtc_auxiliar = 0;
    rtc_start();

    //Subscribing all devices
    if (timer_subscribe_int(&new_game->timer_irq) != OK)
    {
        return 1;
    }
    if (mouse_issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE, EN_DATA_REPORT) != OK)
    {
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
    if (rtc_subscribe_int(&irq_rtc_auxiliar) != OK)
    {
       return 1;
    }
    new_game->irq_rtc = BIT(irq_rtc_auxiliar);

    while (new_game->running)
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
                    }
                }

                if (msg.m_notify.interrupts & new_game->timer_irq)
                {
                    timer_int_handler();
                    /*if (timer_counter / (60 / GAME_FPS) == GAME_DURATION)
                    {
                        new_game->game_state = GAME_OVER;
                    }*/
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
                if (msg.m_notify.interrupts & new_game->irq_rtc) {
                    rtc_int_handler();
                    GeneralInterrupt(RTC, new_game);
                }
                break;
            default:
                break;
            }
        }
    }
    free(new_game);
    //Unsubscribing all devices
    if (kbd_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (timer_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (mouse_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (issue_command_to_mouse(DIS_DATA_REPORT) != OK)
    {
        return 1;
    }
    if (rtc_unsubscribe_int() != 0) 
    {
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
    case INSTRUCTIONS:
        Instructions_interrupt_handler(device, new_game);
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
    case GAME_OVER:
        Game_Over_interrupt_handler(device, new_game);
        break;
    case LEADERBOARD:
        Leaderboard_interrupt_handler(device, new_game);
        break;
    case EXIT:
        Exit_interrupt_handler(device, new_game);
        break;
    default:
        break;
    }
}

void Main_Menu_interrupt_handler(device device, WhacAMole *new_game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch (device)
    {
    case TIMER:
        draw_background(new_game->menu);
        draw_logo(new_game->menu);
        draw_buttons(new_game->menu->buttons, new_game->menu->num_buttons);
        if (new_game->menu->buttons[5]->state == ACTIVE)
            draw_date(new_game->numbers_font, CALLENDAR_NUM_X, CALLENDAR_NUM_Y, rtc_date);
        if (new_game->menu->buttons[6]->state == ACTIVE) {
            draw_time(new_game->numbers_font, CLOCK_NUM_X, CLOCK_NUM_Y, rtc_time);
        }
        draw_cursor(new_game->cursor);
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, new_game->cursor);
        update_buttons(new_game->cursor, new_game->menu->buttons, new_game->menu->num_buttons);
        if (new_game->menu->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            new_game->game_state = PLAYER_SETTINGS;
        /*else if (new_game->menu->buttons[1]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            new_game->game_state = multiplayer;*/
        else if (new_game->menu->buttons[2]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            new_game->game_state = LEADERBOARD;

        else if (new_game->menu->buttons[3]->state == ACTIVE && mouse_event.type == LB_RELEASED){
            new_game->game_state = INSTRUCTIONS;
        }
        else if (new_game->menu->buttons[4]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            exit_time = timer_counter + 180;
            new_game->game_state = EXIT;
        }
        break;
    case RTC:
        break;
    }
}
void Instructions_interrupt_handler(device device, WhacAMole *new_game)
{
    switch (device)
    {
    case TIMER:
        draw_instructions(new_game->instructions);
        break;
    case KEYBOARD:
        if (keyboard_done_getting_scancodes && scan_code == ESC_BREAK)
            {
                new_game->game_state = MAIN_MENU;
            }
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    }
}
void Player_Settings_interrupt_handler(device device, WhacAMole *new_game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch (device)
    {
    case TIMER:
        draw_background__(new_game->player_settings);
        draw_avatars(new_game->player_settings->avatars);
        draw_buttons(new_game->player_settings->buttons, new_game->player_settings->num_buttons);
        draw_player_name(new_game->player_settings->font, NM_PLACE_X, NM_PLACE_Y, new_game->player->name, new_game->player->max_name_length);
        if (new_game->player_settings->name_maximum_length)
            draw_name_lenght_warning(new_game->player_settings);
        draw_cursor(new_game->cursor);
        break;
    case KEYBOARD:
        if (new_game->player_settings->buttons[2]->state == ACTIVE)
        {
            if (scan_code == BACK_SPACE_BREAK)
            {
                update_player_name(new_game->player_settings, new_game->player, true, ' ');
            }
            else
            {
                char new_letter = kbd_manager(scan_code);
                if (new_letter != '.')
                    update_player_name(new_game->player_settings, new_game->player, false, kbd_manager(scan_code));
            }
        }
        if (scan_code == ENTER_BREAK)
        {
            new_game->player_settings->buttons[2]->state = NORMAL;
            new_game->player_settings->name_maximum_length = false;
        }
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, new_game->cursor);
        update_buttons(new_game->cursor, new_game->player_settings->buttons, new_game->player_settings->num_buttons);
        
        if (mouse_over(new_game->player_settings->buttons[0], new_game->cursor) && mouse_event.type == LB_RELEASED)
            move_left_avatar(new_game->player_settings);
        else if (mouse_over(new_game->player_settings->buttons[1], new_game->cursor) && mouse_event.type == LB_RELEASED)
            move_right_avatar(new_game->player_settings);
        else if (mouse_over(new_game->player_settings->buttons[2], new_game->cursor) && mouse_event.type == LB_RELEASED)
        {
            if (new_game->player->name[new_game->player->max_name_length - 1] != ' ')
                new_game->player_settings->name_maximum_length = true;
        }
        else if (mouse_over(new_game->player_settings->buttons[3], new_game->cursor) && mouse_event.type == LB_RELEASED)
        {
            new_game->cursor->cursor_image = get_hammer(new_game->player_settings);
            new_game->game_state = SINGLE_PLAYER;
        }
        break;
    case RTC:
        break;
    }
}


void Single_Player_interrupt_handler(device device, WhacAMole *new_game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Position mole_position;
    static int time_duration = 0;
    switch (device)
    {
    case TIMER:
        draw_background(new_game->menu);
        draw_all_moles(new_game);
        draw_table(new_game);
        draw_counter_moles(new_game);
        draw_number(new_game->numbers_font, 13, MOLES_MISS_NUM_X, MOLES_MISS_NUM_Y, new_game->player->missed_moles);
        draw_number(new_game->numbers_font, 13, MOLES_HIT_NUM_X, MOLES_HIT_NUM_Y, new_game->player->hitted_moles);
        vg_draw_xpm((uint32_t *)new_game->clock_icon.bytes, new_game->clock_icon, CLOCK_ICON_X, CLOCK_ICON_Y);
        draw_number(new_game->game_time_numbers_font, 10,  GAME_TIMER_X, GAME_TIMER_Y, time_duration/GAME_FPS);
        //draw_game_timer(time_duration/GAME_FPS, new_game);
        draw_cursor(new_game->cursor);
        time_duration++;
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            Mole *ml = new_game->moles[mole_index];
            //if (rand() % 10000 > MOLE_PROBABILITY - (((timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * PROBABILITY_LIMIT_DECREMENT))
            if (ml->position == HIDED && (rand() % 10000 > MOLE_PROBABILITY))
            {
                ml->position = UP_1;
                ml->time_up = 0;
            }
            else if (ml->position == UP_4)
            {
                ml->time_up++;
                //if (ml->time_up >= (MOLE_UP_TIME - ((double)(timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * TIME_UP_LIMIT_DECREMENT))
                if (ml->time_up >= MOLE_UP_TIME)
                {
                    ml->position = DOWN_MISSED_4;
                    ml->time_up = 0;
                }
            }
            else if (ml->position != HIDED)
            {
                ml->position++;
                if (ml->position == DOWN_MISSED_1 || ml->position == DOWN_HIT_1)
                    ml->position = HIDED;
            }
        }
        if (time_duration/GAME_FPS >= new_game->game_time) {
            new_game->game_state = GAME_OVER;
            new_game->cursor->cursor_image = new_game->cursor->cursor_image_default;
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

                    new_game->player->hitted_moles++;
                    break;
                }
                else
                {
                    new_game->player->missed_moles++;
                }
            }
        }
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, new_game->cursor);
        int pos_mole_hitted = check_over_mole(new_game);
        if (pos_mole_hitted != 7)
        {
            if (mouse_event.type == LB_RELEASED)
            {
                new_game->moles[pos_mole_hitted]->time_up = 0;
                new_game->moles[pos_mole_hitted]->position = DOWN_HIT_4;
                //hit_mole(new_game, pos_mole_hitted);
                new_game->player->hitted_moles++;
            }
        }
        else if (pos_mole_hitted == 7)
        {
            if (mouse_event.type == LB_RELEASED)
            {
                new_game->player->missed_moles++;
            }
        }
        break;
    case RTC:
        break;
    default:
        break;
    }
}

void Multi_Player_interrupt_handler(device device, WhacAMole *game)
{
}

void Game_Over_interrupt_handler(device device, WhacAMole *new_game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch (device)
    {
    case TIMER:
        draw_background(new_game->menu);
        draw_game_over(new_game->game_over, new_game->player);
        draw_cursor(new_game->cursor);
        break;
    case KEYBOARD:
        if (scan_code == ENTER_BREAK)
            add_new_score(new_game->leaderboard, new_game->player);
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, new_game->cursor);
        update_buttons(new_game->cursor, new_game->game_over->buttons, new_game->game_over->num_buttons);
        if (new_game->game_over->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            new_game->player_settings = load_player_settings();
            new_game->game_state = MAIN_MENU;
        }
        if (new_game->game_over->buttons[1]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            new_game->game_state = LEADERBOARD;
        if (new_game->game_over->buttons[2]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            exit_time = timer_counter + 180;
            new_game->game_state = EXIT;
        }
        break;
    case RTC:
        break;
    }
}

void Leaderboard_interrupt_handler(device device, WhacAMole *new_game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch (device)
    {
    case TIMER:
        draw_leaderboard_backgound(new_game->leaderboard);
        draw_table__(new_game->leaderboard);
        draw_buttons(new_game->leaderboard->buttons, new_game->leaderboard->num_buttons);
        draw_player_names(new_game->letters_small_font, new_game->leaderboard->score_records, new_game->leaderboard->num_score_records);
        draw_player_scores(new_game->leaderboard);
        draw_player_dates(new_game->leaderboard);
        draw_cursor(new_game->cursor);
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        if (new_game->leaderboard->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            new_game->game_state = MAIN_MENU;
        }
        move_cursor(&new_packet, new_game->cursor);
        update_buttons(new_game->cursor, new_game->leaderboard->buttons, new_game->leaderboard->num_buttons);
        break;
    case RTC:
        break;
    }
}

void Exit_interrupt_handler(device device, WhacAMole *new_game)
{
    switch (device)
    {
    case TIMER:
        if (exit_time > timer_counter)
        {
            draw_background(new_game->menu);
            draw_good_bye_message(new_game->good_bye);
            if (timer_counter % 5 == 0)
            {
                new_game->good_bye->x_mole += 18;
            }
            draw_good_bye_mole(new_game->good_bye);
        }
        else
        {
            save_scores(new_game->leaderboard);
            new_game->running = false;
        }
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    }
}

void draw_all_moles(WhacAMole *new_game)
{
    for (int i = 0; i < 6; i++)
    {
        draw_mole(new_game->moles[i]);
    }
}

void draw_table(WhacAMole *new_game)
{
    uint32_t *table_map = (uint32_t *)new_game->table.bytes;
    vg_draw_xpm(table_map, new_game->table, SCORE_TABLE_X, SCORE_TABLE_Y);
}

void draw_counter_moles(WhacAMole *new_game)
{
    uint32_t *moles_hitted_map = (uint32_t *)new_game->moles_hitted.bytes;
    vg_draw_xpm(moles_hitted_map, new_game->moles_hitted, MOLES_MISS_FRAME_X, MOLES_MISS_FRAME_Y);

    uint32_t *moles_missed_map = (uint32_t *)new_game->moles_missed.bytes;
    vg_draw_xpm(moles_missed_map, new_game->moles_missed, MOLES_HIT_FRAME_X, MOLES_HIT_FRAME_Y);
}

void draw_number(xpm_image_t font, int font_info, int xi, int yi, int number)
{
    int left_number = number / 10;
    int right_number = number % 10;
    int number_width = font.width / font_info;
    int number_height = font.height;
    if (left_number != 0)
        vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi, yi, left_number * number_width, (left_number+1)*number_width , 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi + number_width, yi, right_number * number_width, (right_number+1)*number_width , 0, number_height);
}

int check_over_mole(WhacAMole *new_game)
{
    Position mole_position;
    int xf, yf;
    for (int mole_index = 0; mole_index < 6; mole_index++)
    {
        mole_position = new_game->moles[mole_index]->position;
        xf = new_game->moles[mole_index]->x + 175;
        yf = new_game->moles[mole_index]->y + 151;
        if (new_game->moles[mole_index]->x <= new_game->cursor->x && new_game->cursor->x <= xf && new_game->moles[mole_index]->y <= new_game->cursor->y && new_game->cursor->y <= yf)
        {
            if (mole_position == UP_4)
            {
                return mole_index;
            }
        }
    }
    return 7;
}

/*void hit_mole(WhacAMole *new_game, int mole_index)
{
    new_game->moles[mole_index]->time_up = 0;
    new_game->moles[mole_index]->position = DOWN_HIT_4;
}*/

void draw_game_over_hitted_moles(GameOver *game_over, Player *player)
{
    uint32_t *moles_numbers_map = (uint32_t *)game_over->numbers.bytes;
    int score = player->hitted_moles;
    int second = score % 10;
    int first = score / 10;
    int x = 500;

    if (score == 0)
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 311, x, x + 31, 0, 47);
    }
    else if (score < 10)
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 311, x + score * 31, x + score * 31 + 31, 0, 47);
    }
    else
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 311, x + first * 31, x + first * 31 + 31, 0, 47);
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x + 31, 311, x + second * 17, x + second * 31 + 31, 0, 47);
    }
}

void draw_game_over_missed_moles(GameOver *game_over, Player *player)
{
    uint32_t *moles_numbers_map = (uint32_t *)game_over->numbers.bytes;
    int score = player->missed_moles;
    int second = score % 10;
    int first = score / 10;
    int x = 500;
    if (score == 0)
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 370, x, x + 31, 0, 47);
    }
    else if (score < 10)
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 370, x + score * 31, x + score * 31 + 31, 0, 47);
    }
    else
    {
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x, 370, x + first * 31, x + first * 31 + 31, 0, 47);
        vg_draw_part_of_xpm(moles_numbers_map, game_over->numbers, x + 31, 370, x + second * 31, x + second * 31 + 31, 0, 47);
    }
}

void draw_game_over_buttons(GameOver *game_over)
{
    for (int i = 0; i < game_over->num_buttons; i++)
    {
        draw_button(game_over->buttons[i]);
    }
}
void draw_game_over(GameOver *game_over, Player *player)
{
    uint32_t *game_over_map = (uint32_t *)game_over->logo_game_over.bytes;
    vg_draw_xpm(game_over_map, game_over->logo_game_over, 110, 230);

    uint32_t *hitted_moles_map = (uint32_t *)game_over->hitted_moles.bytes;
    vg_draw_xpm(hitted_moles_map, game_over->hitted_moles, 110, 310);

    uint32_t *missed_moles_map = (uint32_t *)game_over->missed_moles.bytes;
    vg_draw_xpm(missed_moles_map, game_over->missed_moles, 110, 370);

    draw_game_over_missed_moles(game_over, player);
    draw_game_over_hitted_moles(game_over, player);

    draw_game_over_buttons(game_over);
}

void draw_good_bye_message(GoodBye *good_bye)
{
    uint32_t *good_bye_map = (uint32_t *)good_bye->logo.bytes;
    vg_draw_xpm(good_bye_map, good_bye->logo, 0, 230);

    uint32_t *credits_map = (uint32_t *)good_bye->credits.bytes;
    vg_draw_xpm(credits_map, good_bye->credits, 550, 80);
}

void draw_good_bye_mole(GoodBye *good_bye)
{
    uint32_t *good_bye_mole = (uint32_t *)good_bye->mole.bytes;
    vg_draw_xpm(good_bye_mole, good_bye->mole, good_bye->x_mole, 450);
}

void draw_instructions(Instructions *instructions)
{
    uint32_t *instructions_map = (uint32_t *)instructions->instructions.bytes;
    vg_draw_xpm(instructions_map, instructions->instructions, 0,0);
}
