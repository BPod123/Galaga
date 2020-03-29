// #include "testing.h"
#include "game.h"
#include "gba.h"
#include "level.h"
#include "ship.h"
#include "playState.h"
#include "images/titleScreen.h"
#include "level.h"
#include <stdlib.h>
#include "images/missile_Up.h"
//#include "util.h"
Ship *ship;
Ship *testShip;
void placeShipInCenter(Ship *ship);
void proccessInput(void);
void flyShip(void);
//void displayShipData(Ship *ship, int row, int col);
void runTest(void)
{
    Game *game = malloc(sizeof(Game));
    game->level = 1;
    game->lives = 3;
    makeLevel(game);
    waitForVBlank();
    drawLevel();
    u32 curr = BUTTONS;
    u32 prev = BUTTONS;
    while (1)
    {
        curr = BUTTONS;
        waitForVBlank();
        handlePlayerInput(curr, prev);
        for (int i = 0; i < MAX_MISSILES; i++)
        {
            if (missiles[i]->isActive)
            {
                executeRoute(missiles[i]);
            }
        }
        delay(DELAY_TIME);
        prev = curr;
    }
}

void testPlacingMissileAboveShip(void)
{
    Game *game = malloc(sizeof(Game));
    game->level = 1;
    game->lives = 3;
    makeLevel(game);
    waitForVBlank();
    drawLevel();
    u32 curr = BUTTONS;
    u32 prev = BUTTONS;
    while (1)
    {
        curr = BUTTONS;
        waitForVBlank();
        if (KEY_JUST_PRESSED(BUTTON_UP, curr, prev))
        {
            drawImageDMA(player->cords.row - MISSILE_UP_HEIGHT - 1, player->cords.col + getWidth(player) / 2 - MISSILE_UP_WIDTH / 2, MISSILE_UP_WIDTH, MISSILE_UP_HEIGHT, missile_Up);
        }

        handlePlayerInput(curr, prev);
        delay(DELAY_TIME);
        prev = curr;
    }
}
void flyShip(void)
{
    Game *game = malloc(sizeof(Game));
    game->level = 1;
    game->lives = 3;
    makeLevel(game);
    testShip = player;
    ship = enemies[0];
    ship->isActive = 1;
    Cords **path = ship->route.path;
    ship->route.pathLength = 5;
    for (int i = 0; i < ship->route.pathLength; i++)
    {
        switch (i)
        {
        case 0:
            path[i]->col = WIDTH - SHIP_WIDTH;
            path[i]->row = (HEIGHT - SHIP_HEIGHT) / 2;
            break;
        case 1:
            path[i]->col = (WIDTH - SHIP_WIDTH) / 2;
            path[i]->row = 0;
            break;
        case 2:
            path[i]->col = 0;
            path[i]->row = (HEIGHT - SHIP_HEIGHT) / 2;
            break;
        case 3:
            path[i]->col = (WIDTH - SHIP_WIDTH) / 2;
            path[i]->row = HEIGHT - SHIP_HEIGHT;
            break;
        }
    }
    path[ship->route.pathLength - 1]->col = (WIDTH - SHIP_WIDTH) / 2;
    path[ship->route.pathLength - 1]->row = (HEIGHT - SHIP_HEIGHT) / 2;

    //drawShip(testShip, UP);
    //placeShipInCenter(ship);
    u32 prevButtons = BUTTONS;
    u32 currButtons = BUTTONS;
    while (1)
    {
        currButtons = BUTTONS;
        if (ship->route.currentStep >= ship->route.pathLength)
            ship->route.currentStep = 0;
        if (getRelativeDirection(&ship->cords, path[ship->route.currentStep]) == NEUTRAL)
        {
            ship->route.currentStep++;
        }
        waitForVBlank();
        //proccessInput();
        handlePlayerInput(currButtons, prevButtons);
        //drawShip(player, player->direction);
        executeRoute(ship);
        delay(DELAY_TIME);
        prevButtons = currButtons;
    }
}
void proccessInput(void)
{
    if (KEY_DOWN_NOW(BUTTON_UP))
    {
        if (KEY_DOWN_NOW(BUTTON_LEFT))
        {
            move(testShip, UL);
        }
        else if (KEY_DOWN_NOW(BUTTON_RIGHT))
        {
            move(testShip, UR);
        }
        else
        {
            move(testShip, UP);
        }
    }
    else if (KEY_DOWN_NOW(BUTTON_DOWN))
    {
        if (KEY_DOWN_NOW(BUTTON_LEFT))
        {
            move(testShip, DL);
        }
        else if (KEY_DOWN_NOW(BUTTON_RIGHT))
        {
            move(testShip, DR);
        }
        else
        {
            move(testShip, DOWN);
        }
    }
    else if (KEY_DOWN_NOW(BUTTON_LEFT))
        move(testShip, LEFT);
    else if (KEY_DOWN_NOW(BUTTON_RIGHT))
        move(testShip, RIGHT);
}
/** Places a ship in the center of the screen */
void placeShipInCenter(Ship *ship)
{
    ship->cords.col = (WIDTH - SHIP_WIDTH) / 2;
    ship->cords.row = (HEIGHT - SHIP_HEIGHT) / 2;
    drawShip(ship, UP);
}
