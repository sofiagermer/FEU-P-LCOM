#pragma once
#include <lcom/lcf.h>

#include <Sprites/leaderboard_background.xpm>
#include <Sprites/leaderboard_crown.xpm>
#include <Sprites/leaderboard_table.xpm>

#include <Sprites/score_numbers.xpm>
#include <Sprites/Buttons_img/normal/score_close_normal.xpm>
#include <Sprites/Buttons_img/active/score_close_active.xpm>

#include "Player.h"
#include "rtc.h"

/**
 * @struct Score_Record
 * @var: player_name
 * Player's name
 * @var: player_name_size
 * Player name's size
 * @var: date
 * //E
 * @var: time
 * //E
 * @var: score
 * Player's score
 * */
typedef struct {
    char* player_name;
    int player_name_size;
    Date date;
    Time time;
    int score;
} Score_Record;

/**
 * @struct Leaderboard
 * @var: background
 * Leaderboard's background xpm
 * @var: crown
 * Leaderboard's crown xpm
 * @var: table
 * Leaderboard's crown xpm
 * @var: numbers
 * Leaderboard's numbers xpm
 * @var: buttons
 * Leaderboard's buttons
 * @var: num_buttons
 * Number of Buttons of LeaderBoard
 * @var: score_records
 * Leaderboard's Score_Record
 * @var: num_score_records
 * Leaderboard's num of score records 
 * @var: max_name_length
 * Leaderboard's max name lenght
 * */
typedef struct  {
    xpm_image_t background;
    xpm_image_t crown;
    xpm_image_t table;
    xpm_image_t numbers;
    Button** buttons;
    int num_buttons;
    Score_Record* score_records;
    int num_score_records;
    int max_name_length;
} Leaderboard;

/**
 * @brief: loads leaderboard and sets it's variables
 * @return: leaderboard
 * */
Leaderboard* load_leaderboard();

//Score_Record* load_score_record(Player* player, int score);

/**
 *  @brief draws leaderboard
 *  @param leaderboard
 * */
void draw_leaderboard(Leaderboard* leaderboard);

/**
 *  @brief draws leaderboard's background
 *  @param leaderboard
 * */
void draw_leaderboard_backgound(Leaderboard* leaderboard);

/**
 *  @brief draws leaderboard's table
 *  @param leaderboard
 * */
void draw_table__(Leaderboard* leaderboard);

/**
 *  @brief draws leaderboard's player names
 *  @param font font that will be used to draw the names
 *  @param score_records
 *  @param num_records  
 * */
void draw_player_names(xpm_image_t font, Score_Record* score_records, uint8_t num_records);

/**
 *  @brief draws players's scores
 *  @param leaderboard
 * */
void draw_player_scores(Leaderboard* leaderboard);

/**
 *  @brief draws players's dates
 *  @param leaderboard
 * */
void draw_player_dates(Leaderboard *leaderboard);

/**
 *  @brief draws players's score
 *  @param font font that will be used to draw the score
 *  @param xi 
 *  @param yi
 *  @param score
 * 
 * */
void draw_player_score(xpm_image_t font, int xi, int yi, int score);

/**
 * @brief: E
 * @param leaderboard
 * */
void save_scores(Leaderboard* leaderboard);

/**
 * @brief: E
 * @param leaderboard
 * */
void load_scores(Leaderboard* leaderboard);

/**
 * @brief: E
 * @param leaderboard
 * @param player
 * */
bool add_new_score(Leaderboard* leaderboard, Player* player);

/**
 * @brief : E
 * @param xi
 * @param yi
 * @param name[]
 * @param name_size
 * */
void draw_player_name_ll(xpm_image_t font, int xi, int yi, char name[], int name_size);
