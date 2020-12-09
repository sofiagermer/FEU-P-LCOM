// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include <vd_card.c>
// Any header files included below this line should have been created by you

#include <background.xpm>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)() {
  /*uint8_t kbd_id;
  int ipc_status, r;
  message msg;
  bool playing = true;*/
  
  if (vggg_init(0x115) == NULL)
		return 1;
  xpm_image_t img;
  uint32_t *pixmap;
  // get the pixmap from the XPM
  pixmap = (uint32_t *) xpm_load(background_xpm, XPM_8_8_8_8, &img);
  // copy it to graphics memory
  vg_draw_xpm (pixmap,  img, 0, 0);

  /*while (playing) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            // hardware interrupt notification
          if (msg.m_notify.interrupts & kbd_id) {
                                     
            
          }
          break;
        default:
          break; // no other notifications expected: do nothing 
      }
    }
    else { // received a standard message, not a notification 
           // no standard messages expected: do nothing 
    }
  }*/

  return 0;
}