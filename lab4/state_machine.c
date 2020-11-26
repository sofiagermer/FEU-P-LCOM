#include "state_machine.h"

static State state = INITIAL;
uint8_t delta_x, delta_y;

struct mouse_ev mouse_get_event(struct packet *packet) {
    bool left_press = false, right_press = false, mid_press = false;
    struct mouse_ev mouse_event;

    mouse_event.delta_x = packet->delta_x;
    mouse_event.delta_y = packet->delta_y;

    if (left_press && !packet->lb && !right_press && !packet->rb && !mid_press && !packet->mb) {
        left_press = false;
        mouse_event.type = LB_RELEASED; 
    }
    else if (!left_press && packet->lb && !right_press && !packet->rb && !mid_press && !packet->mb) {
        left_press = true;
        mouse_event.type = LB_PRESSED; 
    }

    else if (!left_press && !packet->lb && right_press && !packet->rb && !mid_press && !packet->mb) {
        right_press = false;
        mouse_event.type = RB_RELEASED; 
    }
    else if (!left_press && !packet->lb && !right_press && packet->rb && !mid_press && !packet->mb) {
        right_press = true;
        mouse_event.type = RB_PRESSED; 
    }

    else if (!mid_press && packet->mb) {
        mid_press = true;
        mouse_event.type = BUTTON_EV; 
    }
    else if (mid_press && !packet->mb) {
        mid_press = false;
        mouse_event.type = BUTTON_EV; 
    }

    else
        mouse_event.type = MOUSE_MOV; 
    return mouse_event;
}

bool (drawing_handler)(uint8_t xlen, uint8_t tolerance, struct packet * pack) {
    struct mouse_ev mouse_event = mouse_get_event(pack);
    uint8_t slope = mouse_event.delta_y / mouse_event.delta_x;

    switch(state) {
        case INITIAL:
            delta_x = 0; 
            delta_y = 0;
            if (mouse_event.type == LB_PRESSED)
                state = UP;
            break;
        case UP:
            if (mouse_event.type == LB_RELEASED) {
                if (delta_x >= xlen) {
                    if ((slope > 1 && mouse_event.delta_x != 0) || mouse_event.delta_x == 0) 
                        state = MID;
                }
                else 
                    state = INITIAL;
            }
            else if (mouse_event.type == MOUSE_MOV) {
                if (mouse_event.delta_x < 0 || abs(mouse_event.delta_x) >= tolerance)
                    state = INITIAL;
                else if (mouse_event.delta_y < 0 || abs(mouse_event.delta_y) >= tolerance)
                    state = INITIAL;
                else {
                    delta_x += mouse_event.delta_x;
                    delta_y += mouse_event.delta_y;
                }
            }
            else 
                state = INITIAL;
            break;
        case MID:
            delta_x = 0; 
            delta_y = 0;
            if (mouse_event.type == RB_PRESSED)
                state = DOWN;
            else if (mouse_event.type == MOUSE_MOV && abs(mouse_event.delta_x) <= tolerance && abs(mouse_event.delta_y) <= tolerance) {
                state = MID;
            }
            else if (mouse_event.type == LB_PRESSED)
                state = UP;
            else
                state = INITIAL;
            break;
        case DOWN:
            if (mouse_event.type == RB_RELEASED) {
                if (delta_x >= xlen) {
                    if ((slope > 1 && mouse_event.delta_x != 0) || mouse_event.delta_x == 0)
                        return true;
                }
                else 
                    state = INITIAL;
            }
            else if (mouse_event.type == MOUSE_MOV) {
                if (mouse_event.delta_x < 0 || abs(mouse_event.delta_x) >= tolerance)
                    state = INITIAL;
                else if (mouse_event.delta_y > 0 || abs(mouse_event.delta_y) >= tolerance)
                    state = INITIAL;
                else {
                    delta_x += mouse_event.delta_x;
                    delta_y += mouse_event.delta_y;
                }
            }
            else if (mouse_event.type == LB_PRESSED)
                state = UP;

            else
                state = INITIAL;
            break;
    }
    return false;
}
