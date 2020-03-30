#include "playState.h"
#include "level.h"
//#include "util.h"
#include "ship.h"
#include "images/explosion.h"
#include "images/explosionBig.h"
#include "images/explosionPlayer.h"
#include "maneuvers.h"
/** Excute movements for all of the enemies accourding to their Routes */
void enemyMovements(void);
// TODO: Handle firing missiles still
void handlePlayerInput(u32 currentButtons, u32 previousButtons);
void move(Ship *ship, Direction direction);
int isValidMotion(Ship *ship, Direction direction);
void handleCollisions(Game *game);
Cords getFloatTarget(Ship *ship);
void handleExplosion(Ship *ship);

void showKillPlayer(void);
// TODO
void takeExtraLife(int life);
void executeRoute(Ship *ship);
void planRoute(Ship *enemy);

/* // TODO Maybe
void redrawAllShips(void);*/
/**
 * High level control for playing the level
 * @param game The current game
 * @param currentButtons The most recently pressed buttons by the player
 * @param previousButtons the currenmt buttons from the previous loop in Game.main*/
void runPlayState(Game *game, u32 currentButtons, u32 previousButtons)
{
    levelCounter++;
    int lives = game->lives;
    waitForVBlank();
    // * Enemy Actions
    enemyMovements();
    // * Player Actions
    if (player->isActive)
        handlePlayerInput(currentButtons, previousButtons);
    else
    {
        executeRoute(player);
    }
    // * Missile Actions
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        if (missiles[i]->isActive)
            executeRoute(missiles[i]);
    }
    handleCollisions(game);
    if (lives > game->lives)
    {
        // Player has died
        showKillPlayer();
        if (game->lives > 0)
        {
            takeExtraLife(game->lives);
        }
    }
}
void enemyMovements(void)
{
    // Do Float Tracker
    if (floatTracker->route.currentStep >= floatTracker->route.pathLength)
        floatTracker->route.currentStep = 0;
    executeRoute(floatTracker);
    int activeEnemies = 0;
    int attackingEnemies = 0;
    int floatingEnemies = 0;

    for (int i = 0; i < numEnemies; i++)
    {
        if (!enemies[i]->isActive)
            continue;
        activeEnemies++;

        if (enemies[i]->route.activity == ATTACKRUN)
        {

            if (enemies[i]->route.currentStep <= enemies[i]->route.pathLength)
            {
                // New Floating enemy
                // Crash into bottom of stage to send back to top
                Cords **path = enemies[i]->route.path;
                path[0]->col = player->cords.col;
                path[0]->row = HEIGHT; // TODO: In collision, when ship hits bottom, teleport to top and send home
                enemies[i]->route.currentStep = 0;
                enemies[i]->route.pathLength = 1;
                enemies[i]->route.activity = RETRUNING_HOME;
                executeRoute(enemies[i]);
                continue;
            }
            else
            {
                attackingEnemies++;
            }
        }
        if (enemies[i]->route.activity == FLOATING)
        {
            // int floatWidth = floatTracker->route.path[0]->col;
            // int num = (floatWidth % count);
            // Cords targetCords;
            // targetCords.row = enemies[i]->home.row;
            // if (num < floatWidth / 2) {
            //     targetCords.col = 0;//enemies[i]->home.col - num;
            // } else {
            //     targetCords.col = GAME_WIDTH - SHIP_WIDTH;//enemies[i]->home.col + num;
            // }
            // Direction direction = getRelativeDirection(&enemies[i]->cords, &targetCords);

            // move(enemies[i], direction);
            floatingEnemies++;
            executeRoute(enemies[i]);
            continue;
        }
    }
    // if (attackingEnemies == 0) // TODO Implement getting new attackers later
    // {
    //     for (int i = 0, f = 0; i < numEnemies && f < floatingEnemies; i++)
    //     {
    //         // Pick new attackers randomly
    //         if (!enemies[i]->isActive || enemies[i]->route.activity != FLOATING)
    //         {
    //             continue;
    //         }
    //         enemies[i]->route.activity = ATTACKRUN;
    //         f++;
    //         planRoute(enemies[i]);
    //         executeRoute(enemies[i]);
    //     }
    // }
}
/** Checks to see what collisions have taken place.
 * If an enemy ship is destroyed, will add to player score and set ship to explode
 * If the player is killed, will decrement the number of lives
 * @param game The current game 
 * */
