#include "playState.h"
#include "level.h"
//#include "util.h"
#include "ship.h"
#include "images/explosion.h"
#include "images/explosionBig.h"
#include "images/explosionPlayer.h"
#include "maneuvers.h"
#include "stdlib.h"
/** Excute movements for all of the enemies accourding to their Routes */
void enemyMovements(void);
void handlePlayerInput(u32 currentButtons, u32 previousButtons);
void move(Ship *ship, Direction direction);
int isValidMotion(Ship *ship, Direction direction);
void handleCollisions(Game *game);
void handleExplosion(Ship *ship);
void takeExtraLife(int life);
void executeRoute(Ship *ship);
/**
 * High level control for playing the level
 * @param game The current game
 * @param currentButtons The most recently pressed buttons by the player
 * @param previousButtons the currenmt buttons from the previous loop in Game.main*/
void runPlayState(u32 currentButtons, u32 previousButtons)
{
    Game *game = getGame();
    levelCounter++;
    int lives = game->lives;
    int level = game->level;
    int score = game->score;
    waitForVBlank();

    // * Enemy Actions
    enemyMovements();
    // * Player Actions
    if (player->route.activity == CONTROLLING)
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
    // Update score
    for (int i = 0; i < numEnemies; i++)
    {
        if (!enemies[i]->isActive && enemies[i]->route.activity != DEAD)
        {
            enemies[i]->route.activity = DEAD;
            game->score += 10;
        }
    }
    if (score != game->score)
        drawSidePanel();
    if (lives > game->lives)
    {
        if (game->lives == 0)
        {
            *getState() = LOSE;
            return;
        }
        if (game->lives > 0)
        {
            takeExtraLife(game->lives);
        }
    }
    else if (level < game->level)
    {
        if (game->level >= NUM_LEVELS)
        *getState() = WIN;
        else
        *getState() = NEW_LEVEL;
        // if (level > NUM_LEVELS)
        //     *getState() = WIN;
        // else
        // return NEW_LEVEL;
    }
}
void enemyMovements(void)
{
    int activeEnemies = 0;
    int attackingEnemies = 0;
    int floatingEnemies = 0;
    for (int i = 0; i < numEnemies; i++)
    {
        if (!enemies[i]->isActive)
            continue;
        if (player->route.activity != CONTROLLING)
        {
            enemies[i]->route.currentStep = enemies[i]->route.pathLength;
        }

        activeEnemies++;
        // Shift enemy homes
        int mod = levelCounter % (floatRadiusX * 2);
        if (mod < floatRadiusX)
            moveCords(&enemies[i]->home, LEFT);
        else
        {
            moveCords(&enemies[i]->home, RIGHT);
        }

        if (enemies[i]->route.activity == ATTACKRUN)
        {
            executeRoute(enemies[i]);
            attackingEnemies++;
            continue;
        }
        if (enemies[i]->route.activity == FLOATING)
        {
            floatingEnemies++;
            executeRoute(enemies[i]);
            continue;
        }
    }
    if (activeEnemies == 0 && getGame()->lives > 0)
    {
        getGame()->level++;
        return;
    }
    if (attackingEnemies == 0 && floatingEnemies > 0 && player->route.activity == CONTROLLING)
    {
        // Do lines or not
        srand(random);
        random = rand();
        srand(random);
        int lineLength = rand() % numAttackers;
        int attackIndex = rand() % numAttackers;
        for (int i = 0, a = 0, line = 0, seed = rand(); i < numEnemies && a < numAttackers && a < floatingEnemies; i++)
        {
            if (!enemies[attackIndex]->isActive || enemies[attackIndex]->route.activity != FLOATING)
            {
                attackIndex++;
                if (attackIndex >= numEnemies)
                {
                    attackIndex = 0;
                }
            }
            else
            {
                if (enemies[attackIndex]->cords.row != enemies[attackIndex]->home.row)
                {
                    floatingEnemies--;
                    continue;
                }
                else
                {
                    a++;
                    enemies[attackIndex]->route.activity = ATTACKRUN;
                    planRoute(enemies[attackIndex], seed);
                    enemies[i]->route.path[1].col = (GAME_WIDTH - getWidth(enemies[i])) % enemies[i]->route.path[0].col;
                    line++;
                    if (line >= lineLength)
                    {
                        line = 0;
                        seed = rand();
                    }

                    executeRoute(enemies[attackIndex++]);
                }
            }
        }
    }
}
/** Checks to see what collisions have taken place.
 * If the player is killed, will decrement the number of lives
 * @param game The current game 
 * */
