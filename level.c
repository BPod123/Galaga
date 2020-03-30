#include "level.h"
#include "ship.h"
#include "images/levelBackground.h"
#include "images/playerShip_Up.h"
#include <stdlib.h>
#include <math.h>
#define EXTRA_LIFE_COL(i) (SHIP_WIDTH + SHIP_WIDTH * (i))
#define EXTRA_LIFE_ROW (HEIGHT - SHIP_WIDTH)
#define DRAW_EXTRA_LIFE(i) drawImageDMA(EXTRA_LIFE_ROW, EXTRA_LIFE_COL(i), SHIP_WIDTH, SHIP_HEIGHT, playerShip_Up);
char *intToString(int num);
//void makeFloatPath(Cords floatRadius);
// * These methods set up the field for each level
void levelOne(void);
void levelTwo(void);
void levelThree(void);
void levelFour(void);
void levelFive(void);
void levelSix(void);
/** All of the enemies in the game */
Ship **enemies;
/** The player */
Ship *player;
/** The missiles that can exist in the game */
Ship *missiles[MAX_MISSILES];
/** An invisible ship that is always floating. It is used to keep all of the enemies floating in the same direction at the same time */
Ship *floatTracker;
int levelCounter;
int floatRadiusX;
Game *game;
/** The number of enemies in the current level, set in the level functions (levelOne(), levelTwo(), ...) */
int numEnemies;
/** The maximum number of attackers at a time for the current level, set in the level functions (levelOne(), levelTwo(), ...) */
int numAttackers;
void makeLevel(Game *gameIn)
{
    levelCounter = 0;
    game = &(*gameIn);
    // * Place Player
    player = constructShip();
    player->shipType = PLAYER;
    player->cords->col = PLAYER_START_COL;
    player->cords->row = PLAYER_START_ROW;
    player->isActive = 1;
    // * Set up Missiles
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        missiles[i] = constructShip();
        missiles[i]->shipType = MISSILE;
        missiles[i]->route.activity = RETURNING_HOME;
        missiles[i]->isActive = 0;
    }
    // Set the float path for floatTracker and by extension, all ships that float
    // * Set Up Enemies
    switch (game->level)
    {
    case 1:
        levelOne();
        break;
    case 2:
        levelTwo();

        break;
    case 3:
        levelThree();
        break;
    case 4:
        levelFour();
        break;
    case 5:
        levelFive();
        break;
    default: // 6
        levelSix();
        break;
    }
    // Set home for all ships
    for (int i = 0; i < numEnemies; i++)
    {
        enemies[i]->home = *enemies[i]->cords;
    }
    player->home = *player->cords;
}

/**
 * Frees all of the memory allocated during makeLevel
 */
void deconstructLevel(void)
{
    // * Free player
    deconstructShip(player);
    // * Free missiles
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        deconstructShip(missiles[i]);
    }
    // * Free enemies
    for (int i = 0; i < numEnemies; i++)
    {
        deconstructShip(enemies[i]);
    }
}

