#include "level.h"
#include "ship.h"
#include "images/levelBackground.h"
#include "images/playerShip_Up.h"
#include <stdlib.h>
#define EXTRA_LIFE_COL(i) (SHIP_WIDTH + SHIP_WIDTH * (i))
#define EXTRA_LIFE_ROW (HEIGHT - SHIP_WIDTH)
#define DRAW_EXTRA_LIFE(i) drawImageDMA(EXTRA_LIFE_ROW, EXTRA_LIFE_COL(i), SHIP_WIDTH, SHIP_HEIGHT, playerShip_Up);

// Because the library function wouldn't work, I wrote my own that only works for non-negative numbers
void itoa(int num, char *buffer);
// * These methods set up the field for each level
void levelOne(void);
void levelTwo(void);
void levelThree(void);
void levelFour(void);
void levelFive(void);
void levelSix(void);
Ship *enemies[MAX_ENEMIES];
Ship *player;
Ship *missiles[MAX_MISSILES];
Game *game;
int numEnemies;
void makeLevel(Game *gameIn)
{
    game = &(*gameIn);
    // * Place Player
    player = malloc(sizeof(Ship));
    player->shipType = PLAYER;
    player->cords.col = PLAYER_START_COL;
    player->cords.row = PLAYER_START_ROW;
    player->isActive = 1;
    
    // * Set up Missiles
    for (int i = 0; i < MAX_MISSILES; i++) {
        missiles[i]->shipType = MISSILE;
    }

    // * Place Enemies
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
}
void levelOne(void)
{
    numEnemies = 10;
    int spacing = GAME_WIDTH / (numEnemies / 2);
    int topLeftCol = getWidth(player);
    int botLeftCol = topLeftCol * 3;
    for (int i = 0, n = numEnemies / 2; i < numEnemies / 2; i++, n++)
    {
        enemies[i] = malloc(sizeof(Ship));
        enemies[n] = malloc(sizeof(Ship));
        enemies[i]->isActive = 1;
        enemies[n]->isActive = 1;
        enemies[i]->cords.col = i * spacing + topLeftCol;
        enemies[n]->cords.col = i * spacing + botLeftCol;
        enemies[i]->shipType = getRandomEnemy();
        enemies[n]->shipType = getRandomEnemy();
        enemies[i]->cords.row = 2 * getHeight(enemies[i]);
        enemies[n]->cords.row = 4 * getHeight(enemies[n]);
    }
}
void levelTwo(void) {}
void levelThree(void) {}
void levelFour(void) {}
void levelFive(void) {}
void levelSix(void) {}
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
    char lev[2];
    lev[0] = game->level + 48;
    lev[1] = ' ';
    drawCenteredString(20, GAME_WIDTH, 40, 10, "Level", WHITE);
    drawCenteredString(30, GAME_WIDTH + 7, 33, 20, &lev[0], RED);
    drawCenteredString(50, GAME_WIDTH, 40, 10, "Score", WHITE);
    char score[5];
    if (game->score == 0)
    {
        score[0] = '0';
        score[1] = 0;
    }
    else
    {
        itoa(game->score, score);
    }
    drawCenteredString(60, GAME_WIDTH, 40, 20, &score[0], RED);
}
void itoa(int num, char *buffer)
{
    // base case: num == 0
    static int index;
    if (num / 10 > 0)
    {
        itoa(num / 10, buffer);
    }
    buffer[index] = num % 10 + '0';
    index++;
    buffer[index] = 0;
}
