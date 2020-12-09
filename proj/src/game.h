#include <lcom/lcf.h>

#include <Sprites/background.xpm>

#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "mole.h"

typedef enum {MAIN_MENU, GAME, GAME_OVER, EXIT} game_state;
typedef enum {TIMER, KEYBOARD} device;

typedef struct {
	xpm_image_t background;
	uint8_t timer_irq, keyboard_irq;
	Mole* moles[6];
	game_state game_state;

} WhacAMole;

WhacAMole* load_game();

int game_main_loop(WhacAMole* game);

void interrupt_handler(device device, WhacAMole* game);