void handleCollisions(Game *game)
{
    UNUSED(game);
    // TODO
    for (int i = 0; i < numEnemies; i++) {
        if (!enemies[i]->isActive)
            continue;
        for(int m = 0; m < MAX_MISSILES; m++) {
            if (!missiles[m]->isActive)
                continue;
            if (hasCollided(enemies[i], missiles[m])) {
                eraseShip(missiles[m]);
                missiles[m]->isActive = 0;
                enemies[i]->route.activity = EXPLODING;
                game->score += 10;
                drawSidePanel(game);
                break;
            }
        }
        if (hasCollided(enemies[i], player)) {
            enemies[i]->route.activity = EXPLODING;
            player->route.activity = EXPLODING;
            game->score += 10;
            drawSidePanel(game);
        }
        if (enemies[i]->route.activity == EXPLODING)
            handleExplosion(enemies[i]);
        if (player->route.activity == EXPLODING) {
            handleExplosion(player);
            waitForVBlank();
            drawSidePanel(game);
        }

    }
}
/** Moves player and fires missiles for player
 * @param currentButtons The mostRecent buttons pressed by user
 * @param previousButtons currentButtons from last iteration
 */
void handlePlayerInput(u32 currentButtons, u32 previousButtons)
{
    UNUSED(previousButtons);
    if (KEY_DOWN(BUTTON_LEFT, currentButtons))
    {
        move(player, LEFT);
    }
    if (KEY_DOWN(BUTTON_RIGHT, currentButtons))
    {
        move(player, RIGHT);
    }
    if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons))
    {
        for (int index = 0; index < MAX_MISSILES; index++)
        {
            if (missiles[index]->isActive)
                continue;

            missiles[index]->isActive = 1;
            missiles[index]->direction = UP;
            missiles[index]->cords.col = player->cords.col + getWidth(player) / 2 - getWidth(missiles[index]) / 2;
            missiles[index]->cords.row = player->cords.row - getHeight(missiles[index]) - 1;
            missiles[index]->home.col = missiles[index]->cords.col;
            missiles[index]->home.row = 0;
            drawShip(missiles[index], missiles[index]->direction);

            break;
        }
    }
}
void move(Ship *ship, Direction direction)
{
    if (isValidMotion(ship, direction))
    {
        if (ship->shipType != NONE)
            eraseShip(ship);
        // Find new Cords
        switch (direction)
        {
        case UP:
            ship->cords.row--;
            break;
        case DOWN:
            ship->cords.row++;
            break;
        case LEFT:
            ship->cords.col--;
            break;
        case RIGHT:
            ship->cords.col++;
            break;
        case UL:
            ship->cords.col--;
            ship->cords.row--;
            break;
        case UR:
            ship->cords.col++;
            ship->cords.row--;
            break;
        case DL:
            ship->cords.col--;
            ship->cords.row++;
            break;
        case DR:
            ship->cords.col++;
            ship->cords.row++;
            break;
        default:
            return;
        }
        if (ship->shipType != NONE)
        {
            if (ship->shipType == PLAYER || ship->route.activity == FLOATING)
            {
                drawShip(ship, UP);
            }
            else
            {
                drawShip(ship, direction);
            }
        }
        ship->direction = direction;
    }
    else
    {

        if (ship->route.activity == FLOATING)
            floatTracker->route.currentStep++;
    }
}

/**
 * @return true if there is room for the ship to move in the direction
 */
