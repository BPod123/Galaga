#ifndef MANEUVERS_H
#define MANEUVERS_H
#include "game.h"
#define fOf(column, line) ((int)(((line).m * (float)(column)) + (line).b))
#define xOf(row, line) ((int) (((row) - (line).b) / (line).m))
typedef struct line {
    float m;
    float b;
} Line;
void flip(Ship *ship, int ccw, int index);
// Go up to center, then target player
void waterFallDown(Ship *ship, int index);
void drawLine(Line line, int c1, int c2, u16 color);
Line getLine(int r1, int c1, int r2, int c2);
void planRoute(Ship *ship);

#endif