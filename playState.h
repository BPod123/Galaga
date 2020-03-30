#ifndef PLAY_STATE_H
#define PLAY_STATE_H
#include "game.h"
#include "gba.h"
void runPlayState(u32 currentButtons, u32 previousButtons);
// ! Delete these when done testing
void executeRoute(Ship *ship);
void enemyMovements(void);
void move(Ship *ship, Direction direction);
void handlePlayerInput(u32 currentButtons, u32 previousButtons);
void handleCollisions(void);
void takeExtraLife(int life);
void showKillPlayer(void);
#endif