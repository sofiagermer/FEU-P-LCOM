#include <lcom/lcf.h>
#include <stdint.h>

#include "mouse.h"
#include "i8042.h"


static int mouse_hook_id = 1; //hook id used for the mouse
uint8_t packet[3]; //array of bytes, packet read from the mouse
bool mouse_last_byte_of_packet; //signals that the last byte of a packet was read

int mouse_subscribe_int(uint8_t *bit_no) {
  *bit_no = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
    printf("mouse_subscribe_int::ERROR in setting policy!\n");
    return FAIL;
  }
  return OK;
}


int mouse_unsubscribe_int() {
  if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
    printf("kbd_unsubscribe_int::ERROR removing policy!\n");
    return FAIL;
  };
  return OK;
}


void read_status_register(uint8_t *stat) {
  if (util_sys_inb(STAT_REG, stat) != OK) {
    printf("ERROR::Unable to read keyboard status register!\n");
  }
}


int output_full() {
    uint8_t st;
    read_status_register(st);
    if(st & KBD_OBF)
      return OK;
    return FAIL;
}


int input_empty() {
    uint8_t st;
    read_status_register(st);
    if(st & KBD_IBF)
      return FAIL;
    return OK;
}


int read_out_buffer(uint8_t *info) {
  if (util_sys_inb(OUT_BUFF, info) != OK) {
    printf("ERROR::Error reading the out buffer!\n");
    return FAIL;
  }
  return OK;
}


void (mouse_ih)(void) {
  uint8_t aux, st;
  static uint8_t index = 1;
  if (mouse_last_byte_of_packet) {
      index = 0;
      mouse_last_byte_of_packet = false;
  }
  if (output_full() == OK) {
    if (read_out_buffer(&aux) != OK) {
      printf("ERROR::Error reading the out buffer!\n");
      return;
    }
    packet[index] = aux;
    index++;
    if (index == 3) {
      mouse_last_byte_of_packet = true;
    }
  }
}


void mouse_parse_packet(uint8_t packet[], struct packet *new_packet) {
  uint8_t first_byte = packet[0];
  new_packet->bytes[0] = packet[0];
  new_packet->bytes[1] = packet[1];
  new_packet->bytes[2] = packet[2];
  new_packet->rb = (first_byte & RB);
  new_packet->lb = (first_byte & LB);
  new_packet->mb = (first_byte & MB);
  new_packet->x_ov = (first_byte & X_OVFL);
  new_packet->y_ov = (first_byte & Y_OVFL);
  new_packet->delta_x = packet[1];
  new_packet->delta_y = packet[2];

  if (new_packet->x_ov && (first_byte & MSB_X_DELTA))
    new_packet->delta_x = -256;
  else if (new_packet->x_ov && !(first_byte & MSB_X_DELTA))
    new_packet->delta_x = 255;
  else 
    new_packet->delta_x *= -1;

  if (new_packet->y_ov && (first_byte & MSB_Y_DELTA))
    new_packet->delta_y = -256;
  else if (new_packet->y_ov && !(first_byte & MSB_Y_DELTA))
    new_packet->delta_y = 255;
  else 
    new_packet->delta_y *= -1;
}


int issue_command_to_kbc(uint8_t command, uint8_t arguments) {
  uint8_t status;
  uint8_t attemps = 0; //number of attemps the function will try to issue the command

  //stops after 4 attemps
  while (attemps < 4) {
    attemps++;

    // input buffer should not be full
    if (input_empty() == OK) {
      //writtes command
      if (sys_outb(CMD_REG, command) != OK) {
        printf("ERROR::Unable to write command to register!\n");
        return 1;
      }
      //in case the command is WriteCommandByte it needs the new command (arguments)
      if (command == WRITE_CMD_BYTE) {
        if (sys_outb(CMD_ARG_REG, arguments) != OK) {
          printf("ERROR::Unable to writes arguments to register!\n");
          return 1;
        }
      }
      if (command == WRITE_BYTE_TO_MOUSE) {
        if (sys_outb(CMD_ARG_REG, arguments) != OK) {
          printf("ERROR::Unable to writes arguments to register!\n");
          return 1;
        }
        issue_command_to_mouse(arguments);
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
    //wait 20ms before trying againFAILwas not ready to receive command!\n");
  }
  return FAIL;
}


int issue_command_to_mouse(uint8_t command) {
  uint8_t response;
  uint8_t attemps = 0; //number of attemps the function will try to issue the command
  bool write = false;
  uint8_t st;
  while (attemps < 4) {
    if (input_empty() == OK) {
      if (sys_outb(IN_BUFF, command) != OK) {
        printf("ERROR::Unable to write command to mouse!\n");
        return FAIL;
      }
      else {
        write = true;
        attemps = 0;
      }
    }
    attemps++;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  if (!write) {
    printf("ERROR::After 4 attemps mouse was not ready to receive command!\n");
    return FAIL;
  }

  //stops after 4 attemps
  while (attemps < 4) {
    attemps++;
    if (input_empty() != OK) {
      continue;
    }
    if (util_sys_inb(OUT_BUFF, &response) == OK) {
      if (response == ACK) {
        return OK;
      }
      else if (response == NACK) {
        continue;
      }
      else if (response == ERROR) {
        return FAIL;
      }
    }
    else {
      printf("ERROR::Unable to read mouse response!\n");
      return FAIL;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return OK;
}
