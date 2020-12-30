#include <lcom/lcf.h>
#include <stdint.h>

#include "Player.h"

/*
//EXACTLY THE SAME AS IN MENU
Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright)
{
    Button *button = malloc(sizeof(Button));

    if (button == NULL)
        return NULL;

    xpm_image_t img;
    xpm_load(normal, XPM_8_8_8_8, &img);
    button->normal = img;

    xpm_load(bright, XPM_8_8_8_8, &img);
    button->bright = img;

    button->xi = xi;
    button->yi = yi;
    button->xf = xi + button->normal.width;
    button->yf = yi + button->normal.height;
    button->bright_ = false;
    return button;
}

//EXACTLY THE SAME AS IN MENU
Cursor *load_cursor(xpm_row_t *img_cursor)
{
    Cursor *cursor = malloc(sizeof(Cursor));

    if (cursor == NULL)
        return NULL;

    xpm_image_t img;
    xpm_load(cursor_xpm, XPM_8_8_8_8, &img);
    cursor->cursor_image = img;
    cursor->x = 50;
    cursor->y = 50;
    return cursor;
}*/

Player* load_player(Avatar* default_avatar)
{
    //Allocating memory
    Player *player = (Player *)malloc(sizeof(Player));
    player->x = 10;
    player->y = 10;
    
    player->name[0] = 'P';
    player->name[1] = 'L';
    player->name[2] = 'A';
    player->name[3] = 'Y';
    player->name[4] = 'E';
    player->name[5] = 'R';
    player->name[6] = ' ';
    player->name_max_size = 7;
    player->avatar = default_avatar->normal;
    return player;
}

Avatar* load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small) {
    Avatar *avatar = malloc(sizeof(Avatar));
    if (avatar == NULL)
        return NULL;

    xpm_image_t img;
    xpm_load(normal, XPM_8_8_8_8, &img);
    avatar->normal = img;

    xpm_load(selected, XPM_8_8_8_8, &img);
    avatar->selected = img;

    xpm_load(small, XPM_8_8_8_8, &img);
    avatar->small = img;

    avatar->atual = avatar->normal;
    avatar->state = NOT_SELECTED;
    
    return avatar;
}

Player_Settings *load_player_settings() {
    Player_Settings *player_settings = malloc(sizeof(Player_Settings));

    xpm_image_t img;
    xpm_load(player_background_xpm, XPM_8_8_8_8, &img);
    player_settings->background = img;
    xpm_load(player_title_xpm, XPM_8_8_8_8, &img);
    player_settings->background_title = img;
    xpm_load(font_xpm, XPM_8_8_8_8, &img);
    player_settings->font = img;
    xpm_load(name_length_warning_xpm, XPM_8_8_8_8, &img);
    player_settings->name_length_warning = img;

    player_settings->avatars[0] = load_avatar(hammer_0_big_xpm, hammer_0_big_bright_xpm, hammer_0_small_xpm);
    player_settings->avatars[1] = load_avatar(hammer_1_big_xpm, hammer_1_big_bright_xpm, hammer_1_small_xpm);
    player_settings->avatars[2] = load_avatar(hammer_2_big_xpm, hammer_2_big_bright_xpm, hammer_2_small_xpm);
    player_settings->avatars[3] = load_avatar(hammer_3_big_xpm, hammer_3_big_bright_xpm, hammer_3_small_xpm);
    
    player_settings->avatars[3]->state = SELECTED;

    player_settings->left_arrow = load_button(275, 300, arrow_left_normal_xpm, arrow_left_active_xpm);
    player_settings->right_arrow = load_button(450, 300, arrow_right_normal_xpm, arrow_right_active_xpm);
    player_settings->start = load_button(677, 415, start_normal_xpm, start_active_xpm);
    player_settings->name_box = load_button(0, 375, name_box_normal_xpm, name_box_active_xpm);

    player_settings->cursor = load_cursor(cursor_xpm);

    player_settings->player = load_player(player_settings->avatars[0]);
    player_settings->name_maximum_length = false;
    return player_settings;
}





void draw_player(Player *player)
{
    uint32_t *pixmap = (uint32_t *)player->avatar.bytes;
    // copy it to graphics memory
    vg_draw_xpm(pixmap, player->avatar, player->x, player->y);
}

void draw_background__(Player_Settings *player_settings)
{
    uint32_t *background_map = (uint32_t *)player_settings->background.bytes;
    vg_draw_xpm(background_map, player_settings->background, 0, 0);
    uint32_t *background_title_map = (uint32_t *)player_settings->background_title.bytes;
    vg_draw_xpm(background_title_map, player_settings->background_title, 0, 0);
}

