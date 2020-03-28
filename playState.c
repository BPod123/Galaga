#include "playState.h"
#include "level.h"
#include "ship.h"
#include "images/explosion.h"
#include "images/explosionBig.h"
#include "images/explosionPlayer.h"
/** Excute movements for all of the enemies accourding to their Routes */
void enemyMovements(void);
// TODO: Handle firing missiles still
void handlePlayerInput(u32 currentButtons, u32 previousButtons);
void move(Ship *ship, Direction direction);
int isValidMotion(Ship *ship, Direction direction);
void handleCollisions(Game *game);

void showKillPlayer(void);
// TODO
void takeExtraLife(int life);
/* // TODO Maybe
void redrawAllShips(void);*/
/**
 * High level control for playing the level
 * @param game The current game
 * @param currentButtons The most recently pressed buttons by the player
 * @param previousButtons the currenmt buttons from the previous loop in Game.main*/
void runPlayState(Game *game, u32 currentButtons, u32 previousButtons) {
    int lives = game->lives;
    waitForVBlank();
    enemyMovements();
    handlePlayerInput(currentButtons, previousButtons);
    handleCollisions(game);
    if (lives > game->lives) {
        // Player has died
        showKillPlayer();
        if (game->lives > 0) {
            takeExtraLife(game->lives);
        }
    }
}
void enemyMovements(void) {
    /*
    for(int i = 0; i < numEnemies; i++) {
        if (!enemies[i]->isActive)
            continue;
        if (enemies[i]->route.currentStep == ROUTE_COMPLEXITY - 1)
            planMovement(enemies[i]);
        executeMovement(enemies[i]);
    } */
}
/** Checks to see what collisions have taken place.
 * If an enemy ship is destroyed, will add to player score and set ship to explode
 * If the player is killed, will decrement the number of lives
 * @param game The current game 
 * */
void handleCollisions(Game *game) {
    UNUSED(game);
    // TODO
}
/** Moves player and fires missiles for player
 * @param currentButtons The mostRecent buttons pressed by user
 * @param previousButtons currentButtons from last iteration
 */
void handlePlayerInput(u32 currentButtons, u32 previousButtons) {
    UNUSED(previousButtons);
    if(KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        move(player, LEFT);
    }
    if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        move(player, RIGHT);
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
        if (ship->shipType == PLAYER)
        {
            drawShip(ship, UP);
        }
        else
        {
            drawShip(ship, direction);
        }
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
void takeExtraLife(int life) {
    // Get all living enemy ships back to their home position
    // TODO: Change this from teleporting them to letting them finish their routes
    for (int i = 0; i < numEnemies; i++) {
        if (enemies[i]->isActive) {
            // * Edge Case: The player dies while enemy is exploding
            if (enemies[i]->route.exploding > 0) {
                eraseShip(enemies[i]);
                enemies[i]->isActive = 0;
                continue;
            }
            // Send enemies that are on attack run back home
            if (enemies[i]->route.activity == ATTACKRUN) {
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
    while (player->cords.col != PLAYER_START_COL && player->cords.row != PLAYER_START_ROW) {
        //! TODO
    }
}
/** Stops the normal flow of the game, and shows the player exploding */
void showKillPlayer(void) {
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
