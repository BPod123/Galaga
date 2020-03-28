#ifndef SHIP_H
#define SHIP_H
#include "game.h"
#include "level.h"
ShipType getRandomEnemy(void);
extern const int SHIP_WIDTH;
extern const int SHIP_HEIGHT;
void drawShip(Ship *ship, Direction direction);
int hasCollided(Ship *s1, Ship *s2);
void eraseShip(Ship *ship);
int getWidth(Ship *ship);
int getHeight(Ship *ship);
const u16 *getImage(Ship *ship, Direction direction);
#endif