void draw_name_lenght_warning(Player_Settings* player_settings) {
  uint32_t *name_length_warning_map = (uint32_t *)player_settings->name_length_warning.bytes;
  vg_draw_xpm(name_length_warning_map, player_settings->name_length_warning, 300, 426);
}

void draw_avatars(Avatar* avatars[4]) {
    //there are only 4 possible avatars to chose from
    for (int i = 0; i < 4; i++) {
        //update mole position
        switch (avatars[i]->state) {
        case NOT_SELECTED:
            avatars[i]->atual = avatars[i]->normal;
            break;
        case SELECTED:
            avatars[i]->atual = avatars[i]->selected;
            break;
        default:
            break;
        }
    }

    uint32_t *player_avatar1_map = (uint32_t *)avatars[0]->atual.bytes;
    vg_draw_xpm(player_avatar1_map, avatars[0]->atual, 80, 150);

    uint32_t *player_avatar2_map = (uint32_t *)avatars[1]->atual.bytes;
    vg_draw_xpm(player_avatar2_map, avatars[1]->atual, 260, 150);

    uint32_t *player_avatar3_map = (uint32_t *)avatars[2]->atual.bytes;
    vg_draw_xpm(player_avatar3_map, avatars[2]->atual, 440, 150);

    uint32_t *player_avatar4_map = (uint32_t *)avatars[3]->atual.bytes;
    vg_draw_xpm(player_avatar4_map, avatars[3]->atual, 620, 150);
}

void draw_buttons__(Player_Settings* player_settings) {
    if (player_settings->left_arrow->bright_){
        uint32_t* left_arrow_bright_map = (uint32_t*) player_settings->left_arrow->bright.bytes;
        vg_draw_xpm(left_arrow_bright_map, player_settings->left_arrow->bright, player_settings->left_arrow->xi , player_settings->left_arrow->yi);
    }
    else{
        uint32_t* left_arrow_normal_map = (uint32_t*) player_settings->left_arrow->normal.bytes;
        vg_draw_xpm(left_arrow_normal_map, player_settings->left_arrow->normal, player_settings->left_arrow->xi , player_settings->left_arrow->yi);
    }

    if (player_settings->right_arrow->bright_){
        uint32_t* right_arrow_bright_map = (uint32_t*) player_settings->right_arrow->bright.bytes;
        vg_draw_xpm(right_arrow_bright_map, player_settings->right_arrow->bright, player_settings->right_arrow->xi , player_settings->right_arrow->yi);
    }
    else{
        uint32_t* right_arrow_normal_map = (uint32_t*) player_settings->right_arrow->normal.bytes;
        vg_draw_xpm(right_arrow_normal_map, player_settings->right_arrow->normal, player_settings->right_arrow->xi , player_settings->right_arrow->yi);
    }

    if (player_settings->start->bright_){
        uint32_t* start_bright_map = (uint32_t*) player_settings->start->bright.bytes;
        vg_draw_xpm(start_bright_map, player_settings->start->bright, player_settings->start->xi , player_settings->start->yi);
    }
    else{
        uint32_t* start_normal_map = (uint32_t*) player_settings->start->normal.bytes;
        vg_draw_xpm(start_normal_map, player_settings->start->normal, player_settings->start->xi , player_settings->start->yi);
    }

    if (player_settings->name_box->bright_){
        uint32_t* name_box_bright_map = (uint32_t*) player_settings->name_box->bright.bytes;
        vg_draw_xpm(name_box_bright_map, player_settings->name_box->bright, player_settings->name_box->xi , player_settings->name_box->yi);
    }
    else{
        uint32_t* start_normal_map = (uint32_t*) player_settings->name_box->normal.bytes;
        vg_draw_xpm(start_normal_map, player_settings->name_box->normal, player_settings->name_box->xi , player_settings->name_box->yi);
    }
}

/*
void draw_cursor(Cursor *cursor) {
  uint32_t* cursor_map = (uint32_t*) cursor->cursor_image.bytes;
  vg_draw_xpm(cursor_map, cursor->cursor_image, cursor->x, cursor->y);
}*/

void draw_menu_player_settings(Player_Settings* player_settings) {
    draw_background__(player_settings);
    draw_avatars(player_settings->avatars);
    draw_buttons__(player_settings);
    draw_player_name(player_settings->font, 0, 420, player_settings->player->name, player_settings->player->name_max_size);
    if (player_settings->name_maximum_length) {
      draw_name_lenght_warning(player_settings);
    }
    draw_cursor(player_settings->cursor);
}

