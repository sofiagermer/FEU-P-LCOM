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

void* vggg_init(unsigned short mode) {
    vbe_mode_info_t vbe_mode;
    vbe_get_mode_info(mode, &vbe_mode);

    hres = vbe_mode.XResolution;
    vres = vbe_mode.YResolution;
    bits_per_pixel = vbe_mode.BitsPerPixel;

    red_mask_size = vbe_mode.RedMaskSize;
    green_mask_size = vbe_mode.GreenMaskSize;
    blue_mask_size = vbe_mode.BlueMaskSize;

    red_field_position = vbe_mode.RedFieldPosition;
    green_field_position = vbe_mode.GreenFieldPosition;
    blue_field_position = vbe_mode.BlueFieldPosition;

    unsigned vram_size = hres * vres * (bits_per_pixel / 8);

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

int vggg_exit() {
    reg86_t t;
    memset(&t, 0, sizeof(t)); /* wipe the struct */
	t.intno = 0x10;
    t.ah = 0x00; /* Set Video Mode function */
	t.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&t) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}
    return OK;
}

int square_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int l = 0; l < height; l++) {
        printf("DRAW LINE!\n");
        if (vg_draw_hlineee(x , y+l, width, color) != OK)
            return 1;
    }
    return OK;
}

int vg_draw_hlineee(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len;i++) {
        if (vg_paint_pixelll(x+i,y,color) != OK)
            return 1;
    }
    return OK;
}

int vg_paint_pixelll(uint16_t x_coord, uint16_t y_coord, uint32_t color) {
    if (x_coord > hres || y_coord > vres) {
        return 1;
    }

    if (bits_per_pixel == 8) { //Indexed
        memset(video_mem + hres*y_coord + x_coord, color, 1);
    }
    else {
        memcpy(video_mem + hres*y_coord*(bits_per_pixel/8) + vres*x_coord*(bits_per_pixel/8), &color, (bits_per_pixel/8));
    }
    return OK;
}

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    if (vres % no_rectangles != 0) {
        vg_draw_hlineee(0,vres-1,hres,0xFF);
        return OK;
    }
    uint16_t no_lines = vres / no_rectangles;
    uint16_t no_columns = hres / no_rectangles;
    uint32_t color;
    printf("ARRIVED0\n");

    uint8_t r,g,b,firstR,firstG,firstB;

    if (mode == 0x105) { //Indexed mode
        printf("ARRIVED1\n");
        printf("%x\n",no_columns);
        printf("%x\n",no_lines);
        for (int col = 0; col < no_columns; col++) {
            printf("  a --");
            for (int row = 0; row < no_lines; row++) {
                printf("  b --");
                color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
                printf("CALLED FUNC!\n");
                if (square_draw(col*no_columns, row*no_lines,no_columns,no_lines,color) != OK) {
                    printf("ERROR!\n");
                    return 1;
                }
                    
            }
        }
    }
    else {
        firstR = first >> red_field_position;
        firstG = first >> green_field_position;
        firstB = first >> blue_field_position;
        for (int col = 0; col < no_columns; col++) {
            for (int row = 0; row < no_lines; row++) {
                r = (firstR + col * step) % (1 << red_mask_size);
                g = (firstG + row * step) % (1 << green_mask_size);
                b = (firstB + (col + row) * step) % (1 << blue_mask_size);
                color = (b << blue_mask_size) + (g << green_mask_size) + (r << red_field_position);
                if (square_draw(col*no_columns, row*no_lines,no_columns,no_lines,color) != OK)
                    return 1;
            }
        }
    }
    printf("ARRIVED3\n");
    return OK;
}