int isValidMotion(Ship *ship, Direction direction)
{
    Cords *cords;
    cords = &(ship->cords);
    switch (direction)
    {
    case UL:
        return isValidMotion(ship, UP) && isValidMotion(ship, LEFT);
    case UR:
        return isValidMotion(ship, UP) && isValidMotion(ship, RIGHT);
    case DL:
        return isValidMotion(ship, DOWN) && isValidMotion(ship, LEFT);
    case DR:
        return isValidMotion(ship, DOWN) && isValidMotion(ship, RIGHT);
    case UP:
        return cords->row > 0;
    case DOWN:
        return cords->row < HEIGHT;
    case LEFT:
        return cords->col > 0;
    default:
        return cords->col < GAME_WIDTH; // RIGHT
    }
}
void takeExtraLife(int life)
{
    // Get all living enemy ships back to their home position
    // TODO: Change this from teleporting them to letting them finish their routes
    for (int i = 0; i < numEnemies; i++)
    {
        if (enemies[i]->isActive)
        {
            // * Edge Case: The player dies while enemy is exploding
            if (enemies[i]->route.exploding > 0)
            {
                eraseShip(enemies[i]);
                enemies[i]->isActive = 0;
                continue;
            }
            // Send enemies that are on attack run back home
            if (enemies[i]->route.activity == ATTACKRUN)
            {
                eraseShip(enemies[i]);
                enemies[i]->cords.col = enemies[i]->route.path[ROUTE_COMPLEXITY - 1]->col;
                enemies[i]->cords.row = enemies[i]->route.path[ROUTE_COMPLEXITY - 1]->row;
                enemies[i]->route.currentStep = ROUTE_COMPLEXITY;
                waitForVBlank();
                drawShip(enemies[i], UP);
            }
        }
    }

    // TODO: Move player to start position
    player->cords.col = EXTRA_LIFE_COL(life);
    player->cords.row = EXTRA_LIFE_ROW;
    player->isActive = 0; // ! Set to 1 once player is in start position
    // * Route player to starting position
    player->route.path[ROUTE_COMPLEXITY - 1]->col = PLAYER_START_COL;
    player->route.path[ROUTE_COMPLEXITY - 1]->row = PLAYER_START_ROW;
    player->route.currentStep = ROUTE_COMPLEXITY;
}
/** Stops the normal flow of the game, and shows the player exploding */
void showKillPlayer(void)
{
    delay(DELAY_TIME * 3);
    waitForVBlank();
    drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosion);

    delay(DELAY_TIME * 2);
    waitForVBlank();
    drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionBig);

    delay(DELAY_TIME * 2);
    waitForVBlank();
    drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosion);

    delay(DELAY_TIME * 2);
    waitForVBlank();
    drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionBig);

    delay(DELAY_TIME * 2);
    waitForVBlank();
    drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionPlayer);

    delay(DELAY_TIME);
    waitForVBlank();
    eraseShip(player);
    delay(DELAY_TIME);
}
/**
 * Moves ship in the direction of its current target
 *
 * @param ship The ship to be moved.
 */
