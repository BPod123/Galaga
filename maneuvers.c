#include "maneuvers.h"
#include "level.h"
#include "game.h"
#include "ship.h"
#include "playState.h"
void drawLine(Line line, int c1, int c2, u16 color)
{
    //waitForVBlank();
    for (int c = c1; c < c2; c++)
    {
        int row = (int)(line.m * c + line.b);
        setPixel(row, c, color);
    }
}
Line getLine(int r1, int c1, int r2, int c2)
{
    Line line;
    if (r2 == r1)
    {
        line.m = 0;
        line.b = r1;
        return line;
    }
    line.m = ((float)(r2 - r1)) / ((float)(c2 - c1));
    line.b = r1 - fOf(c1, line);
    return line;
}
void planRoute(Ship *ship) {
    Cords **path = ship->route.path;
    path[0]->col = GAME_WIDTH / 2;
    path[0]->row = SHIP_HEIGHT + 4;
    path[1]->col = player->cords.col;
    path[1]->row = HEIGHT - SHIP_HEIGHT;
    path[2]->col = (GAME_WIDTH - SHIP_WIDTH) / 2;
    path[2]->row = (HEIGHT - SHIP_HEIGHT) / 2;
    ship->route.pathLength = 3;
    ship->route.currentStep = 0;
}