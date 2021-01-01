#include <lcom/lcf.h>

#include <Sprites/player_background.xpm>
#include <Sprites/player_title.xpm>
#include <Sprites/name_length_warning.xpm>
#include <Sprites/font.xpm>
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
#include <Sprites/Buttons_img/normal/name_box_normal.xpm>
#include <Sprites/Buttons_img/active/name_box_active.xpm>
#include <Sprites/Buttons_img/normal/arrow_left_normal.xpm>
#include <Sprites/Buttons_img/active/arrow_left_active.xpm>
#include <Sprites/Buttons_img/normal/arrow_right_normal.xpm>
#include <Sprites/Buttons_img/active/arrow_right_active.xpm>
#include <Sprites/Buttons_img/normal/start_normal.xpm>
#include <Sprites/Buttons_img/active/start_active.xpm>

#include "menu.h"

typedef enum {NOT_SELECTED, SELECTED} button_state;

typedef struct  {
    xpm_image_t avatar;
    int missed_moles;
    int hitted_moles;
    char* name;
    int max_name_length;
} Player;

typedef struct  {
    xpm_image_t sprites[3];
    button_state state;
    uint8_t x,y;
} Avatar;

typedef struct  {
    xpm_image_t background;
    xpm_image_t background_title;
    xpm_image_t name_length_warning;
    xpm_image_t font;
    bool name_maximum_length;
    //Player* player;
    Avatar* avatars[4];
    Button** buttons;
    int num_buttons;
} Player_Settings;


Player* load_player(Avatar* default_avatar);


Avatar* load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small);


Player_Settings *load_player_settings();


void draw_background__(Player_Settings *player_sets);


void draw_name_lenght_warning(Player_Settings* player_sets);


void draw_avatars(Avatar* avatars[]);


void move_left_avatar(Player_Settings* player_sets);


void move_right_avatar(Player_Settings* player_sets);


xpm_image_t get_hammer(Player_Settings* player_sets);


void draw_player_name(xpm_image_t font, int xi, int yi, char name[], int name_size);


void update_player_name(Player_Settings* player_settings, Player *player, bool delete_letter, char new_letter);
