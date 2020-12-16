#include "menu.h"
#include <lcom/lcf.h>

//MOUSE
extern bool mouse_last_byte_of_packet;
extern uint8_t packet[];

//background xpm
xpm_image_t background;

Button *create_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright) {
  Button *button = malloc(sizeof(Button));

  if (button == NULL)
    return NULL;

  xpm_image_t img;
  xpm_load(normal, XPM_8_8_8_8, &img);
  button->normal = img;

  xpm_load(bright, XPM_8_8_8_8, &img);
  button-> bright = img;

  button->xi = xi;
  button->yi = yi;
  button->xf = xi + button->normal.width;
  button->yf = yi + button->normal.height;
  button->bright_ = false;
  return button;
}

Cursor *create_cursor(xpm_image_t *img_cursor){
  Cursor *cursor = malloc(sizeof(Cursor));

  if(cursor == NULL)
    return NULL;

  xpm_image_t img;
  xpm_load(cursor_xpm, XPM_8_8_8_8, &img);
  cursor->cursor_xpm = img;
  cursor-> x = 50;
  cursor-> y = 50;
  return cursor;
}

Menu *create_menu() {
  Menu *menu = malloc(sizeof(Menu));
  xpm_image_t img;
  xpm_load(logo_xpm, XPM_8_8_8_8, &img);
  menu->logo =img;
  //ver coordenadas onde vou por os botões
  menu->single_player_button = create_button(699, 570, exit_xpm, exit_bright_xpm);
  menu->multi_player_button = create_button(280, 400, singleplayer_xpm, singleplayer_bright_xpm);
  menu->exit_button = create_button(280, 300, multiplayer_xpm, multiplayer_bright_xpm);
  return menu;
}

void draw_background(){
    uint32_t *background_map = (uint32_t *)background.bytes;
    vg_draw_xpm(background_map,background, 0, 0);
}

void draw_menu(Menu *menu) {
    draw_background();

    uint32_t* logo_map = (uint32_t*) menu->logo.bytes;
    vg_draw_xpm(logo_map, menu->logo, 119 , 88);

    if (menu->single_player_button->bright_){
        uint32_t* sp_normal_map = (uint32_t*) menu->single_player_button->bright.bytes;
        vg_draw_xpm(sp_normal_map, menu->single_player_button->bright, 214 , 313);
    }
    else{
        uint32_t* sp_bright_map = (uint32_t*) menu->single_player_button->normal.bytes;
        vg_draw_xpm(sp_bright_map, menu->single_player_button->normal, 214 , 313);
    }

    if (menu->multi_player_button->bright_){
        uint32_t* mp_normal_map = (uint32_t*) menu->multi_player_button->bright.bytes;
        vg_draw_xpm(mp_normal_map, menu->multi_player_button->bright, 119 , 88);
    }
    else{
        uint32_t* mp_bright_map = (uint32_t*) menu->multi_player_button->normal.bytes;
        vg_draw_xpm(mp_bright_map, menu->multi_player_button->normal, 119 , 88);
    }

    if (menu->exit_button->bright_){
        uint32_t* ex_normal_map = (uint32_t*) menu->exit_button->bright.bytes;
        vg_draw_xpm(ex_normal_map, menu->exit_button->bright, 119 , 88);
    }
    else{
        uint32_t* ex_bright_map = (uint32_t*) menu->exit_button->normal.bytes;
        vg_draw_xpm(ex_bright_map, menu->exit_button->normal, 119 , 88);
    }
}

void draw_cursor(Cursor *cursor){
  uint32_t* cursor_map = (uint32_t*) cursor->cursor_xpm.bytes;
  vg_draw_xpm(cursor_map, cursor->cursor_xpm, 0, 0);
}

int mouse_over(Button *button, Cursor *cursor) {
  if (button->xi <= cursor->x && cursor->x <= button->xf && button->yi <= cursor->y && cursor->y <= button->yf)
    return 1;
  else
    return 0;
}

void move_cursor(Cursor *cursor, int16_t dx, int16_t dy) {
  cursor->x += dx;
  if (cursor->x > 790) {
    cursor->x = 790;
  }
  else if (cursor->x < 0) {
    cursor->x = 0;
  }

  cursor->y -= dy;
  if (cursor->y > 590) {
    cursor->y = 590;
  }
  else if (cursor->y < 0) {
    cursor->y = 0;
  }
}

