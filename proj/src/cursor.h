#pragma once
#include <lcom/lcf.h>
#include <Sprites/cursor.xpm>
#include "vd_card.h"

/**
 * @struct Cursor
 * @var: cursor_image 
 * Cursor's xpm
 * @var: x
 * Cursor's position in x axis
 * @var: y
 * Cursor's  position in y axis
 * */

typedef struct{
    xpm_image_t cursor_image;
    uint16_t x;
    uint16_t y;
} Cursor;

/**
 * @brief Function that creates a cursor with a given xpm that its initially set at the position (400,300)
 * @param xmp_row_t *img_cursor
 * @return Cursor *cursor
 * */
Cursor *load_cursor(xpm_row_t *img_cursor);

/**
 * @brief Function that draws the cursor at it's current position
 * @param Cursor *cursor 
 *  */
void draw_cursor(Cursor *cursor);

/**
 * @brief Function that moves cursor based on the Mouse Packet Received
 * @param struct packet *packet, Mouse Packet with mouse informations
 * @param Cursor *cursor
 * */

void move_cursor(struct packet *packet, Cursor *cursor);
