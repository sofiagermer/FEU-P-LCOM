
#include <lcom/lcf.h>
#include <Sprites/background.xpm>
#include <Sprites/cursor.xpm>
#include <Sprites/exit_bright.xpm>
#include <Sprites/exit.xpm>
#include <Sprites/logo.xpm>
#include <Sprites/multiplayer_bright.xpm>
#include <Sprites/multiplayer.xpm>
#include <Sprites/singleplayer_bright.xpm>
#include <Sprites/singleplayer.xpm>


#include <vd_card.h>
#include <mouse.h>
#include <state_machine.h>

/**
 *  @brief Struct button_t. Acts as class BUTTON.
 *  A button has two xpms: a default and a bright xpm.
 *  The normal xpm is the xpm that we are choosing, when one button is in it's normal state the others are bright
 *  A button also holds information about the coordinates from where it starts and to where it ends.
**/
/* 
typedef enum_click{
    single_player_click,
    multiplayer_click,
    exit_click,
    no_click = -1
}click_t; */

typedef struct {
    xpm_image_t normal;
    xpm_image_t bright;
    bool bright_;
    uint16_t xi;
    uint16_t yi;
    uint16_t xf;
    uint16_t yf;
} Button;

typedef struct{
    xpm_image_t cursor_image;
    uint16_t x;
    uint16_t y;
} Cursor;

typedef struct {
    xpm_image_t logo;         
    Button *single_player_button;  
    Button *multi_player_button; 
    Button *exit_button;       
    Cursor *cursor;              
} Menu;

void load_background();


Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright);
Menu *load_menu();
Cursor *load_cursor(xpm_row_t *img_cursor);

void draw_cursor(Cursor *cursor);
void draw_background();
void draw_logo(Menu *menu);
void draw_buttons(Menu *menu);
void draw_menu(Menu *menu);

int mouse_over(Button *button, Cursor *cursor);
void move_cursor(struct packet *packet, Menu *menu);


