#include "maneuvers.h"
#include "level.h"
#include "game.h"
#include "ship.h"
#include "playState.h"
#include <stdlib.h>
int randomColumn(void);
int randomRow(void);
int playerColumnIsh(void);
int playerRowIsh(void);
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
void planRoute(Ship *ship)
{
    free(ship->route.path);
    ship->route.pathLength = 4;
    ship->route.path = (Cords *)calloc(sizeof(Cords), ship->route.pathLength);
    ship->route.currentStep = 0;
    srand(random);

    ship->route.path[0].col = randomColumn();
    ship->route.path[0].row = randomRow() % 50;
    ship->route.path[1].col = randomColumn();
    ship->route.path[1].row = randomRow()  % 50 + 50;
    ship->route.path[2].row = randomRow() % 50 + 100;//(HEIGHT - getHeight(ship)) / 2 + getHeight(ship) * 2;
    ship->route.path[2].col = playerColumnIsh();
        int left = (player->cords.col > (GAME_WIDTH - getWidth(player)) / 2) ? 1 : 0;
        if (left)
            ship->route.path[2].col += getWidth(player) * 2;
        else {
            ship->route.path[2].col -= getWidth(player) * 2;
        }
    ship->route.path[3].row = playerRowIsh();
    ship->route.path[3].col = playerColumnIsh();
    // Check bounds for path
    for (int i = 0; i < ship->route.pathLength; i++)
    {
        int *row = &ship->route.path[i].row;
        if (*row < 0)
            *row = getHeight(ship);
        else if(*row > HEIGHT) {
            *row = HEIGHT - getHeight(ship) - 1;
        }
        if (i > 0 && *row >= player->home.row && *row >= ship->route.path[i - 1].row) {
            srand(count / i);
            *row = rand() % player->home.row - getHeight(player) * 2;
        }
        int *col = &ship->route.path[i].col;
        if (*col < 0)
            *col = 0;
        else if (*col > GAME_WIDTH - getWidth(ship) - 1)
            *col = GAME_WIDTH - getWidth(ship) - 1;
        //ship->route.path[i] = player->cords;
        // if (ship->route.path[i].col < 0 || ship->route.path[i].col >= GAME_WIDTH - getWidth(ship))
        // {
        //     ship->route.path[i].col = randomColumn();
        // }
        // if (ship->route.path[i].row < 0 || ship->route.path[i].row >= HEIGHT - getHeight(ship))
        // {
        //     ship->route.path[i].row = randomRow(); //ship->home.row;
        // }
    }
}
int randomColumn(void)
{
    srand(random);
    int col;
    col = (rand() % (GAME_WIDTH - 2 * SHIP_WIDTH) + SHIP_WIDTH) / 2;
    while (col < 0 || col > GAME_WIDTH - SHIP_WIDTH * 2)
    col = (rand() % (GAME_WIDTH - 2 * SHIP_WIDTH) + SHIP_WIDTH) / 2;
    return col;
}
int playerColumnIsh(void) {
    int left = (player->cords.col > (GAME_WIDTH - getWidth(player)) / 2) ? 1 : 0;
    srand(count);
    int col = rand() % (getWidth(player) * 2);
    if (left)
        col *= -1;
    return col + player->cords.col;
}
int randomRow(void)
{
    srand(levelCounter);
    return rand() % (HEIGHT - 2 * SHIP_HEIGHT) + SHIP_HEIGHT;
}
int playerRowIsh(void) {
    srand(count);
    int row = rand() % getHeight(player);
    return row - getHeight(player) / 2 + player->cords.row;
}