#pragma once
#include <lcom/lcf.h>

int mouse_subscribe_int(uint8_t *bit_no);

int mouse_unsubscribe_int();

void read_status_register(uint8_t *stat);

int output_empty(uint8_t *st);

int input_empty(uint8_t *st);

int read_out_buffer(uint8_t *info);

void (mouse_ih)(void);

void mouse_parse_packet(uint8_t packet[], struct packet *new_packet);

int issue_command_to_kbc(uint8_t command, uint8_t arguments);

int issue_command_to_mouse(uint8_t command);
