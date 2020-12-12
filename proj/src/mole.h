#include <lcom/lcf.h>

#include <Sprites/hole.xpm>

#include <Sprites/mole_up1.xpm>
#include <Sprites/mole_up2.xpm>
#include <Sprites/mole_up3.xpm>
#include <Sprites/mole_up4.xpm>

/* #include <Sprites/mole_down_miss1.xpm>
#include <Sprites/mole_down_miss2.xpm>
#include <Sprites/mole_down_miss3.xpm>
#include <Sprites/mole_down_miss4.xpm>

#include <Sprites/mole_down_hit1.xpm>
#include <Sprites/mole_down_hit2.xpm>
#include <Sprites/mole_down_hit3.xpm>
#include <Sprites/mole_down_hit4.xpm> */

#define MOLE_HEIGHT  150
#define MOLE_WIDHT   200
#define STEP_FROM_X  180
#define STEP_FROM_Y  180
#define MOLE_UP_TIME 5

#define KBD_KEY_0 'A'
#define KBD_KEY_1 'S'
#define KBD_KEY_2 'D'
#define KBD_KEY_3 'J'
#define KBD_KEY_4 'K'
#define KBD_KEY_5 'L'

typedef enum {HIDED, UP_1, UP_2, UP_3, UP_4,DOWN_4, DOWN_3, DOWN_2, DOWN_1} Position;

typedef struct  {
    xpm_image_t atual_img;
    xpm_image_t hided;
    xpm_image_t up[4];
    xpm_image_t down_miss[4];
    xpm_image_t down_hit[4];
    
    int index;
    char kbd_key;
    int x,y;
    int time_up;
    Position position;
} Mole;

Mole* createMole();

void draw_mole(Mole* mole);
