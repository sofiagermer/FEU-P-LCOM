#pragma once
#include <lcom/lcf.h>
#include <Sprites/background.xpm>
#include <Sprites/logo.xpm>
#include <Sprites/Buttons_img/normal/singleplayer_normal.xpm>
#include <Sprites/Buttons_img/active/singleplayer_active.xpm>
#include <Sprites/Buttons_img/normal/multiplayer_normal.xpm>
#include <Sprites/Buttons_img/active/multiplayer_active.xpm>
#include <Sprites/Buttons_img/normal/leaderboard_normal.xpm>
#include <Sprites/Buttons_img/active/leaderboard_active.xpm>
#include <Sprites/Buttons_img/normal/instructions_normal.xpm>
#include <Sprites/Buttons_img/active/instructions_active.xpm>
#include <Sprites/Buttons_img/normal/exit_normal.xpm>
#include <Sprites/Buttons_img/active/exit_active.xpm>
#include <Sprites/Buttons_img/normal/calendar_normal.xpm>
#include <Sprites/Buttons_img/active/calendar_active.xpm>
#include <Sprites/Buttons_img/normal/clock_normal.xpm>
#include <Sprites/Buttons_img/active/clock_active.xpm>

#include "button.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
 * @struct Menu
 * @var: sprites[2]
 * Menu's sprites xpm
 * @var: buttons
 * Menu's buttons
 * @var:num_buttons
 * Menu's number of buttons
 * */
typedef struct {
    xpm_image_t sprites[2];
    Button** buttons;
    int num_buttons;     
} Menu;

void load_background();

/**
 * @brief Function that creates a menu: loads the background xpm, the WhackaMole's logo and all menu's buttons
 * @param xmp_row_t *img_cursor
 * @return Cursor *cursor
 * */
Menu *load_menu();

/**
 * @brief Function that draws the game's background
 * @param Menu * menu
 * */

void draw_background(Menu * menu);

/**
 * @brief Function that draws the game's logo
 * @param Menu * menu
 * */

void draw_logo(Menu *menu);

/**
 * @brief Function that draws the game's date
 * @param xpm_image_t font
 * @param int xi
 * @param int yi
 * @param Date date 
 * */

void draw_date(xpm_image_t font, int xi, int yi, Date date);

/**
 * @brief Function that draws the current's time
 * @param xpm_image_t font
 * @param int xi
 * @param int yi
 * @param Time time 
 * */

void draw_time(xpm_image_t font, int xi, int yi, Time time);



