#include <lcom/lcf.h>

#include <Sprites/player_background.xpm>
#include <Sprites/player_title.xpm>


#include <Sprites/player_avatars/hammer_0_big.xpm>
#include <Sprites/player_avatars/hammer_1_big.xpm>
#include <Sprites/player_avatars/hammer_2_big.xpm>
#include <Sprites/player_avatars/hammer_3_big.xpm>

#include <Sprites/player_avatars/hammer_0_big_bright.xpm>
#include <Sprites/player_avatars/hammer_1_big_bright.xpm>
#include <Sprites/player_avatars/hammer_2_big_bright.xpm>
#include <Sprites/player_avatars/hammer_3_big_bright.xpm>

#include <Sprites/player_avatars/hammer_0_small.xpm>
#include <Sprites/player_avatars/hammer_1_small.xpm>
#include <Sprites/player_avatars/hammer_2_small.xpm>
#include <Sprites/player_avatars/hammer_3_small.xpm>

#include <Sprites/arrow_left.xpm>
#include <Sprites/arrow_left_bright.xpm>
#include <Sprites/arrow_right.xpm>
#include <Sprites/arrow_right_bright.xpm>
#include <Sprites/start.xpm>
#include <Sprites/start_bright.xpm>

#include "menu.h"

typedef enum {NOT_SELECTED, SELECTED} button_state;

typedef struct  {
    xpm_image_t avatar;
    int x,y;
    //char name[10];
} Player;

typedef struct  {
    xpm_image_t normal;
    xpm_image_t selected;
    xpm_image_t small;
    xpm_image_t atual;
    button_state state;
} Avatar;

typedef struct  {
    xpm_image_t background;
    xpm_image_t background_title;
    Player* player;
    Avatar* avatars[4];
    Button* left_arrow;
    Button* right_arrow;
    Button* start;
    Cursor* cursor;
} Player_Settings;



//Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright);

//Cursor *load_cursor(xpm_row_t *img_cursor);

Player* load_player(Avatar* default_avatar);

Avatar* load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small);

Player_Settings *load_player_settings();


void draw_player(Player *player);

void draw_background__(Player_Settings *player_settings);

void draw_avatars(Avatar* avatars[]);

void draw_buttons__(Player_Settings* player_settings);

void draw_menu_player_settings(Player_Settings* player_settings);

void draw_cursor(Cursor *cursor);

int mouse_over(Button *button, Cursor *cursor);

void move_cursor__(struct packet *packet, Player_Settings* player_settings);

void move_left_avatar(Player_Settings* player_settings);

void move_right_avatar(Player_Settings* player_settings);

xpm_image_t get_hammer(Player_Settings* player_settings);
