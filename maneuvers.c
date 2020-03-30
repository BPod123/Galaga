#include "maneuvers.h"
#include "level.h"
#include "game.h"
#include "ship.h"
#include "playState.h"
#include <stdlib.h>
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
/**
 * Creats an instruction that tells the ship to do a flip
 * @param ship The ship
 * @param ccw if > 0, then the flip will be counter clock wise, otherwise it will be clockwise 
 */
Instruction *flip(int ccw, Instruction *inst)
{
    // Do the left and right sides of the flip (the flip moves the ship in a circle)
    if (ccw)
    {
        Direction directions[8] = {RIGHT, UR, UP, UL, LEFT, DL, DOWN, DR};
        inst->directions = malloc(sizeof(Direction) * 8);
        for (int i = 0; i < 8; i++)
        {
            inst->directions[i] = &directions[i];
        }
    }
    else
    {
        Direction directions[8] = {LEFT, UL, UP, UR, RIGHT, DR, DOWN, DL};
        inst->directions = malloc(sizeof(Direction) * 8);
        for (int i = 0; i < 8; i++)
        {
            inst->directions[i] = &directions[i];
        }
    }
    return inst;
}

/** Plans enemy attack runs */
void planRoute(Ship *ship)
{
    srand(random);
    int numFlips = rand() % 3;
    int complexity = rand() % 4;
    ship->route.pathLength = (3 + complexity + 4 * numFlips) + 1;
    ship->route.path = malloc(sizeof(ship->route.path) * ship->route.pathLength);
    Cords *target = malloc(sizeof(Cords));
    target->col = (GAME_WIDTH - getWidth(ship)) / 2 + (rand() % (GAME_WIDTH / 4) - GAME_WIDTH / 8);
    target->row = 2 * getHeight(ship) + rand() % 50;
    Instruction *firstInst = malloc(sizeof(Instruction));
    firstInst->gotoTarget = 1;
    firstInst->target = target;
    int side = rand();
    for (int i = 1; i < ship->route.pathLength - 1; i++)
    {
        Instruction *inst = malloc(sizeof(Instruction));
        inst->target = malloc(sizeof(Cords));
        ship->route.path[i] = inst;
        // Go back and forth from left to right side
        if (i % 2 == 0 && numFlips > 0 && rand() % ship->route.pathLength == 0)
        {
            // pass middile
            inst->target->col = rand() % (GAME_WIDTH - getWidth(ship) / 4 + (GAME_WIDTH - getWidth(ship)) / 8);
            inst->target->row = (rand() % ((HEIGHT - getHeight(ship)) / ship->route.pathLength)) * i;
            numFlips--;
            int ccw;
            Direction dir = (getDirection(ship->route.path[i - 1]->target, ship->route.path[i - 2]->target, 1));
            ccw = (dir == UP || dir == DR || dir == RIGHT || dir == UR);
            inst->target = ship->route.path[i - 1]->target;
            flip(ccw, inst);
            continue;
        }
        inst->gotoTarget = 1;
        if ((side + i) % 3 == 0 || rand() % 2 == 0)
        {
            // Go to the left side
            inst->target->col = rand() % ((GAME_WIDTH - getWidth(ship)) / 2);
        }
        else
        {
            inst->target->col = rand() % (GAME_WIDTH - getWidth(ship) / 2) + (GAME_WIDTH - getWidth(ship) / 2);
        }
        inst->target->row = (rand() % ((HEIGHT - getHeight(ship)) / ship->route.pathLength)) * i;
    }
}

/**
 * Returns the best direction to go in if you could travel through walls to get from
 * c1 to c2
 * @param c1 the starting coordinate
 * @param c2 the target coordinate
 * @return the optimal direction to travel in */
