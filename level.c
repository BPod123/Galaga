#include "level.h"
#include "ship.h"
#include "images/levelBackground.h"
#include "images/playerShip_Up.h"
#include <stdlib.h>
#define EXTRA_LIFE_COL(i) (SHIP_WIDTH + SHIP_WIDTH * (i))
#define EXTRA_LIFE_ROW (HEIGHT - SHIP_WIDTH)
#define DRAW_EXTRA_LIFE(i) drawImageDMA(EXTRA_LIFE_ROW, EXTRA_LIFE_COL(i), PLAYERSHIP_UP_WIDTH, PLAYERSHIP_UP_HEIGHT, playerShip_Up);
// * These methods set up the field for each level
void levelOne(void);
void levelTwo(void);
void levelThree(void);
void levelFour(void);
void levelFive(void);
void levelSix(void);
Ship *enemies[MAX_ENEMIES];
Ship *player;
Game *game;
void makeLevel(Game *gameIn) {
    game = &(*gameIn);
    // * Place Player
    player = malloc(sizeof(Ship));
    player->cords.col = GAME_WIDTH / 2;
    player->cords.row = HEIGHT - 3 * getHeight(player);
    player->route.isActive = 1;
    // * Place Enemies
    switch(game->level) {
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
void levelOne(void) {
    int numEnemies = 10;
    int topSpacing = GAME_WIDTH / numEnemies;
    for (int i = 0, n = numEnemies / 2; i < numEnemies / 2; i++) { 
       enemies[i] = malloc(sizeof(Ship));
       enemies[n] = malloc(sizeof(Ship));
       enemies[i]->route.isActive = 1;
       enemies[n]->route.isActive = 1;
       enemies[i]->cords.col = i * topSpacing;
       enemies[n]->cords.col = i * topSpacing + topSpacing / 2;
       enemies[i]->shipType = getRandomEnemy();
       enemies[n]->shipType = getRandomEnemy();
       enemies[i]->cords.row = 2 * getHeight(enemies[i]);
       enemies[n]->cords.row = 3 * getHeight(enemies[n]);
    }
}
void levelTwo(void) {}
void levelThree(void) {}
void levelFour(void) {}
void levelFive(void) {}
void levelSix(void) {}
void drawLevel(void) {
    waitForVBlank();
    drawImageDMA(0, 0, LEVELBACKGROUND_WIDTH, LEVELBACKGROUND_HEIGHT, levelBackground);
    drawRectDMA(0, GAME_WIDTH, WIDTH - GAME_WIDTH, HEIGHT, BLACK);
    for (int i = 0; i < game->lives - 1; i++)
        DRAW_EXTRA_LIFE(i);
}