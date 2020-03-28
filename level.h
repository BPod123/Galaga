#ifndef LEVEL_H
#define LEVEL_H
#define GAME_WIDTH 200
#include "game.h"
#include "gba.h"

extern Ship *enemies[MAX_ENEMIES];
extern Ship *player;

void makeLevel(Game *game);
void drawLevel(void);
#endif