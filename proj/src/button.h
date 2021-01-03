#pragma once
#include <lcom/lcf.h>
#include "cursor.h"
#include "vd_card.h"
#include <Sprites/Buttons_img/normal/main_menu_normal.xpm>
#include <Sprites/Buttons_img/active/main_menu_active.xpm>
#include <Sprites/Buttons_img/normal/score_close_normal.xpm>
#include <Sprites/Buttons_img/active/score_close_active.xpm>



typedef enum {NORMAL=0, ACTIVE} Button_state;

/**
 * @struct Button
 * @var: sprites
 * Button's normal and bright sprites
 * @var:Button_state
 * Button's state: if cursor it's over it :ACTIVE, else :NORMAL
 * @var: xi
 * Button's most left pixel position at x axis
 * @var: xf
 * Button's most right pixel position at x axis
 * @var: yi
 * Button's most lower pixel position at y axis
 * @var: yf
 * Button's most higher pixel position at y axis
 * */
typedef struct {
    xpm_image_t sprites[2];
    Button_state state;
    uint16_t xi, xf, yi, yf;
} Button;

/**
 * @brief loads buttons normal and bright xpm and defines it's coordinates
 * @param xi Button's most left pixel position at x axis
 * @param yi Button's most lower pixel position at y axis
 * @param normal Button's normal xpm
 * @param bright Button's bright xpm
 * @return button
 * */
Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright);

/**
 * @brief Function that updates Button (if cursor it's over it changes button state to ACTIVE)
 * @param cursor cursor being used at the moment 
 * @param buttons buttons of curent game state
 * @param num_buttons number of buttons of current game state
 * */
void update_buttons(Cursor* cursor, Button** buttons, int num_buttons);

/**
 * @brief Draws button
 * @param button Button that we want to draw
 * */
void draw_button(Button *button);

/**
 * @brief draws all buttons wished
 * @param buttons Buttons that we want to draw
 * @param num_buttons Number of buttons that we will draw
 * */

void draw_buttons(Button** buttons, int num_buttons);

/**
 * @brief checks if cursor is over the button
 * @param button Button 
 * @param cursor Cursor
 * */
int mouse_over(Button *button, Cursor *cursor);
