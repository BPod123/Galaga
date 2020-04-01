#include "ship.h"
#include <stdio.h>
#include <stdlib.h>
#include "images/playerShip_Up.h"
#include "images/blueAnt_Up.h"
#include "images/blueAnt_Down.h"
#include "images/blueAnt_Left.h"
#include "images/blueAnt_Right.h"
#include "images/blueAnt_UL.h"
#include "images/blueAnt_UR.h"
#include "images/blueAnt_DL.h"
#include "images/blueAnt_DR.h"
#include "images/redAnt_Up.h"
#include "images/redAnt_Down.h"
#include "images/redAnt_Left.h"
#include "images/redAnt_Right.h"
#include "images/redAnt_UL.h"
#include "images/redAnt_UR.h"
#include "images/redAnt_DL.h"
#include "images/redAnt_DR.h"
#include "images/redFly_Up.h"
#include "images/redFly_Down.h"
#include "images/redFly_Left.h"
#include "images/redFly_Right.h"
#include "images/redFly_UL.h"
#include "images/redFly_UR.h"
#include "images/redFly_DL.h"
#include "images/redFly_DR.h"
#include "images/blueFly_Up.h"
#include "images/blueFly_Down.h"
#include "images/blueFly_Left.h"
#include "images/blueFly_Right.h"
#include "images/blueFly_UL.h"
#include "images/blueFly_UR.h"
#include "images/blueFly_DL.h"
#include "images/blueFly_DR.h"
#include "images/missile_Up.h"
#include "images/levelBackground.h"
const int SHIP_WIDTH = PLAYERSHIP_UP_WIDTH;
const int SHIP_HEIGHT = PLAYERSHIP_UP_HEIGHT;
ShipType getRandomEnemy(void)
{
    int num = rand() % 4;
    switch (num)
    {
    case 0:
        return BLUE_ANT;
    case 1:
        return BLUE_FLY;
    case 2:
        return RED_ANT;
    default:
        return RED_FLY;
    }
}

void drawShip(Ship *ship, Direction direction)
{
    drawImageDMA(ship->cords.row, ship->cords.col, getWidth(ship), getHeight(ship), getImage(ship, direction));
}
/**
 * @param s1 Ship 1
 * @param s2 Ship 2
 * @return 1 if they have overlaping pixels, 0 if not */
int hasCollided(Ship *s1, Ship *s2)
{
    Cords *s1Cords = &s1->cords;
    Cords *s2Cords = &s2->cords;
    if (s1Cords->col > s2Cords->col + getWidth(s2) || s1Cords->col + getWidth(s1) < s2Cords->col)
        return 0;

    if (s1Cords->row > s2Cords->row + getHeight(s2) || s1Cords->row + getHeight(s1) < s2Cords->row)
        return 0;
    return 1;
    // return overlap(s1Cords->col, s1Cords->col + getWidth(s1), s2Cords->col, s2Cords->col + getWidth(s2)) ||
    //        overlap(s1Cords->row, s1Cords->row + getHeight(s1), s2Cords->row, s2Cords->row + getHeight(s2));
}

/** Replaces a ship with the background on the screen 
 * @param ship The ship to be erased
*/
void eraseShip(Ship *ship)
{
    for (int r = 0; r < getHeight(ship); r++)
    {
        DMA[3].src = &levelBackground[OFFSET(ship->cords.row + r, ship->cords.col, LEVELBACKGROUND_WIDTH)];
        DMA[3].dst = &videoBuffer[OFFSET(ship->cords.row + r, ship->cords.col, WIDTH)];
        DMA[3].cnt = SHIP_WIDTH | DMA_ON | DMA_SOURCE_INCREMENT;
    }
}
int getWidth(Ship *ship)
{
    if (ship->shipType != MISSILE)
        return SHIP_WIDTH;
    return MISSILE_UP_WIDTH;
}
int getHeight(Ship *ship)
{
    if (ship->shipType != MISSILE)
        return SHIP_HEIGHT;
    return MISSILE_UP_HEIGHT;
}
/** Returns the image array for a ship facing the passed in direction
 * @param ship The ship whose image is being retrieved
 * @param direction The direction the ship will face in the image
 * @return The image of ship facing direction */
const u16 *getImage(Ship *ship, Direction direction)
{
    switch (ship->shipType)
    {
    case PLAYER:
        return playerShip_Up;
    case BLUE_ANT:

        switch (direction)
        {

        case UP:
            return blueAnt_Up;

        case DOWN:
            return blueAnt_Down;

        case LEFT:
            return blueAnt_Left;

        case RIGHT:
            return blueAnt_Right;

        case UL:
            return blueAnt_UL;

        case UR:
            return blueAnt_UR;

        case DL:
            return blueAnt_DL;

        default:
            return blueAnt_DR;
        }
    case RED_ANT:

        switch (direction)
        {

        case UP:
            return redAnt_Up;

        case DOWN:
            return redAnt_Down;

        case LEFT:
            return redAnt_Left;

        case RIGHT:
            return redAnt_Right;

        case UL:
            return redAnt_UL;

        case UR:
            return redAnt_UR;

        case DL:
            return redAnt_DL;

        default:
            return redAnt_DR;
        }
    case RED_FLY:

        switch (direction)
        {

        case UP:
            return redFly_Up;

        case DOWN:
            return redFly_Down;

        case LEFT:
            return redFly_Left;

        case RIGHT:
            return redFly_Right;

        case UL:
            return redFly_UL;

        case UR:
            return redFly_UR;

        case DL:
            return redFly_DL;

        default:
            return redFly_DR;
        }
    case BLUE_FLY:

        switch (direction)
        {

        case UP:
            return blueFly_Up;

        case DOWN:
            return blueFly_Down;

        case LEFT:
            return blueFly_Left;

        case RIGHT:
            return blueFly_Right;

        case UL:
            return blueFly_UL;

        case UR:
            return blueFly_UR;

        case DL:
            return blueFly_DL;

        default:
            return blueFly_DR;
        }
    default: // MISSILE
        return missile_Up;
    }
}