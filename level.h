#ifndef LEVEL_H
#define LEVEL_H
#include "game.h"
#include "gba.h"
#define GAME_WIDTH 200
#define EXTRA_LIFE_COL(i) (SHIP_WIDTH + SHIP_WIDTH * (i))
#define EXTRA_LIFE_ROW (HEIGHT - SHIP_WIDTH)
#define PLAYER_START_COL (GAME_WIDTH / 2)
#define PLAYER_START_ROW (HEIGHT - 3 * getHeight(player))
/* Defines what is going on in the level
typedef enum levelMode {
    NORMAL,
    SPECIAL
} LevelOperation; */
extern Ship *enemies[MAX_ENEMIES];
extern Ship *player;
extern int numEnemies;
void makeLevel(Game *game);
void deconstructLevel(void);
void drawLevel(void);
void drawSidePanel(Game *game);
#endif