void handleCollisions(Game *game)
{
    UNUSED(game);
    // TODO
    for (int i = 0; i < numEnemies; i++)
    {
        if (!enemies[i]->isActive)
            continue;
        for (int m = 0; m < MAX_MISSILES; m++)
        {
            if (!missiles[m]->isActive)
                continue;
            if (hasCollided(enemies[i], missiles[m]))
            {
                eraseShip(missiles[m]);
                missiles[m]->isActive = 0;
                enemies[i]->route.activity = EXPLODING;
                break;
            }
        }
        if (hasCollided(enemies[i], player))
        {
            move(enemies[i], UP);
            enemies[i]->route.activity = EXPLODING;
            player->route.activity = EXPLODING;
        }
        if (enemies[i]->route.activity == EXPLODING)
        {
            handleExplosion(enemies[i]);
            eraseShip(enemies[i]);
            enemies[i]->isActive = 0;
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
            if (ship->shipType != PLAYER && ship->shipType != MISSILE && ship->cords.row > player->cords.row + getHeight(player))
            {
                move(ship, LEFT); // Move to the sides first, so it doesn't move on the player's row
                direction = LEFT;
            }
            else
            {
                ship->cords.col--;
                ship->cords.row++;
            }

            break;
        case DR:
            if (ship->shipType != PLAYER && ship->shipType != MISSILE && ship->cords.row > player->cords.row + getHeight(player))
            {
                move(ship, RIGHT); // Move to the sides first, so it doesn't move on the player's row
                direction = RIGHT;
            }
            else
            {
                ship->cords.col++;
                ship->cords.row++;
            }
            break;
        default: // NEUTRAL
            return;
        }
        if (ship->shipType == PLAYER || (ship->route.activity == FLOATING && ship->cords.row == ship->home.row))
        {
            drawShip(ship, UP);
        }
        else
        {
            drawShip(ship, direction);
        }
        ship->direction = direction;
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
        return cords->row < (HEIGHT - getHeight(ship));
    case LEFT:
        return cords->col > 0;
    default:
        return cords->col < (GAME_WIDTH - getWidth(ship)); // RIGHT
    }
}
void takeExtraLife(int life)
{
    player->route.activity = RETURNING_HOME;
    // Get all living enemy ships back to their home position
    for (int i = 0; i < numEnemies; i++)
    {
        if (!enemies[i]->isActive)
            continue;
        enemies[i]->route.activity = FLOATING;
    }
    player->cords.col = EXTRA_LIFE_COL(life - 1);
    player->cords.row = EXTRA_LIFE_ROW;
}
// /** Stops the normal flow of the game, and shows the player exploding */
// void showKillPlayer(void)
// {
//     delay(DELAY_TIME * 3);
//     waitForVBlank();
//     drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosion);

//     delay(DELAY_TIME * 2);
//     waitForVBlank();
//     drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionBig);

//     delay(DELAY_TIME * 2);
//     waitForVBlank();
//     drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosion);

//     delay(DELAY_TIME * 2);
//     waitForVBlank();
//     drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionBig);

//     delay(DELAY_TIME * 2);
//     waitForVBlank();
//     drawImageDMA(player->cords.row, player->cords.col, getWidth(player), getHeight(player), explosionPlayer);

//     delay(DELAY_TIME);
//     waitForVBlank();
//     eraseShip(player);
//     delay(DELAY_TIME);
// }
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
    {
        Direction target = getRelativeDirection(ship->cords, ship->home);
        if (target == NEUTRAL)
            return;
        else
        {
            move(ship, target);
            return;
        }
        // This is done in enemy movements now
        // int mod = levelCounter % (floatRadiusX * 2);
        // if (mod < floatRadiusX)
        //     move(ship, LEFT);
        // else
        // {
        //     move(ship, RIGHT);
        // }
    }
    break;
    case ATTACKRUN:
        // Direction direction = getRelativeDirection(&ship->cords, ship->route.path[ship->route.currentStep]);
        if (getRelativeDirection(ship->cords, ship->route.path[ship->route.currentStep]) == NEUTRAL)
        {
            ship->route.currentStep++;
            if (ship->route.currentStep == ship->route.pathLength - 1 && count % 3 == 0)
            {
                ship->route.path[ship->route.pathLength - 1] = player->cords;
            }
            if (ship->route.currentStep == ship->route.pathLength)
            {
                ship->route.activity = RETURNING_HOME;
            }
            executeRoute(ship);
            return;
        }
        else
        {
            Direction direction = getRelativeDirection(ship->cords, ship->route.path[ship->route.currentStep]);
            if (isValidMotion(ship, direction))
                move(ship, direction);
            else
                ship->route.activity = FLOATING;
        }
        break;
    case RETURNING_HOME:
    {
        if (ship->shipType != PLAYER && ship->shipType != MISSILE)
        {
            // ship->route.activity = FLOATING;
            // executeRoute(ship);
            // if (!CORDS_EQUAL(ship->cords, ship->home))
            //     ship->route.activity = RETURNING_HOME;
            ship->route.activity = FLOATING;
            executeRoute(ship);
            return;
            // Direction direction = getRelativeDirection(ship->cords, ship->home);
            // if (direction == NEUTRAL) {
            //     ship->route.activity = FLOATING;
            //     return;
            // }
            // drawShip(ship, ship->direction);
            // Direction direction = getRelativeDirection(ship->cords, ship->home);
            // move(ship, direction);
            // direction = getRelativeDirection(ship->cords, ship->home);
            // if (direction == NEUTRAL)
            //     ship->route.activity = FLOATING;
            return;
        }
        Direction direction = getRelativeDirection(ship->cords, ship->home);
        if (direction == NEUTRAL)
        {
            if (ship->shipType == PLAYER)
            {
                ship->route.activity = CONTROLLING;
                return;
            }
            else if (ship->shipType == MISSILE)
            {
                // Missile has flown as far as its gonna go
                ship->isActive = 0;
                eraseShip(ship);
                return;
            }
        }
        else
        {
            move(ship, direction);
        }

        break;
    }
    case EXPLODING:
        handleExplosion(ship);
        ship->isActive = 0;
        if (ship->shipType == PLAYER && getGame()->lives > 0)
            takeExtraLife(getGame()->lives);
        break;
    default:
        break;
    }
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
    if (ship->shipType == PLAYER)
    {
        drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSIONPLAYER_WIDTH, EXPLOSIONPLAYER_HEIGHT, explosionPlayer);
    }
    else
    {
        drawImageDMA(ship->cords.row, ship->cords.col, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, explosion);
    }
    delay(DELAY_TIME / 2);
    waitForVBlank();
    eraseShip(ship);
    ship->isActive = 0;
    if (ship->shipType == PLAYER)
    {
        getGame()->lives--;
    }
}
