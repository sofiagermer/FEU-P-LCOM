#pragma once
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

/** 
 * @struct GameOver
 * @brief Struct relative to GameOver
 * @var::logo_game_over
 * Game Over's logo xpm
 * @var:: numbers
 * Game Over's number xpm
 * @var:: missed_moles
 * Game Over's  missed_moles xpm
 * @var:: hitted_moles
 * Game Over's hitted_moles xpm
 * @var:: cursor
 * Game Over's cursor
 * @var:: buttons
 * Game Over's buttons
 * @var:: num_buttons
 * how many buttons Game Over has
 */
typedef struct {
	xpm_image_t logo_game_over;
	xpm_image_t numbers;
	xpm_image_t missed_moles;
	xpm_image_t hitted_moles;
	Cursor *cursor;
	Button** buttons;
    int num_buttons;
} GameOver;

/**
 * @struct GoodBye
 * @bried Struct realtive to GoodBye
 * @var:: logo
 * GoodBye's logo xpm
 * @var::mole
 * GoodBye's mole xpm
 * @var:: credits
 * GoodBye's credits xpm 
 * @var:: x_mole
 * GoodBye's mole's initial x axis position
 * 
 * 
 * */
typedef struct {
	xpm_image_t logo;
	xpm_image_t mole;
	xpm_image_t credits;
	uint16_t x_mole;
} GoodBye;

/**
 * @struct Instructions
 * @var::instructions
 * Instruction's xpm
 * */
typedef struct {
	xpm_image_t instructions;

} Instructions;

/**
 * @struct WhackAMole
 * @var: numbers_font
 * WhackAMole's numbers font xpm
 * @var: letters_small_font
 * WhackAMole's  small letters font xpm
 * @var: clock_icon
 * WhackAMole's clock icon xpm
 * @var: moles_missed
 * WhackAMole's moles missed xpm
 * @var: moles_hitted
 * WhackAMole's moles hitted xpm
 * @var: table
 * WhackAMole's table where missed moles and hitted moles will be displayed xpm
 * @var: timer_irq
 * timer IRQ 
 * @var: keyboard_irq
 * keyboard IRQ
 * @var: mouse_irq
 * mouse IRQ
 * @var: irq_rtc
 * rtc IRQ
 * @var: game_time
 * WhackAMole's game_time
 * @var: moles[6]
 * WhackAMole's moles
 * @var: menu
 * WhackAMole's menu
 * @var: player_settings
 * WhackAMole's Player Settings
 * @var: player
 * WhackAMole's Player
 * @var: cursor
 * WhackAMole's Cursor
 * @var: instructions
 * WhackAMole's Instructions
 * @var: game_over
 * WhackAMole's Game Over
 * @var: leaderboard
 * WhackAMole's Leaderboard
 * @var: good_bye
 * WhackAMole's Good Bye
 * @var: game_state
 * WhackAMole's game state
 * @bool: running
 * Boolean : it's true while the user has not yet chosen to exit the game, else it's false 
 * 
 * */

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

/**
 * @brief loads Game : Loads all Game's sprites,  Creates  Mole, Menu, Player_Settings, Player,Cursor, Instructions, Leaderboard, GoodBye, initializates the game at MAIN_MENU and set's bool running as true
 * 
 * */
WhacAMole* load_game();
/**
 * @brief loads Game over: loads all it's xpm, creates it's Curson and all it's Buttons 
 * */
GameOver* load_game_over();
/**
 * @brief loads Good Bye : loads all it's sprites and sets it's moles first position
 * */
GoodBye *load_good_bye();
/**
 * @brief loads Instructions : loads it's sprite 
 * */
Instructions *load_instructions();


int game_main_loop(WhacAMole* game);
/**
 * @brief General Interrupt Control for any device
 * @param device device Device that generated interrupts
void GeneralInterrupt(device device,WhacAMole* game);

/**
 *  @brief Main Menu's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Main_Menu_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief  Instruction's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Instructions_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief  Player_Setting's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Player_Settings_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief  Single_Player's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Single_Player_interrupt_handler(device device, WhacAMole* game);
/**
 *  @brief  Multi_Player's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Multi_Player_interrupt_handler(device device, WhacAMole* game);
/**
 *  @brief  Game Over's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Game_Over_interrupt_handler(device device, WhacAMole* game);
/**
 *  @brief  Leaderboard's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Leaderboard_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief Exit's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Exit_interrupt_handler(device device, WhacAMole* new_game);

void show_timer(unsigned int timer_counter, WhacAMole *game);

/**
 * @brief draws all moles at their current position
 * @param WhackAMole *new_game
 * */
void draw_all_moles(WhacAMole * new_game);

/**
 * @brief checks if cursor its over mole
 * @param WhackAMole * new_game
 * @return int mole index, returns the index of the mole where the cursor is at, or 7 if the cursor isn't over any mole
 * */
int check_over_mole(WhacAMole * new_game);


/**
 *  @brief Draw "table" where the missed moles and hitted moles will be displayed during the game
 *  @param WhackAMole *new_game actual game
 * */
void draw_table(WhacAMole * new_game);
/**
 *  @brief Draw "missed moles" and "hitted moles" xpm inside it's table
 *  @param WhackAMole *new_game actual game
 * */
void draw_counter_moles(WhacAMole * new_game);
/**
 *  @brief Draw a number at a given position through a given font 
 *  @param xpm_image_t font xpm used to draw the number
 *  @param int font_info
 *  @param int xi upper left xpm's x position
 *  @param int yi upper left xpm's y position
 * */

void draw_number(xpm_image_t font, int font_info, int xi, int yi, int number);

/**
 * @brief draw game_over module: it's logo, the number of missed moles and the number of hitted moles
 * @param GameOver * game_over
 * @param Player *player
 * */
void draw_game_over(GameOver * game_over, Player *player);

//?? estas vão deixar de ser usadas
void draw_game_over_missed_moles(GameOver *game_over, Player *Player);
void draw_game_over_hitted_moles(GameOver *game_over, Player *Player);

/**
 * @brief draws Game Over's buttons
 * @param GameOver *game_over
 * */
void draw_game_over_buttons(GameOver *game_over);

/**
 * @brief draws "Good Bye" when user chooses to leave game
 * @param GoodBye *good_bye
 * */
void draw_good_bye_message(GoodBye *good_bye);
/**
 * @brief draws the goodbye's module mole at it's current position (the mole will me moved from left to right during GoodBye's time)
 * @param GoodBye *good_bye
 * */
void draw_good_bye_mole(GoodBye *good_bye);

/**
 * @brief draws Game's Instructions
 * @param Instructions *instructions
 * */
void draw_instructions(Instructions *instructions);
