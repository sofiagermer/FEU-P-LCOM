#include <lcom/lcf.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"


static int kbd_hook_id = 1; //hook id used for the keyboard
uint8_t scan_code = 0;
uint32_t sys_inb_counter = 0;

int cnt_sys_inb(port_t port, uint8_t *byte) {
  if (util_sys_inb(port, byte))
    return 1;
  sys_inb_counter++; //global variable is icremented by one every time function is called
  return 0;
}


int kbd_subscribe_int(uint8_t *bit_no) {
  *bit_no = BIT(kbd_hook_id);
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK) {
    printf("kbd_subscribe_int::ERROR in setting policy!\n");
    return 1;
  }
  return 0;
}


int kbd_unsubscribe_int() {
  if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
    printf("kbd_unsubscribe_int::ERROR removing policy!\n");
    return 1;
  };
  return 0;
}


void read_status_register(uint8_t *stat) {
  if (cnt_sys_inb(STAT_REG, stat) != OK) {
    printf("ERROR::Unable to read keyboard status register!\n");
  }
}


int check_status_register() {
  uint8_t temp; //hold the status
  read_status_register(&temp);
  if ((temp & (KBD_PAR_ERROR | KBD_TIME_ERROR | KBD_AUX)) != 0) {
      return 1;
    }
  return 0;
}


int read_out_buffer(uint8_t *info) {
  if (cnt_sys_inb(OUT_BUFF, info) != OK) {
    printf("ERROR::Error reading the out buffer!\n");
    return 1;
  }
  return 0;
}


void (kbc_ih)(void) {
  uint8_t status, scan_code;
  read_status_register(&status);

  // checks if the output buffer is full  
  if (status & KBD_OBF) {

    //reads scan code from output buffer               
    if (read_out_buffer(&scan_code) != OK) { 
      scan_code = 0;
      return;                                
    }

    //checks if there is signal of an error
    if (check_status_register() != OK) { 
      // discards the scan code
      scan_code = 0;                                      
      return;                                           
    }
  }
}


int issue_command(uint8_t command, uint8_t arguments) {
  uint8_t status;
  uint8_t attemps = 0; //number of attemps the function will try to issue the command

  //stops after 4 attemps
  while (attemps < 4) {
    attemps++;
    read_status_register(&status);

    // input buffer should not be full
    if ((status & KBD_IBF) == 0) {
      //writtes command
      if (sys_outb(CMD_REG, command) != OK) {
        printf("ERROR::Unable to write command to register!\n");
        return 1;
      }
      //in case the command is WriteCommandByte it needs the new command (arguments)
      if (command == WRITE_CMD_BYTE) {
        if (sys_outb(CMD_ARG_REG, arguments) != OK) {
          printf("ERROR::Unable to writes arguments to regiter!\n");
          return 1;
        }
      }
    }

    //wait 20ms before trying again
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("ERROR::After 4 attemps KBC was not ready to receive command!\n");
  return 1;
}


