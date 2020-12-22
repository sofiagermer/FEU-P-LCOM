#include <lcom/lcf.h>

#include <Sprites/hammer.xpm>

typedef struct  {
    xpm_image_t hammer;
    xpm_image_t score_numbers;
    int score;
    char[] name;
    int x_position,y_position;
} Player;

Player* createPlayer();

void draw_player(Player* player);