void drawLevel(void)
{
    waitForVBlank();
    drawImageDMA(0, 0, LEVELBACKGROUND_WIDTH, LEVELBACKGROUND_HEIGHT, levelBackground);
    drawRectDMA(0, GAME_WIDTH, WIDTH - GAME_WIDTH, HEIGHT, BLACK);
    // Draw Extra Lives
    for (int i = 0; i < game->lives - 1; i++)
        DRAW_EXTRA_LIFE(i);
    // Draw Enemy Ships
    for (int i = 0; i < numEnemies; i++)
    {
        drawShip(enemies[i], UP);
    }
    // Draw Player
    drawShip(player, UP);

    // Draw Side Panel
    drawSidePanel(game);
}
// Displays leve land score
void drawSidePanel(Game *game)
{
    UNUSED(game);
    drawRectDMA(0, GAME_WIDTH, WIDTH - GAME_WIDTH, HEIGHT, BLACK);
    char lev[2];
    lev[0] = game->level + 48;
    lev[1] = ' ';
    drawCenteredString(20, GAME_WIDTH, 40, 10, "Level", WHITE);
    drawCenteredString(30, GAME_WIDTH + 7, 33, 20, &lev[0], RED);
    drawCenteredString(50, GAME_WIDTH, 40, 10, "Score", WHITE);
    char *score = intToString(game->score);
    drawCenteredString(60, GAME_WIDTH, 40, 20, score, RED);
}
// /** Sets the route of a ship to float around thier home Cords
//  * @param ship The ship
//  */
// void makeFloatPath(Cords floatRadius)
// {
//     floatTracker->route.pathLength = 2;
//     floatTracker->route.path = (Cords **)malloc(sizeof(Cords) * floatTracker->route.pathLength);
//     floatTracker->route.activity = FLOATING;
//     floatTracker->shipType = NONE;
//     floatTracker->route.currentStep = 0;
//     Cords **path = floatTracker->route.path;
//     path[0]->col = floatRadius.col;
//     path[0]->row = 0;
//     path[1]->col = -1 * floatRadius.col;
//     path[0]->row = 0;
// }

/**
 * @return Cords that store the vertical and horizontal spacing between enemy ships. This is used to calculate float path for enemy ships
 * */
void levelOne(void)
{

    numEnemies = 10;
    enemies = ((Ship **)malloc(sizeof(Ship *) * numEnemies));
    numAttackers = 2;
    int spacing = GAME_WIDTH / (numEnemies / 2) - SHIP_WIDTH;
    Cords floatRadius;
    //SHIP_HEIGHT / 5;
    int topLeftCol = getWidth(player) + SHIP_WIDTH * 2; // 30
    int botLeftCol = 5 * SHIP_WIDTH;                    // 50
    // spacing is 200 / 5 - 10 = 40 - 10 = 30
    // Left most ship is also the topMost ship at (x, y) = (30, 20)
    // Right most ship is also botMost ship at (x, y) = (170, 40)
    // Horizontal freedom of movenemt is 20
    // Vertical freedom of movement is 20
    floatRadius.col = 20;
    floatRadiusX = 20;
    floatRadius.row = 0; //20;
    UNUSED(floatRadius);
    for (int i = 0, n = numEnemies / 2; i < numEnemies / 2; i++, n++)
    {
        enemies[i] = constructShip();
        enemies[n] = constructShip();
        enemies[i]->route.activity = FLOATING;
        enemies[n]->route.activity = FLOATING;
        enemies[i]->isActive = 1;
        enemies[n]->isActive = 1;
        enemies[i]->cords->col = i * spacing + topLeftCol;
        enemies[n]->cords->col = i * spacing + botLeftCol;
        enemies[i]->shipType = getRandomEnemy();
        enemies[n]->shipType = getRandomEnemy();
        enemies[i]->cords->row = 2 * getHeight(enemies[i]);
        enemies[n]->cords->row = 4 * getHeight(enemies[n]);
    }
}
// TODO
void levelTwo(void)
{
}
// TODO
void levelThree(void)
{
}
// TODO
void levelFour(void)
{
}
// TODO
void levelFive(void)
{
}
// TODO
void levelSix(void)
{
}

char *intToString(int num)
{

    if (num == 0)
    {
        char *str = (char *)malloc(sizeof(char *) * 2);
        str[0] = '0';
        str[1] = 0;
        return str;
    }
    // int numIndecies = 2;
    // int pow = 1;
    // while (pow * 10 < num)
    // {
    //     pow *= 10;
    //     numIndecies++;
    // }
    char *str = (char *)malloc(sizeof(char *) * 10);
    int i = 0;
    while (num > 0)
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    }

    char *res = (char *)malloc(sizeof(char *) * (i));
    i--;
    for (int n = 0, e = i; n < i; n++, e--)
    {
        res[n] = str[e];
    }
    free(str);
    return res;
}
