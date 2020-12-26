#include <lcom/lcf.h>

#include <Sprites/numbers.xpm>
#include <Sprites/moles_hitted.xpm>
#include <Sprites/moles_missed.xpm>
#include <Sprites/numbers_moles.xpm>
#include <Sprites/table.xpm>
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
	xpm_image_t numbers_mole;
	xpm_image_t moles_missed;
	xpm_image_t moles_hitted;
	xpm_image_t table;
	uint8_t timer_irq, keyboard_irq;
	uint16_t mouse_irq;
	Mole* moles[6];
	Menu* menu;
	Cursor* cursor;
	game_state game_state;

	//PARA O PLAYER
	int num_moles_hitted;
	int num_moles_missed;

} WhacAMole;

WhacAMole* load_game();

int game_main_loop(WhacAMole* game);

void GeneralInterrupt(device device,WhacAMole* game);
void Main_Menu_interrupt_handler(device device, WhacAMole *new_game);
void Single_Player_interrupt_handler(device device, WhacAMole* game);
void Multi_Player_interrupt_handler(device device, WhacAMole* game);
//void Game_Over_interrupt_handler(device device, WhacAMole* game);

void show_timer(unsigned int timer_counter, WhacAMole *game);

void draw_numbers(WhacAMole *new_game);

void draw_all_moles(WhacAMole * new_game);

int check_over_mole(WhacAMole * new_game);
void hit_mole(WhacAMole * new_game, int mole_index);

void draw_table(WhacAMole * new_game);
void draw_counter_moles(WhacAMole * new_game);
void draw_number_moles_missed(WhacAMole * new_game);
void draw_number_moles_hitted(WhacAMole * new_game);

