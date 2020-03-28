#ifndef SHIP_H
#define SHIP_H
#include "game.h"
#include "level.h"
#define SHIP_WIDTH PLAYERSHIP_UP_WIDTH
#define SHIP_HEIGHT PLAYERSHIP_UP_HEIGHT
const u16 *getImage(Ship *ship, Direction direction);
int getWidth(Ship *ship);
int getHeight(Ship *ship);
ShipType getRandomEnemy(void);
#endif