Direction getBestDirection(Cords c1, Cords c2)
{
    return getDirection(&c1, &c2, 0);
    // Direction relDirection = getDirection(&c1, &c2, 0);
    // if (relDirection == NEUTRAL)
    // {
    //     return relDirection;
    // }
    // Cords relSeparation = getSeparation(c1, c2);
    // int relDistance = relSeparation.col + relSeparation.row;

    // int nearestLRWall = c1.col < (GAME_WIDTH - SHIP_WIDTH) / 2 ? LEFT : RIGHT;
    // int nearestUDWall = c1.row < (HEIGHT - SHIP_HEIGHT) / 2 ? UP : DOWN;

    // int lrWallDist = nearestLRWall == LEFT ? c1.col : GAME_WIDTH - SHIP_WIDTH - c1.col;
    // int udWallDist = nearestUDWall == UP ? c1.row : HEIGHT - SHIP_HEIGHT - c1.row;

    // // The coorniates of where the ship will be after traveling through this wall
    // Cords lrWall = {c1.row, nearestLRWall == LEFT ? GAME_WIDTH - SHIP_WIDTH : 0};
    // Cords lrSeparation = getSeparation(lrWall, c2);
    // int distThroughLRWall = lrWallDist + abs(lrSeparation.col) + abs(lrSeparation.row);
    // // The coorniates of where the ship will be after traveling through this wall
    // Cords udWall = {nearestUDWall == UP ? HEIGHT - SHIP_HEIGHT : 0, c1.col};
    // Cords udSeparation = getSeparation(udWall, c2);
    // int distThroughUDWall = udWallDist + abs(udSeparation.col) + abs(udSeparation.row);

    // // The cordniates of where the ship will be after traveling through both walls
    // // Cords bothWalls = {udWall.row, lrWall.col};
    // // Cords bothWallsSeparation = getSeparation(bothWalls, c2);
    // // //int distThroughBothWalls = lrWallDist + udWallDist + abs(bothWallsSeparation.col) + abs(bothWallsSeparation.row);

    // // Now, find the best direction to travel in
    // int minDist = relDistance;
    // Direction bestDir = relDirection;
    // if (minDist > distThroughLRWall)
    // {
    //     minDist = distThroughLRWall;
    //     bestDir = nearestLRWall;
    // }
    // if (minDist > distThroughUDWall)
    // {
    //     minDist = distThroughUDWall;
    //     bestDir = nearestUDWall;
    // }
    // // if (minDist > distThroughBothWalls)
    // // {
    // //     bestDir = combineDirections(nearestUDWall, nearestLRWall);
    // // }
    // return bestDir;
}
/**
 * @param upDown Direction.UP or Direction.DOWN
 * @param leftRight Direction.LEFT or DIRECTION.RIGHT
 * @return The resultant direction from combining the two cordinal directions */
Direction combineDirections(Direction upDown, Direction leftRight)
{
    switch (upDown)
    {
    case UP:
        switch (leftRight)
        {
        case LEFT:
            return UL;
        case RIGHT:
            return UR;
        default:
            return UP;
        }
    case DOWN:
        switch (leftRight)
        {
        case LEFT:
            return DL;
        case RIGHT:
            return DR;
        default:
            return DOWN;
        }
    default:
        return leftRight;
    }
}
/** 
 * @param cords1 Pointer to a ships Cords
 * @param cords2 Pointer to target's cords
 * @param isEnemy Enemies are capable of moving through walls, function will handle that
 * @return Gets the relative direction of cords2 from cords 1 */
Direction getDirection(Cords *cords1, Cords *cords2, int isEnemy)
{
    if (isEnemy)
    {
        return getBestDirection(*cords1, *cords2);
    }
    // -1 = DOWN, 0 = Neutral, 1 = UP
    Direction vertical = NEUTRAL;
    // -1 = LEFT, 0 = Neutral, 1 = RIGHT
    Direction horizontal = NEUTRAL;
    // Check relative rows
    if (cords2->row < cords1->row)
        vertical = UP;
    else if (cords2->row > cords1->row)
        vertical = DOWN;
    // Check Columns
    if (cords2->col < cords1->col)
        horizontal = LEFT;
    else if (cords2->col > cords1->col)
        horizontal = RIGHT;
    // Determine direction

    switch (vertical)
    {
    case NEUTRAL:
    {
        return horizontal;
    }
    case DOWN:
    {
        switch (horizontal)
        {
        case LEFT:
            return DL;
        case RIGHT:
            return DR;
        default:
            return DOWN;
        }
    }
    case UP:
    {
        switch (horizontal)
        {
        case LEFT:
            return UL;
        case RIGHT:
            return UR;
        default:
            return UP;
        }
    }
    default:
        break;
    }
    return NEUTRAL; // Cords are the same
}
/** 
 * @param c1 The base cordniate
 * @param c2 The target cordinate
 * @return  the vertical and horizontal distance between two cordinates */
