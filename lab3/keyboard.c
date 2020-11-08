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


int output_full(uint8_t *st){
    read_status_register(st);
    if(st & KBD_OBF)
      return OK;
    return FAIL
}


int read_out_buffer(uint8_t *info) {
  if (cnt_sys_inb(OUT_BUFF, info) != OK) {
    printf("ERROR::Error reading the out buffer!\n");
    return 1;
  }
  return 0;
}


void(kbc_ih)(void){
  uint8_t status;
  output_full(status);
  uint8_t bytes_read[2];
  bool more_to_read = true;
  int scan_code_size = 0;
  if(status == OK){
    if(status(KBD_PAR_ERROR | KBD_TIME_ERROR | KBD_AUX) == error){
      return 1;
    }
    else{
      if (scan_code == TWO_BYTES_CODE) {
        more_to_read = true;
        bytes_read[0] = scan_code;
      }
      else if (more_to_read) {
        bytes_read[1] = scan_code;
        scan_code_size = 2;
        more_to_read = false;
      }
      else {
        bytes_read[0] = scan_code;
        scan_code_size = 1;
      }
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


