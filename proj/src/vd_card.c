#include <lcom/lcf.h>
#include <stdint.h>

#include "vd_card.h"


static char *video_mem; /* virtual address to which VRAM is mapped */

static uint16_t hres; /* XResolution */
static uint16_t vres; /* YResolution */
static uint16_t bits_per_pixel; /* VRAM bits per pixel */

static uint8_t red_mask_size, green_mask_size, blue_mask_size;
static uint8_t red_field_position, green_field_position, blue_field_position;

uint16_t vg_get_hres() { return hres; }

uint16_t vg_get_vres() { return vres; }

int (vbe_get_mode_info_remade)(uint16_t mode, vbe_mode_info_t *vmi_p) {
    mmap_t buff;

    int tries = 4;

    while(tries > 0) {
        tries--;
        if (lm_alloc(sizeof(vbe_mode_info_t),&buff) != NULL)
            break;
    }

    if (tries == 0) {
        printf("ERROR\n");
        return 1;
    }

    reg86_t r;
    memset(&r,0,sizeof(r));

    r.intno = 0x10;
    r.cx = mode;
    r.ax = 0x4F01;
    r.es = PB2BASE(buff.phys);
    r.di = PB2OFF(buff.phys);

    if (sys_int86(&r) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
        if (lm_free(&buff) != OK)
            return 1;
		return 1;
	}

    if (r.al != 0x4F || r.ah != 0x00) {
        return 1;
    }

    *vmi_p = *((vbe_mode_info_t*) buff.virt);

    if (lm_free(&buff) != OK) 
        return 1;

    return 0;
}


void* vggg_init(unsigned short mode) {
    vbe_mode_info_t vbe_mode;
    vbe_get_mode_info_remade(mode, &vbe_mode);

    hres = vbe_mode.XResolution;
    vres = vbe_mode.YResolution;
    bits_per_pixel = vbe_mode.BitsPerPixel;

    red_mask_size = vbe_mode.RedMaskSize;
    green_mask_size = vbe_mode.GreenMaskSize;
    blue_mask_size = vbe_mode.BlueMaskSize;

    red_field_position = vbe_mode.RedFieldPosition;
    green_field_position = vbe_mode.GreenFieldPosition;
    blue_field_position = vbe_mode.BlueFieldPosition;

    unsigned vram_size = hres * vres * ((bits_per_pixel + 7) / 8);

    int r;
    struct minix_mem_range mr;

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vbe_mode.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    reg86_t t;
    memset(&t, 0, sizeof(t)); /* wipe the struct */
	t.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	t.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	t.intno = 0x10;

	if (sys_int86(&t) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	return (void*) vbe_mode.PhysBasePtr;
}

int square_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int l = 0; l < height; l++) {
        if (vg_draw_hlineee(x , y+l, width, color) != OK)
            return 1;
    }
    return OK;
}

int vg_draw_hlineee(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len;i++) {
        if (vg_paint_pixel(x+i,y,color) != OK)
            return 1;
    }
    return OK;
}

int vg_paint_pixel(uint16_t x_coord, uint16_t y_coord, uint32_t color) {
    if (x_coord > hres || y_coord > vres) {
        return 1;
    }

    if (bits_per_pixel == 8) { 
        memset(video_mem + hres*y_coord + x_coord, color, 1);
    }
<<<<<<< HEAD:proj/src/vd_card.c
    else if (bits_per_pixel == 15) {
        memcpy(video_mem + hres*y_coord*2 + x_coord*2, &color, 2);
=======
    else if(bits_per_pixel == 15){
        memcpy(video_mem + hres*y_coord*2 + vres*x_coord*2, &color, 2);
    }
    else {
        memcpy(video_mem + hres*y_coord*(bits_per_pixel/8) + x_coord*(bits_per_pixel/8), &color, (bits_per_pixel/8));
    }
    return OK;
}

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    /*uint16_t no_lines = vres / no_rectangles;
    uint16_t no_columns = hres / no_rectangles;
    uint32_t color;

    uint8_t r,g,b,firstR,firstG,firstB;

    if (mode == 0x105) { //Indexed mode
        for (int col = 0; col < no_columns; col++) {
            for (int row = 0; row < no_lines; row++) {
                color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
                if (square_draw(col*no_columns, row*no_lines,no_columns,no_lines,color) != OK) {
                    printf("ERROR!\n");
                    return 1;
                }
                    
            }
        }
>>>>>>> 2ae7ceabe6083879afa7c25a52912788cfaa793f:proj/vd_card.c
    }
    else {
        memcpy(video_mem + hres*y_coord*(bits_per_pixel/8) + x_coord*(bits_per_pixel/8), &color, (bits_per_pixel/8));
    }
    return OK;
}

void(vg_draw_xpm)(uint32_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y) {
    int width = img.width;
    int height = img.height;

    for(int dy = 0; dy < height; dy++){
        for(int dx = 0; dx < width ; dx++){
<<<<<<< HEAD:proj/src/vd_card.c
            vg_paint_pixel(x+dx, y+dy, pixmap[(dx + width*dy)]);
=======
            vg_paint_pixelll(x+dx, y+dy, pixmap[(dx + width*dy)]);
>>>>>>> 2ae7ceabe6083879afa7c25a52912788cfaa793f:proj/vd_card.c
        }
    }
}
