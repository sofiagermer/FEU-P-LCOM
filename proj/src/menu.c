#include "menu.h"
#include <lcom/lcf.h>

Button *create_button(uint16_t xi, uint16_t yi, xpm_image_t *normal, xpm_image_t *bright) {
  Button *button = malloc(sizeof(Button));

  if (button == NULL)
    return NULL;

  xpm_load(normal, XPM_8_8_8_8, &button->normal);
  xpm_load(bright, XPM_8_8_8_8, &button-> bright);
  button->xi = xi;
  button->yi = yi;
  button->xf = xi + but->normal->width;
  button->yf = yi + but->normal->height;
  button->highlighted = false;

  return button;
}

Cursor *create_cursor(xpm_image_t *img_cursor){
  Cursor *cursor = malloc(sizeof(Cursor));

  if(cursor == NULL)
    return NULL;
  xpm_load(img_cursor, XPM_8_8_8_8, &cursor->cursor_xpm);
  cursor-> x = 50;
  cursor-> y = 50;
}

Menu *create_menu() {
  Menu *menu = malloc(sizeof(menu_t));
  xpm_load(background_xpm, XPM_8_8_8_8, &menu->background);
  xpm_load(logo_xpm, XPM_8_8_8_8, &menu->logo);
  xpm_load(logo_xpm, XPM_8_8_8_8, &menu->cursor);
  //ver coordenadas onde vou por os botões
  menu->single_player_button = create_button(699, 570, exit_xpm, exit_bright_xpm);
  menu->multi_player_button = create_button(280, 400, singleplayer_xpm, singleplayer_bright_xpm);
  menu->exit_button = create_button(280, 300, multiplayer_xpm, multiplayer_bright_xpm);
  return menu;
}

void draw_menu(menu_t *menu) {
    uint32_t* background_map = (uint32_t*) menu->background.bytes;
    vg_draw_xpm(background_map, menu->background, 0, 0);

    uint32_t* logo_map = (uint32_t*) menu->logo.bytes;
    vg_draw_xpm(logo_map, menu->logo, 119 , 88);

    if (menu->single_player_button->bright){
        uint32_t* sp_normal_map = (uint32_t*) menu->single_player_button.bytes;
        vg_draw_xpm(sp_normal_map, menu->single_player_button, 214 , 313);
    }
    else{
        uint32_t* sp_bright_map = (uint32_t*) menu->single_player_button.bytes;
        vg_draw_xpm(sp_bright_map, menu->single_player_button, 214 , 313);
    }

    if (menu->multi_player_button->bright){
        uint32_t* mp_normal_map = (uint32_t*) menu->multi_player_button.bytes;
        vg_draw_xpm(mp_normal_map, menu->multi_player_button, 119 , 88);
    }
    else{
        uint32_t* mp_bright_map = (uint32_t*) menu->multi_player_button.bytes;
        vg_draw_xpm(mp_bright_map, menu->multi_player_button, 119 , 88);
    }

    if (menu->exit_button->bright)
        uint32_t* ex_normal_map = (uint32_t*) menu->exit_button.bytes;
        vg_draw_xpm(ex_normal_map, menu->exit_button, 119 , 88);
    }
    else{
        uint32_t* ex_bright_map = (uint32_t*) menu->exit_button.bytes;
        vg_draw_xpm(ex_bright_map, menu->exit_button, 119 , 88);
    }

    uint32_t* cursor_map = (uint32_t*) menu->cursor.bytes;
    vg_draw_xpm(cursor_map, menu->cursor, 50 , 50);
}


int mouse_over(Button *button, Cursor *cursor) {
  if (but->xi <= cursor->x && cursor->x <= but->xf && but->yi <= cursor->y && cursor->y <= but->yf)
    return 1;
  else
    return 0;
}

void move_cursor(Menu *menu, int16_t dx, int16_t dy) {
  menu->cursor->x += dx;
  if (menu->cursor->x > 790) {
    menu->cursor->x = 790;
  }
  else if (menu->cursor->x < 0) {
    menu->cursor->x = 0;
  }

  menu->cursor->y -= dy;
  if (menu->cursor->y > 590) {
    menu->cursor->y = 590;
  }
  else if (menu->cursor->y < 0) {
    menu->cursor->y = 0;
  }
}

void loop_choose_button(){
    int ipc_status, r;
    message msg;
    uint16_t mouse_id;
    
    uint16_t x_cursor = 50;
    uint16_t y_cursor = 50;
    while (cnt > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_id) { // subscribed interrupt BIT MASK
            mouse_ih();
            if (mouse_last_byte_of_packet) {
              struct packet new_packet;
              mouse_parse_packet(packet, &new_packet);
              move_cursor(cursor_map, menu->logo, new_packet.dx , new_packet.dy);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }
}
