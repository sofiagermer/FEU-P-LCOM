

#include "keyboard.h"

// extern unsigned int timer_counter;

// extern bool keyboard_done_getting_scancodes;
// extern uint8_t scan_code;

// int game_main_loop() {
//   int timer_id, kbd_id;
//   bool playing = true;

//   //Subscribing all devices
//   if (kbd_subscribe_int(&kbd_id) != OK) {
//     return FAIL;
//   }
//   if (timer_subscribe_int(&timer_id) != OK) {
//     return FAIL;
//   }

//   while (playing) {
//     if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
//       printf("driver_receive failed with: %d", r);
//       continue;
//     }

//     if (is_ipc_notify(ipc_status)) { // received notification
//       switch (_ENDPOINT_P(msg.m_source)) {
//         case HARDWARE:                            // hardware interrupt notification
//           if (msg.m_notify.interrupts & kbd_id) { // subscribed interrupt BIT MASK
//             kbc_ih();
//           }

//           if (msg.m_notify.interrupts & timer_id) {
//             timer_int_handler();
//           }

//           break;
//         default:
//           break; /* no other notifications expected: do nothing */
//       }
//     }
//     else { /* received a standard message, not a notification */
//            /* no standard messages expected: do nothing */
//     }
//   }

//   //Unsubscribing all devices
//   if (kbd_unsubscribe_int() != OK) {
//     return FAIL;
//   }
//   if (timer_unsubscribe_int() != OK) {
//     return FAIL;
//   }
// }