/*
int mouse_over(Button *button, Cursor *cursor) {
  if (button->xi <= cursor->x && cursor->x <= button->xf && button->yi <= cursor->y && cursor->y <= button->yf)
    return 1;
  else
    return 0;
}*/


void move_cursor__(struct packet *packet, Player_Settings* player_settings) {
  //Update x position
  player_settings->cursor->x += packet->delta_x;
  if (player_settings->cursor->x > 790) {
    player_settings->cursor->x = 790 - player_settings->cursor->cursor_image.width;
  }
  else if (player_settings->cursor->x < 0) {
    player_settings->cursor->x = 0;
  }

  //Update y position
  player_settings->cursor->y -= packet ->delta_y;
  if (player_settings->cursor->y > 590) {
    player_settings->cursor->y = 590 - player_settings->cursor->cursor_image.height;
  }
  else if (player_settings->cursor->y < 0) {
    player_settings->cursor->y = 0;
  }

  if (mouse_over(player_settings->left_arrow, player_settings->cursor))
    player_settings->left_arrow->bright_ = true;
  else if (mouse_over(player_settings->right_arrow, player_settings->cursor))
    player_settings->right_arrow->bright_ = true;
  else if (mouse_over(player_settings->start, player_settings->cursor))
    player_settings->start->bright_ = true;
  else { 
    player_settings->left_arrow->bright_ = false;
    player_settings->right_arrow->bright_ = false;
    player_settings->start->bright_ = false;
  }
}

void move_left_avatar(Player_Settings* player_settings) {
  if (player_settings->avatars[0]->state != SELECTED) {
    for (int i = 1; i <= 3; i++) {
      if (player_settings->avatars[i]->state == SELECTED) {
        player_settings->avatars[i]->state = NOT_SELECTED;
        player_settings->avatars[i-1]->state = SELECTED;
      }
    }
  }
}

void move_right_avatar(Player_Settings* player_settings) {
  if (player_settings->avatars[3]->state != SELECTED) {
    for (int i = 2; i >= 0; i--) {
      if (player_settings->avatars[i]->state == SELECTED) {
        player_settings->avatars[i]->state = NOT_SELECTED;
        player_settings->avatars[i+1]->state = SELECTED;
      }
    }
  }
}

xpm_image_t get_hammer(Player_Settings* player_settings) {
  for (int i = 0; i <= 3; i++) {
    if (player_settings->avatars[i]->state == SELECTED) {
      return player_settings->avatars[i]->small;
    }
  }
  return player_settings->avatars[0]->small;
}

void draw_player_name(xpm_image_t font, int xi, int yi, char name[], int name_size) {
  uint32_t *font_pixmap = (uint32_t *)font.bytes;
  int letter_width = font.width / (26/2); // 26 letters in two rows equals 13 letter per row
  int letter_height = font.height / 2; //two rows of letters
  printf("--- ");
  printf("%d", font.height);
  printf(" ---\n");
  int index = 0;
  for (int i = 0; i < name_size; i++) {
    if (name[i] == ' ')
      break;
    index = (int)name[i] - (int)'A';
    if (index < 13) {
        //first row
        vg_draw_part_of_xpm(font_pixmap, font, xi+i*letter_width, yi, index*letter_width, index*letter_width + letter_width, 0, letter_height);
    }
    else {
        //second row
        vg_draw_part_of_xpm(font_pixmap, font, xi+i*letter_width, yi, index*letter_width, index*letter_width + letter_width, letter_height, letter_height*2);
    }
  }
}

void update_player_name(Player_Settings* player_settings, bool delete_letter, char new_letter) {
  if (delete_letter) { //searches for the first empty space and deletes the letter before this space
    if (player_settings->player->name[player_settings->player->name_max_size-1] != ' ') {
      player_settings->player->name[player_settings->player->name_max_size-1] = ' ';
      player_settings->name_maximum_length = false;
      return;
    }
    for (int i = 0; i < player_settings->player->name_max_size; i++) {
      if (player_settings->player->name[i] == ' ' && i != 0) { 
        player_settings->player->name[i-1] = ' ';
        player_settings->name_maximum_length = false;
        return;
      }
    }
  }
  else { //search for the first empty space to add a new letter
    for (int i = 0; i < player_settings->player->name_max_size; i++) {
      if (player_settings->player->name[i] == ' ') {
        player_settings->player->name[i] = new_letter;
        if (i == (player_settings->player->name_max_size-1))
          player_settings->name_maximum_length = true;
        return;
      }
    }
  }
  
}