void executeRoute(Ship *ship)
{
    switch (ship->route.activity)
    {
    case FLOATING:
        /*if (ship->shipType == NONE)
        {
            // This is the floatTracker
            // Check if at current target
            Cords *targetPtr = (ship->route.path[ship->route.currentStep]);
            Cords target;
            target.col = targetPtr->col;
            target.row = targetPtr->row;
            // Make sure it is going in the right direction
            if (CORDS_EQUAL(ship->cords, target))
            {
                // FloatTracker has reached its current target
                ship->route.currentStep++;
                // Check for out of bounds
                if (ship->route.currentStep >= ship->route.pathLength)
                    ship->route.currentStep = 0;

                executeRoute(ship);
                return;
            }
            Direction direction = getRelativeDirection(&ship->cords, ship->route.path[ship->route.currentStep]);
            moveCords(&floatTracker->cords, direction);
            ship->direction = direction;
            return;
        }
        // Ship is a normal enemy floating
        Cords target = getFloatTarget(ship);
        Direction direction = getRelativeDirection(&ship->cords, &target);
        move(ship, direction);*/
        {
            int mod = levelCounter % (floatRadiusX * 2);
            if (mod < floatRadiusX)
                move(ship, LEFT);
            else
            {
                move(ship, RIGHT);
            }
        }
        break;
    case ATTACKRUN:
        // Direction direction = getRelativeDirection(&ship->cords, ship->route.path[ship->route.currentStep]);
        if (getRelativeDirection(&ship->cords, ship->route.path[ship->route.currentStep]) == NEUTRAL)
        {
            ship->route.currentStep++;
            if (ship->route.currentStep == ship->route.pathLength)
            {
                ship->route.activity = RETRUNING_HOME;
            }
            executeRoute(ship);
            return;
        }
        else
        {
            move(ship, getRelativeDirection(&ship->cords, ship->route.path[ship->route.currentStep]));
        }
        break;
    case RETRUNING_HOME:
        if (ship->shipType == PLAYER)
        {
            Direction direction = getRelativeDirection(&ship->cords, &ship->home);
            if (direction == NEUTRAL)
            {
                // Player has returned home
                ship->isActive = 1;
                ship->route.activity = CONTROLLING;
            }
            else
                // player still returning home
                move(ship, direction);
            return;
        }
        else if (ship->shipType == MISSILE)
        {
            Direction direction = getRelativeDirection(&ship->cords, &ship->home);
            if (direction == NEUTRAL)
            {
                // Missile has flown as far as its gonna go
                ship->isActive = 0;
                eraseShip(ship);
            }
            else
                // Missile stil traveling
                move(ship, direction);
            return;
        }
        else
        {
            // Ship is an enemy
            // Get in line with floaters
            // Since the floatTracker just moved, need to compare the coordinates of the result of moving with floatTracker
            Cords floatSeparation = getSeparation(&floatTracker->cords, &floatTracker->home);
            Cords targetCords;
            targetCords.col = ship->home.col + floatSeparation.col;
            targetCords.row = ship->home.row = floatSeparation.row;
            Cords shipSeparation = getSeparation(&ship->cords, &targetCords);
            if (CORDS_EQUAL(shipSeparation, floatSeparation))
            {
                // This means that the ship is where is needs to be at the beginning of next round
                // Ship becomes a floater, but DOES NOT MOVE this round
                ship->route.activity = FLOATING;
                return;
            }
            Cords potentialCords = ship->cords;
            moveCords(&potentialCords, floatTracker->direction);
            Cords potentialSeparation = getSeparation(&potentialCords, &targetCords);
            if (CORDS_EQUAL(potentialSeparation, floatSeparation))
            {
                // The ship is lined up with the floaters
                // The ship becomes a floater and DOES MOVE this round
                ship->route.activity = FLOATING;
                move(ship, floatTracker->direction);
                return;
            }
        }
        break;
    case EXPLODING:
        if (ship->route.exploding > EXPLOSION_FRAMES || (ship->shipType != PLAYER && ship->route.exploding >= EXPLOSION_FRAMES))
        {
            ship->route.exploding = 0;
            eraseShip(ship);
            ship->isActive = 0;
            return;
        }
        if (ship->route.exploding == EXPLOSION_FRAMES && ship->shipType == PLAYER)
        {
            drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSIONPLAYER_WIDTH, EXPLOSIONPLAYER_HEIGHT, explosionPlayer);
        }
        else
        {
            if (ship->route.exploding % 2 == 0)
            {
                drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSIONBIG_WIDTH, EXPLOSIONBIG_HEIGHT, explosionBig);
            }
            else
            {
                drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosion);
            }
        }
        ship->route.exploding++;
        break;
    default: // Should never come to this
        return;
    }
}
Cords getFloatTarget(Ship *ship)
{
    Cords target;
    target.col = ship->home.col + floatTracker->cords.col;
    target.row = ship->home.row + floatTracker->cords.row;
    return target;
}
void handleExplosion(Ship *ship)
{
    for (int i = 0; i < EXPLOSION_FRAMES - 1; i++)
    {
        waitForVBlank();
        if (i % 2 == 0)
        {
            drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSIONBIG_WIDTH, EXPLOSIONBIG_HEIGHT, explosionBig);
        }
        else
        {
            drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosion);
        }
        delay(DELAY_TIME / 2);
    }
    waitForVBlank();
    if (ship->shipType == PLAYER) {
        drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSIONPLAYER_WIDTH, EXPLOSIONPLAYER_HEIGHT, explosionPlayer);
    }
    else {
        drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosion);
    }
    delay(DELAY_TIME / 2);
    waitForVBlank();
    eraseShip(ship);
    ship->isActive = 0;
    if (ship->shipType == PLAYER) {
        getGame()->lives--;
    }
}
