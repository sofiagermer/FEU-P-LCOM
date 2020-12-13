
#include "menu.h"
#include <lcom/lcf.h>

/**
 *  @brief Struct button_t. Acts as class BUTTON.
 *  A button has two xpms: a default and a bright xpm.
 *  The normal xpm is the xpm that we are choosing, when one button is in it's normal state the others are bright
 *  A button also holds information about the coordinates from where it starts and to where it ends.
**/

typedef enum_click{
    single_player_click,
    multiplayer_click,
    exit_click,
    no_click = -1
}click_t;

typedef struct button{
    xpm_image_t normal;
    xpm_image_t bright;
    bool bright;
    uint16_t xi;
    uint16_t yi;
    //não tenho a certeza se preciso da posição do canto inferior esquerdo
    uint16_t xf;
    uint16_t yf;
} button_t;

typedef struct menu {
    xpm_image_t logo;           
    button_t *single_player_button;  
    button_t *multi_player_button; 
    button_t *exit_button;                     
} menu_t;

