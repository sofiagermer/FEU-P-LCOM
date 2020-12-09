#ifndef _VD_CARD_H_
#define _VD_CARD_H_

#include <lcom/lcf.h>

uint16_t vg_get_hres();

uint16_t vg_get_vres();

int (vbe_get_mode_info_remade)(uint16_t mode, vbe_mode_info_t *vmi_p);

void *vggg_init(unsigned short mode);

int vggg_exit();

int square_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_draw_hlineee(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int vg_paint_pixel(uint16_t x_coord, uint16_t y_coord, uint32_t color);


void(vg_draw_xpm)(uint32_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y);

<<<<<<< HEAD:proj/src/vd_card.h
#endif /* _VD_CARD_H_ */
=======
void(vg_draw_xpm)(uint32_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y);
>>>>>>> 2ae7ceabe6083879afa7c25a52912788cfaa793f:proj/vd_card.h