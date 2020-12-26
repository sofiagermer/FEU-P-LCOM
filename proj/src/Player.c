#include <lcom/lcf.h>
#include <stdint.h>

#include "Player.h"
#include "vd_card.h"

Player* createPlayer(string name) {
    //Allocating memory
    Player *player = (Player *)malloc(sizeof(Player));

    //Setting up mole variables
    player->x_position = vg_get_hres()/2;
    player->y_position = vg_get_vres/2;
    player->score = 0;
    player->name[] = {'P', 'L','A', 'Y','E', 'R'};
}

void draw_player(Player* player) {
    uint32_t *pixmap = (uint32_t *)player->hammer.bytes;
    // copy it to graphics memory
    vg_draw_xpm(pixmap, player->hammer, player->x_position, player->y_position);
}

void draw_score(Player* player) {
    uint32_t *pixmap = (uint32_t *)player->hammer.bytes;
    // copy it to graphics memory
    vg_draw_xpm(pixmap, player->hammer, player->x_position, player->y_position);
}