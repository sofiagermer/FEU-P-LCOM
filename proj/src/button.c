#include <lcom/lcf.h>
#include <stdint.h>
#include "button.h"


Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright)
{
  Button *button = (Button *) malloc(sizeof(Button));

  if (button == NULL)
    return NULL;

  xpm_load(normal, XPM_8_8_8_8, &(button->sprites[1]));
  xpm_load(bright, XPM_8_8_8_8, &(button->sprites[1]));
  button->xi = xi;
  button->yi = yi;
  button->xf = xi + button->sprites[0].width;
  button->yf = yi + button->sprites[0].height;
  button->state = NORMAL;
  return button;
}


void update_buttons(Cursor* cursor, Button** buttons, int num_buttons) {
  for (int i = 0; i < num_buttons; i++) {
    if (mouse_over(buttons[i], cursor))
      buttons[i]->state = ACTIVE;
    else
      buttons[i]->state = NORMAL;
  }
}


void draw_button(Button *button)
{
  xpm_image_t current_img = button->sprites[(int) button->state];
  vg_draw_xpm((uint32_t*) current_img.bytes, current_img, button->xi, button->yi);
}


void draw_buttons(Button** buttons, int num_buttons)
{
  for (int i = 0; i < num_buttons; i++) {
    draw_button(buttons[i]);
  }
}


int mouse_over(Button *button, Cursor *cursor)
{
  if (button->xi <= cursor->x && cursor->x <= button->xf && button->yi <= cursor->y && cursor->y <= button->yf)
    return 1;
  else
    return 0;
}
