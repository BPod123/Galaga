#include "maneuvers.h"
#include "level.h"
#include "game.h"
#include "ship.h"
#include "playState.h"
#include <stdlib.h>
int randomColumn(int seed);
int randomRow(int seed);
int playerColumnIsh(int seed);
int playerRowIsh(int seed);
/**
 * Creates a random path for a ship to follow
 * @param ship a pointer to the ship
 * @param seed the random seed
 * */
void planRoute(Ship *ship, int seed)
{
    free(ship->route.path);
    ship->route.pathLength = 4;
    ship->route.path = (Cords *)calloc(sizeof(Cords), ship->route.pathLength);
    ship->route.currentStep = 0;
    srand(seed);

    ship->route.path[0].col = randomColumn(seed);
    ship->route.path[0].row = randomRow(seed) % 50;
    ship->route.path[1].col = randomColumn(seed);
    ship->route.path[1].row = randomRow(seed) % 50 + 50;
    ship->route.path[2].row = randomRow(seed) % 50 + 100; //(HEIGHT - getHeight(ship)) / 2 + getHeight(ship) * 2;
    ship->route.path[2].col = playerColumnIsh(seed);
    int left = (player->cords.col > (GAME_WIDTH - getWidth(player)) / 2) ? 1 : 0;
    if (left)
        ship->route.path[2].col += getWidth(player) * 2;
    else
    {
        ship->route.path[2].col -= getWidth(player) * 2;
    }
    ship->route.path[3].row = playerRowIsh(seed);
    ship->route.path[3].col = playerColumnIsh(seed);
    // Check bounds for path
    for (int i = 0; i < ship->route.pathLength; i++)
    {
        int *row = &ship->route.path[i].row;
        if (*row < 0)
            *row = getHeight(ship);
        else if (*row > HEIGHT)
        {
            *row = HEIGHT - getHeight(ship) - 1;
        }
        if (i > 0 && *row >= player->home.row && *row >= ship->route.path[i - 1].row)
        {
            srand(seed);
            *row = rand() % player->home.row - getHeight(player) * 2;
        }
        int *col = &ship->route.path[i].col;
        if (*col < 0)
            *col = 0;
        else if (*col > GAME_WIDTH - getWidth(ship) - 1)
            *col = GAME_WIDTH - getWidth(ship) - 1;
    }
}
/**
 * @param seed The random seed
 * @return a random col */
int randomColumn(int seed)
{
    srand(seed);
    int col;
    col = (rand() % (GAME_WIDTH - 2 * SHIP_WIDTH) + SHIP_WIDTH) / 2;
    while (col < 0 || col > GAME_WIDTH - SHIP_WIDTH * 2)
        col = (rand() % (GAME_WIDTH - 2 * SHIP_WIDTH) + SHIP_WIDTH) / 2;
    return col;
}
/** 
 * @param seed The random seed
 * @return a row near the player col */
int playerColumnIsh(int seed)
{
    int left = (player->cords.col > (GAME_WIDTH - getWidth(player)) / 2) ? 1 : 0;
    srand(seed);
    int col = rand() % (getWidth(player) * 2);
    if (left)
        col *= -1;
    return col + player->cords.col;
}
/**
 * @param seed The random seed
 * @return a random row */
int randomRow(int seed)
{
    srand(seed);
    return rand() % (HEIGHT - 2 * SHIP_HEIGHT) + SHIP_HEIGHT;
}
/** 
 * @param seed The random seed
 * @return a row near the player row */
int playerRowIsh(int seed)
{
    srand(seed);
    int row = rand() % getHeight(player);
    return row - getHeight(player) / 2 + player->cords.row;
}