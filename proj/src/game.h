#include <lcom/lcf.h>

#include <Sprites/numbers.xpm>

#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "mole.h"
#include "menu.h"
#include "kbd_manager.h"
#include "mouse.h"
#include "state_machine.h"

typedef enum {MAIN_MENU, SINGLE_PLAYER, MULTI_PLAYER, GAME_OVER, EXIT} game_state;
typedef enum {TIMER, KEYBOARD, MOUSE} device;

typedef struct {
	xpm_image_t numbers;
	uint8_t timer_irq, keyboard_irq;
	uint16_t mouse_irq;
	Mole* moles[6];
	Menu* menu;
	game_state game_state;

} WhacAMole;

WhacAMole* load_game();

int game_main_loop(WhacAMole* game);

void GeneralInterrupt(device device,WhacAMole* game);
void Main_Menu_interrupt_handler(device device, WhacAMole *new_game);
void Single_Player_interrupt_handler(device device, WhacAMole* game);
void Multi_Player_interrupt_handler(device device, WhacAMole* game);

void show_timer(unsigned int timer_counter, WhacAMole *game);

void draw_numbers(WhacAMole *new_game);

void draw_all_moles(WhacAMole * new_game);
