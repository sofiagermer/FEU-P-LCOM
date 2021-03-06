#pragma once
#include <lcom/lcf.h>
#include "state_machine.h"

int mouse_subscribe_int(uint16_t *bit_no);

int mouse_unsubscribe_int();

void read_status_register(uint8_t *stat);

int mouse_check_status_register();

int output_full();

int input_empty();

int read_out_buffer(uint8_t *info);

void (mouse_ih)(void);

void mouse_parse_packet(uint8_t packet[], struct packet *new_packet);

int issue_command_to_kbc(uint8_t command, uint8_t arguments);

int issue_command_to_mouse(uint8_t command);

uint8_t mouse_read_response();



int issue_cmd_to_kbc(uint8_t command, uint8_t argument);
int kbc_write_cmd(uint8_t command);
int kbc_write_argument(uint8_t argument);
int mouse_write_command(uint8_t command, uint8_t*response);
