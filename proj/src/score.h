#include <lcom/lcf.h>

#include <Sprites/leaderboard_background.xpm>
#include <Sprites/leaderboard_crown.xpm>
#include <Sprites/leaderboard_table.xpm>

#include <Sprites/score_numbers.xpm>
#include <Sprites/Buttons_img/normal/score_close_normal.xpm>
#include <Sprites/Buttons_img/active/score_close_active.xpm>

#include "Player.h"
#include "rtc.h"

typedef struct {
    char* player_name;
    int player_name_size;
    Date date;
    Time time;
    int score;
} Score_Record;

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


Leaderboard* load_leaderboard();

//Score_Record* load_score_record(Player* player, int score);

void draw_leaderboard(Leaderboard* leaderboard);

void draw_leaderboard_backgound(Leaderboard* leaderboard);

void draw_table__(Leaderboard* leaderboard);

void draw_player_names(xpm_image_t font, Score_Record* score_records, uint8_t num_records);

void draw_player_scores(Leaderboard* leaderboard);

void draw_player_score(xpm_image_t font, int xi, int yi, int score);


void save_scores(Leaderboard* leaderboard);

void load_scores(Leaderboard* leaderboard);

bool add_new_score(Leaderboard* leaderboard, Player* player);

void draw_player_name_ll(xpm_image_t font, int xi, int yi, char name[], int name_size);
