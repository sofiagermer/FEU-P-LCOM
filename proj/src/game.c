#include <lcom/lcf.h>
#include <stdint.h>

#include "game.h"
#include "keyboard.h"
#include "vd_card.h"
#include "kbd_manager.h"
#include "menu.h"

//TIMER
extern unsigned int timer_counter;

//KEYBOARD
extern bool keyboard_done_getting_scancodes;
extern uint8_t scan_code;

WhacAMole *load_game()
{
    WhacAMole *new_game = (WhacAMole *)malloc(sizeof(WhacAMole));

    //load background
    xpm_load(background_xpm, XPM_8_8_8_8, &new_game->background);
    xpm_load(numbers_xpm, XPM_8_8_8_8, &new_game->numbers);
    //load all moles
    for (int i = 0; i < 6; i++)
    {
        timer_counter = 0;
        Mole *new_mole = createMole(i + 1);
        new_game->moles[i] = new_mole;
    }
    //Menu *menu = create_menu();
    new_game->game_state = MAIN_MENU; //ONLY FOR TESTING
    return new_game;
}

int game_main_loop(WhacAMole *new_game)
{
    //WhacAMole* new_game = load_game();
    int ipc_status, r;
    message msg;


    new_game->game_state= SINGLE_PLAYER;



    //draw background
    uint32_t *pixmap = (uint32_t *)new_game->background.bytes;
    vg_draw_xpm(pixmap, new_game->background, 0, 0);
    //draw all moles
    for (int i = 0; i < 6; i++)
    {
        draw_mole(new_game->moles[i]);
    }

    xpm_image_t teste_numbers;
    xpm_load(numbers_xpm, XPM_8_8_8_8, &teste_numbers);
    uint32_t *pixmap_teste = (uint32_t *)teste_numbers.bytes;
    vg_draw_xpm(pixmap_teste, teste_numbers, 100, 20);

    //Subscribing all devices
    if (kbd_subscribe_int(&new_game->keyboard_irq) != OK)
    {
        return 1;
    }
    if (timer_subscribe_int(&new_game->timer_irq) != OK)
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
                    vg_draw_xpm(pixmap, new_game->background, 0, 0);
                    for (int i = 0; i < 6; i++)
                    {
                        draw_mole(new_game->moles[i]);
                    }
                    show_timer(timer_counter, new_game);
                    if (timer_counter % (60 / GAME_FPS) == 0)
                    {
                        GeneralInterrupt(TIMER, new_game);
                    }

                    update_buffer();
                }

                break;
            default:
                break;
            }
        }
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

    return OK;
}

void GeneralInterrupt(device device, WhacAMole *new_game)
{
    switch (new_game->game_state)
    {
    case MAIN_MENU:
        Main_Menu_interrupt_handler(device, new_game);
        break;
    case SINGLE_PLAYER:
        Single_Player_interrupt_handler(device, new_game);
        break;
    case MULTI_PLAYER:
        Multi_Player_interrupt_handler(device, new_game);
        break;
    case EXIT:
        break;
    default:
        break;
    }
    printf("ola")
}

void Main_Menu_interrupt_handler(device device, WhacAMole *game)
{
}
void Single_Player_interrupt_handler(device device, WhacAMole *game)
{
    srand(time(NULL)); // Initialization, should only be called once.

    Position mole_position;

    switch (device)
    {
    case TIMER:
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            mole_position = game->moles[mole_index]->position;
            //if a mole is going up, update its position
            if (mole_position == UP_1 || mole_position == UP_2 || mole_position == UP_3 || mole_position == UP_4)
            {

                if (game->moles[mole_index]->position == UP_4)
                {
                    // Checks if the mole reached to the time limit to be hit
                    //if (game->moles[mole_index]->time_up >= (MOLE_UP_TIME - ((double)(timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * TIME_UP_LIMIT_DECREMENT))
                    if (game->moles[mole_index]->time_up >= MOLE_UP_TIME)
                    {
                        game->moles[mole_index]->time_up = 0;
                        game->moles[mole_index]->position = DOWN_MISSED_4;
                    }
                    else
                        game->moles[mole_index]->time_up++;
                }
                else
                    game->moles[mole_index]->position++;
                continue;
            }

            //if a mole is going down, update its position
            if (mole_position == DOWN_MISSED_1 || mole_position == DOWN_MISSED_2 || mole_position == DOWN_MISSED_3 || mole_position == DOWN_MISSED_4 || mole_position == DOWN_HIT_1 || mole_position == DOWN_HIT_2 || mole_position == DOWN_HIT_3 || mole_position == DOWN_HIT_4)
            {
                if (mole_position == DOWN_MISSED_1 || mole_position == DOWN_MISSED_2 || mole_position == DOWN_MISSED_3 || mole_position == DOWN_MISSED_4 || mole_position == DOWN_HIT_1 || mole_position == DOWN_HIT_2 || mole_position == DOWN_HIT_3 || mole_position == DOWN_HIT_4)
                {
                    if (game->moles[mole_index]->position == DOWN_MISSED_1 || game->moles[mole_index]->position == DOWN_HIT_1)
                    {
                        game->moles[mole_index]->position = HIDED;
                    }
                    else
                        game->moles[mole_index]->position++;
                }
                continue;
            }

            //Variable chance that if the mole is not active to go up
            //if (rand() % 10000 > MOLE_PROBABILITY - (((timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * PROBABILITY_LIMIT_DECREMENT))
            if (rand() % 10000 > MOLE_PROBABILITY)
            {
                game->moles[mole_index]->position = UP_1;
            }
        }

        break;

    case KEYBOARD:
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            mole_position = game->moles[mole_index]->position;
            if (game->moles[mole_index]->kbd_key == kbd_manager(scan_code))
            {
                if (mole_position == UP_4)
                {
                    game->moles[mole_index]->time_up = 0;
                    game->moles[mole_index]->position = DOWN_HIT_4;
                    break;
                }
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

void show_timer(unsigned int timer_counter, WhacAMole *game)
{
    uint8_t seconds = timer_counter / 60; // passing from ticks to seconds
    uint8_t minutes = seconds / 60;
    seconds = seconds % 60;

    uint8_t left_seconds_number, right_seconds_number;
    uint8_t left_minutes_number, right_minutes_number;
    left_seconds_number = seconds / 60;
    right_seconds_number = (seconds % 60) * 10;

    left_minutes_number = minutes / 60;
    right_minutes_number = (minutes % 60) * 10;

    uint32_t *pixmap = (uint32_t *)game->numbers.bytes;
    vg_draw_xpm_new(pixmap, game->numbers, 10, 10);
}
