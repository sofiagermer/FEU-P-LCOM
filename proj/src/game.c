#include <lcom/lcf.h>
#include <stdint.h>

#include "game.h"
#include "keyboard.h"
#include "vd_card.h"

//TIMER
extern unsigned int timer_counter;

//KEYBOARD
extern bool keyboard_done_getting_scancodes;
extern uint8_t scan_code;

WhacAMole* load_game() {
    WhacAMole* new_game = (WhacAMole *)malloc(sizeof(WhacAMole));
    
    //load background
    xpm_load(background_xpm, XPM_8_8_8_8, &new_game->background);
    //load all moles
    for (int i = 0; i < 6; i++) {
        Mole* new_mole = createMole(i+1);
        new_game->moles[i] = new_mole;
    }
    new_game->game_state = GAME; //ONLY FOR TESTING
    return new_game;
}


int game_main_loop(WhacAMole* new_game) {
    //WhacAMole* new_game = load_game();
    int ipc_status, r;
    message msg;

    //draw background
    uint32_t* pixmap = (uint32_t*) new_game->background.bytes;
    vg_draw_xpm(pixmap, new_game->background, 0, 0);
    //draw all moles
    for (int i = 0; i < 6; i++) {
        draw_mole(new_game->moles[i]);
    }

    //Subscribing all devices
    if (kbd_subscribe_int(&new_game->keyboard_irq) != OK) {return 1;}
    if (timer_subscribe_int(&new_game->timer_irq) != OK)  {return 1;}

    while (new_game->game_state != EXIT || teste < 60) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification
                if (msg.m_notify.interrupts & new_game->keyboard_irq) {
                    //kbc_ih();
                    return 1;
                }

                if (msg.m_notify.interrupts & new_game->timer_irq) {
                    timer_int_handler();
                    interrupt_handler(TIMER, new_game);
                    for (int i = 0; i < 6; i++) {
                        draw_mole(new_game->moles[i]);
                    }
                }

                break;
            default:
                break;
            }
        }
    }

    //Unsubscribing all devices
    if (kbd_unsubscribe_int() != OK) {return 1;}
    if (timer_unsubscribe_int() != OK) {return 1;}

    return OK;
}

void interrupt_handler(device device, WhacAMole* game) {
    srand(time(NULL));   // Initialization, should only be called once.
    
    int mole_up = -1;
    bool up = false;

    int mole_down = -1;
    bool down = false;

    switch(device) {
        case TIMER:
            //Checks if there is any mole going up
            for (int i = 0; i < 6; i++) {
                if (game->moles[i]->position == UP_1 || game->moles[i]->position == UP_2 || game->moles[i]->position == UP_3 || game->moles[i]->position == UP_4) {
                    mole_up = i;
                    up = true;
                    break;
                }
            }
            
            //if a mole is going up, update its position
            if(up) {
                if (game->moles[mole_up]->position == UP_4) {
                    if (game->moles[mole_up]->time_up == MOLE_UP_TIME) {
                        game->moles[mole_up]->time_up = 0;
                        game->moles[mole_up]->position = DOWN_4;
                    }
                    else 
                        game->moles[mole_up]->time_up++;
                }
                else 
                    game->moles[mole_up]->position++;
            }


            //Checks if there is any mole going down
            for (int i = 0; i < 6; i++) {
                if (game->moles[i]->position == DOWN_1 || game->moles[i]->position == DOWN_2 || game->moles[i]->position == DOWN_3 || game->moles[i]->position == DOWN_4) {
                    mole_down = i;
                    down = true;
                    break;
                }
            }

                    new_game->moles[0]->position = UP_4;
            if(down) {
                if (game->moles[mole_up]->position == DOWN_4) {
                    game->moles[mole_up]->position = HIDED;
                }
                else 
                    game->moles[mole_up]->position++;
            }

            //move up a random mole
            if(!down && !up) {
                int r = 1 + (rand() % 7);
                game->moles[r]->position = UP_1;
            }

            break;
        case KEYBOARD:
            break;
        default:
            break;
    }
}