Cords getSeparation(Cords c1, Cords c2)
{
    Cords res;
    res.col = c2.col - c1.col;
    res.row = c2.row - c1.row;
    return res;
}
/** Shifts the cords in a direction */
void moveCords(Cords *cords, Direction direction)
{
    switch (direction)
    {
    case UP:
        cords->row--;
        break;
    case DOWN:
        cords->row++;
        break;
    case LEFT:
        cords->col--;
        break;
    case RIGHT:
        cords->col++;
        break;
    case UL:
        cords->col--;
        cords->row--;
        break;
    case UR:
        cords->col++;
        cords->row--;
        break;
    case DL:
        cords->col--;
        cords->row++;
        break;
    case DR:
        cords->col++;
        cords->row++;
        break;
    default:
        return;
    }
}
/**
 * @param ship the ship in question
 * @return 0 if the ship has executed all of its instructions, 1 if not
 * */
int hasMoreInstructions(Ship *ship)
{
    if (ship->route.currentStep < ship->route.pathLength - 1)
        return 1;
    else if (ship->route.currentStep >= ship->route.pathLength)
        return 0;
    // The ship is on its last instruction
    Instruction *instruction = ship->route.path[ship->route.currentStep];
    if (instruction->gotoTarget)
        return (getDirection(ship->cords, instruction->target, 0) == NEUTRAL) && !instruction->hasNextPart;
    else
        return !instruction->hasNextPart && instruction->currentDirection < instruction->numDirections;
}
/** 
 * @param direction the direction
 * @return Returns the immediate adjacent direction */
Direction *getAdjacentDirections(Direction direction)
{
    Direction *res = (Direction *)malloc(sizeof(Direction *) * 2);
    switch (direction)
    {
    case UP:
        res[0] = UL;
        res[1] = UR;
        break;
    case DOWN:
        res[0] = DL;
        res[1] = DR;
        break;
    case LEFT:
        res[0] = UL;
        res[1] = DL;
        break;
    case RIGHT:
        res[0] = DR;
        res[1] = UR;
        break;
    case UL:
        res[0] = UP;
        res[1] = LEFT;
        break;
    case UR:
        res[0] = UP;
        res[1] = RIGHT;
        break;
    case DL:
        res[0] = DOWN;
        res[1] = LEFT;
        break;
    case DR:
        res[0] = DOWN;
        res[1] = LEFT;
        break;
    default: // NEUTRAL
        res[0] = NEUTRAL;
        res[1] = NEUTRAL;
        break;
    }
    return res;
}
void executeInstruction(Ship *ship)
{
    Instruction *inst = ship->route.path[ship->route.currentStep];

    if (inst->gotoTarget)
    {
        if (getDirection(ship->cords, inst->target, 0) == NEUTRAL)
        {
            if (inst->hasNextPart)
            {
                Instruction oldInst = *ship->route.path[ship->route.currentStep];
                ship->route.path[ship->route.currentStep] = inst->nextPart;
                oldInst.nextPart = malloc(sizeof(Instruction));
                deconstructInstruction(&oldInst);
                executeInstruction(ship);
                return;
            }
            else
            {
                ship->route.currentStep++;
                executeInstruction(ship);
                return;
            }
        }
        else
        {
            int isEnemy = ship->shipType != PLAYER && ship->shipType != MISSILE;
            move(ship, getDirection(ship->cords, inst->target, isEnemy));
            return;
        }
    }

    // Follow directions
    if (inst->currentDirection >= inst->numDirections)
    {
        if (inst->hasNextPart)
        {
            Instruction oldInst = *ship->route.path[ship->route.currentStep];
            ship->route.path[ship->route.currentStep] = inst->nextPart;
            oldInst.nextPart = malloc(sizeof(Instruction));
            deconstructInstruction(&oldInst);
            executeInstruction(ship);
        }
        else
        {
            ship->route.currentStep++;
            executeInstruction(ship);
            return;
        }
    }
    else
    {
        move(ship, *inst->directions[inst->currentDirection++]);
    }
}