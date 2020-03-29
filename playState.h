#ifndef PLAY_STATE_H
#define PLAY_STATE_H
#include "game.h"
#include "gba.h"
void runPlayState(Game *game, u32 currentButtons, u32 previousButtons);
// ! Delete these when done testing
void executeRoute(Ship *ship);
void move(Ship *ship, Direction direction);
void handlePlayerInput(u32 currentButtons, u32 previousButtons);
void fireMissile(void);
#endif