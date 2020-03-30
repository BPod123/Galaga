#ifndef MANEUVERS_H
#define MANEUVERS_H
#include "game.h"
#define fOf(column, line) ((int)(((line).m * (float)(column)) + (line).b))
#define xOf(row, line) ((int) (((row) - (line).b) / (line).m))
typedef struct line {
    float m;
    float b;
} Line;

// Go up to center, then target player
void drawLine(Line line, int c1, int c2, u16 color);
Line getLine(int r1, int c1, int r2, int c2);
void planRoute(Ship *ship);
void executeInstruction(Ship *ship);
Direction getBestDirection(Cords c1, Cords c2);
Direction combineDirections(Direction upDown, Direction leftRight);
Direction getDirection(Cords *cords1, Cords *cords2, int isEnemy);
Cords getSeparation(Cords c1, Cords c2);
void moveCords(Cords *cords, Direction direction);
int hasMoreInstructions(Ship *ship);
Direction *getAdjacentDirections(Direction direction);
#endif