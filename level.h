#ifndef LEVEL_H
#define LEVEL_H
#include "game.h"
#include "gba.h"
#define GAME_WIDTH 200
#define EXTRA_LIFE_COL(i) (SHIP_WIDTH + SHIP_WIDTH * (i))
#define EXTRA_LIFE_ROW (HEIGHT - SHIP_WIDTH)
#define PLAYER_START_COL (GAME_WIDTH / 2)
#define PLAYER_START_ROW (HEIGHT - 3 * getHeight(player))
#define VALID_CORDS(cordsPtr) ((cordsPtr->col >= 0 && cordsPtr->col < WIDTH) && (cordsPtr->row >=0 && cordsPtr->row<HEIGHT))
#define VALID_GAME_CORDS(cordsPtr) ((VALID_CORDS(cordsPtr)) && (cordsPtr->col < GAME_WIDTH))

/* Defines what is going on in the level
typedef enum levelMode {
    NORMAL,
    SPECIAL
} LevelOperation; */
extern Ship **enemies;
extern Ship *player;
extern Ship *missiles[MAX_MISSILES];
extern int numEnemies;
extern int numAttackers;
extern int floatRadiusX;
extern int levelCounter;
void makeLevel(void);
void deconstructLevel(void);
void drawLevel(void);
void drawSidePanel(void);
Direction getRelativeDirection(Cords cords1, Cords cords2);
Cords getSeparation(Cords c1, Cords c2);
void moveCords(Cords *cords, Direction direction);
char *intToString(int num, char *buffer);
void levelOne(void);
void levelTwo(void);
void levelThree(void);
void levelFour(void);
void levelFive(void);
void levelSix(void);
#endif