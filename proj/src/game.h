#include <lcom/lcf.h>

#include <Sprites/numbers.xpm>
#include <Sprites/moles_hitted.xpm>
#include <Sprites/moles_missed.xpm>
#include <Sprites/numbers_moles.xpm>
#include <Sprites/game_over.xpm>
#include <Sprites/game_over_missed_moles.xpm>
#include <Sprites/game_over_hitted_moles.xpm>
#include <Sprites/game_over_numbers.xpm>
#include <Sprites/clock_icon.xpm>
#include <Sprites/instructions.xpm>

#include <Sprites/font_small.xpm>

#include <Sprites/table.xpm>
#include <Sprites/good_bye_message.xpm>
#include <Sprites/good_bye_mole.xpm>
#include <Sprites/credits.xpm>

#include "xpm_coordinates.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "mole.h"
//#include "menu.h"
#include "kbd_manager.h"
#include "mouse.h"
#include "state_machine.h"
#include "rtc.h"
//#include "Player.h"
#include "score.h"


typedef enum {MAIN_MENU, PLAYER_SETTINGS, INSTRUCTIONS, SINGLE_PLAYER, MULTI_PLAYER, GAME_OVER, LEADERBOARD, EXIT} game_state;
typedef enum {TIMER, KEYBOARD, MOUSE, RTC} device;

typedef struct {
	xpm_image_t logo_game_over;
	xpm_image_t numbers;
	xpm_image_t missed_moles;
	xpm_image_t hitted_moles;
	Cursor *cursor;
	Button** buttons;
    int num_buttons;
} GameOver;

typedef struct {
	xpm_image_t logo;
	xpm_image_t mole;
	xpm_image_t credits;
	uint16_t x_mole;
} GoodBye;

typedef struct {
	xpm_image_t instructions;

} Instructions;


typedef struct {
	xpm_image_t game_time_numbers_font;
	xpm_image_t numbers_font;
	xpm_image_t letters_small_font;
	xpm_image_t clock_icon;
	xpm_image_t moles_missed;
	xpm_image_t moles_hitted;
	xpm_image_t table;

	uint8_t timer_irq, keyboard_irq;
	uint16_t mouse_irq, irq_rtc;

	uint8_t game_time;

	Mole* moles[6];
	Menu* menu;
	Player_Settings* player_settings;
	Player* player;
	Cursor* cursor;
	Instructions *instructions;
	GameOver* game_over;
	Leaderboard* leaderboard;
	GoodBye* good_bye;

	game_state game_state;
	bool running;
} WhacAMole;

WhacAMole* load_game();
GameOver* load_game_over();
GoodBye *load_good_bye();
Instructions *load_instructions();

int game_main_loop(WhacAMole* game);

void GeneralInterrupt(device device,WhacAMole* game);
void Main_Menu_interrupt_handler(device device, WhacAMole *new_game);
void Instructions_interrupt_handler(device device, WhacAMole *new_game);
void Player_Settings_interrupt_handler(device device, WhacAMole *new_game);
void Single_Player_interrupt_handler(device device, WhacAMole* game);
void Multi_Player_interrupt_handler(device device, WhacAMole* game);
void Game_Over_interrupt_handler(device device, WhacAMole* game);
void Leaderboard_interrupt_handler(device device, WhacAMole *new_game);
void Exit_interrupt_handler(device device, WhacAMole* new_game);

void show_timer(unsigned int timer_counter, WhacAMole *game);

void draw_all_moles(WhacAMole * new_game);

int check_over_mole(WhacAMole * new_game);


void draw_table(WhacAMole * new_game);
void draw_counter_moles(WhacAMole * new_game);
void draw_number(xpm_image_t font, int font_info, int xi, int yi, int number);

void draw_game_over(GameOver * game_over, Player *player);
void draw_game_over_missed_moles(GameOver *game_over, Player *Player);
void draw_game_over_hitted_moles(GameOver *game_over, Player *Player);
void draw_game_over_buttons(GameOver *game_over);

void draw_good_bye_message(GoodBye *good_bye);
void draw_good_bye_mole(GoodBye *good_bye);

void draw_instructions(Instructions *instructions);
