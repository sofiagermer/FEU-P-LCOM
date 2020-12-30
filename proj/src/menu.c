#include "menu.h"
#include <lcom/lcf.h>

//MOUSE
extern bool mouse_last_byte_of_packet;
extern uint8_t packet[];

//background xpm
xpm_image_t background;

Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright) {
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

Cursor *load_cursor(xpm_row_t *img_cursor){
  Cursor *cursor = malloc(sizeof(Cursor));

  if(cursor == NULL)
    return NULL;

  xpm_image_t img;
  xpm_load(cursor_xpm, XPM_8_8_8_8, &img);
  cursor->cursor_image = img;
  cursor-> x = 50;
  cursor-> y = 50;
  return cursor;
}

Menu *load_menu() {
  Menu *menu = malloc(sizeof(Menu));

  xpm_image_t img;
  xpm_load(logo_xpm, XPM_8_8_8_8, &img);
  menu->logo =img;
  //ver coordenadas onde vou por os botões
  menu->single_player_button = load_button(300, 400, singleplayer_normal_xpm, singleplayer_active_xpm);
  menu->multi_player_button = load_button(300, 450, multiplayer_normal_xpm, multiplayer_active_xpm);
  menu->exit_button = load_button(300, 500, exit_normal_xpm, exit_active_xpm);

  menu->cursor = load_cursor(cursor_xpm);

  return menu;
}

void load_background(){
    xpm_load(background_xpm, XPM_8_8_8_8, &background);
}

void draw_background(){
    uint32_t *background_map = (uint32_t *)background.bytes;
    vg_draw_xpm(background_map,background, 0, 0);
}

void draw_logo(Menu *menu){
  uint32_t* logo_map = (uint32_t*) menu->logo.bytes;
  vg_draw_xpm(logo_map, menu->logo, 168 , 118);
}

void draw_buttons(Menu * menu){
   if (menu->single_player_button->bright_){
        uint32_t* sp_normal_map = (uint32_t*) menu->single_player_button->bright.bytes;
        vg_draw_xpm(sp_normal_map, menu->single_player_button->bright, 300 , 400);
    }
    else{
        uint32_t* sp_bright_map = (uint32_t*) menu->single_player_button->normal.bytes;
        vg_draw_xpm(sp_bright_map, menu->single_player_button->normal, 300 , 400);
    }

    if (menu->multi_player_button->bright_){
        uint32_t* mp_normal_map = (uint32_t*) menu->multi_player_button->bright.bytes;
        vg_draw_xpm(mp_normal_map, menu->multi_player_button->bright, 300 , 450);
    }
    else{
        uint32_t* mp_bright_map = (uint32_t*) menu->multi_player_button->normal.bytes;
        vg_draw_xpm(mp_bright_map, menu->multi_player_button->normal, 300 , 450);
    }

    if (menu->exit_button->bright_){
        uint32_t* ex_normal_map = (uint32_t*) menu->exit_button->bright.bytes;
        vg_draw_xpm(ex_normal_map, menu->exit_button->bright, 300 , 500);
    }
    else{
        uint32_t* ex_bright_map = (uint32_t*) menu->exit_button->normal.bytes;
        vg_draw_xpm(ex_bright_map, menu->exit_button->normal, 300 , 500);
    }
}
void draw_menu(Menu *menu) {
    draw_background();
    draw_logo(menu);
    draw_buttons(menu);
}

void draw_cursor(Cursor *cursor){
  uint32_t* cursor_map = (uint32_t*) cursor->cursor_image.bytes;
  vg_draw_xpm(cursor_map, cursor->cursor_image, cursor->x, cursor->y);
}

int mouse_over(Button *button, Cursor *cursor) {
  if (button->xi <= cursor->x && cursor->x <= button->xf && button->yi <= cursor->y && cursor->y <= button->yf)
    return 1;
  else
    return 0;
}

void check_cursor_over_button(Menu *menu){
  if(mouse_over(menu->single_player_button, menu->cursor))
    menu->single_player_button->bright_ = true;
  else if(mouse_over(menu->multi_player_button, menu->cursor))
    menu->multi_player_button->bright_ = true;
  else if(mouse_over(menu->exit_button, menu-> cursor))
    menu->exit_button->bright_ =true;
  else{ 
    menu->single_player_button->bright_ = false;
    menu->multi_player_button->bright_ = false;
    menu->exit_button->bright_ = false;
}
}

void move_cursor(struct packet *packet, Cursor *cursor) {
  cursor->x += packet->delta_x;
  if (cursor->x > 790) {
    cursor->x = 790 - cursor->cursor_image.width;
  }
  else if (cursor->x < 0) {
    cursor->x = 0;
  }

  cursor->y -= packet ->delta_y;
  if (cursor->y > 590) {
    cursor->y = 590 - cursor->cursor_image.height;
  }
  else if (cursor->y < 0) {
    cursor->y = 0;
  }
}


