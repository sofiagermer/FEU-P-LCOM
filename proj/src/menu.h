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

#include "state_machine.h"
#include "button.h"

/**
 *  @brief Struct button_t. Acts as class BUTTON.
 *  A button has two xpms: a default and a bright xpm.
 *  The normal xpm is the xpm that we are choosing, when one button is in it's normal state the others are bright
 *  A button also holds information about the coordinates from where it starts and to where it ends.
**/
/* 
typedef enum_click{
    single_player_click,
    multiplayer_click,
    exit_click,
    no_click = -1
}click_t; */

typedef struct {
    xpm_image_t logo;         
    Button** buttons;
    int num_buttons;
    Cursor *cursor;              
} Menu;

void load_background();
Menu *load_menu();
void draw_background();
void draw_logo(Menu *menu);
void draw_menu(Menu *menu);
