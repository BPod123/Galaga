#include "game.h"
#include "playState.h"
#include <stdio.h>
#include <stdlib.h>
#include "images/winImage.h"
#include "gba.h"
#include "level.h"
#include "images/titleScreen.h"
#include "testing.h"
void runStartState(void);
void runNewLevelState(Game *game);
void runPlayState(Game *game, u32 currentButtons, u32 previousButtons);
void runLoseState(void);
void runWinState(void);
GBAState selectState(GBAState state, Game *game, u32 currentButtons, u32 previousButtons);

// Counts the  number of iterations of the loop in main
int count = 0;
// Changes with every iteration of main and is used to generate random numbers. The seed for this changes based on the value of count when the user takes certain actions
int random = 0;
int main(void)
{
  /* TODO: */ // DONE
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;
  // ! Set Starting state
  GBAState state = START;//RUN_TEST;//START;
  Game *game = malloc(sizeof(sizeof(Game)));
  srand(count);
  while (1)
  {
    count++;
    random = rand();

    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state)
    {
    case START:
      runStartState();
      // state = ?
      break;
    case NEW_GAME:
    game->level = 1;
    game->score = 0;
    game->lives = NUM_LIVES;
    case NEW_LEVEL: {
      // This helps with randomizing the ships because it is bases on when the user does stuff, taking advantage of their unpredictability.
      srand(random);
      random = rand();
      runNewLevelState(game);
      state = PLAY;
      continue;
    }
    case PLAY:
      runPlayState(game, currentButtons, previousButtons);
      break;
    case WIN:
      runWinState();
      break;
    case LOSE:
      runLoseState();
      break;
    case RUN_TEST:
      runTest();
    }
    delay(DELAY_TIME);
    state = selectState(state, game, currentButtons, previousButtons);
    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
GBAState selectState(GBAState state, Game *game, u32 currentButtons, u32 previousButtons)
{
  switch (state)
  {
  case START:
  {
    // if (KEY_DOWN_NOW(BUTTON_START)) {
    //   while(KEY_DOWN_NOW(KEY_DOWN_NOW(BUTTON_START)));
    //   return NEW_LEVEL;
    // }
    if (KEY_DOWN(BUTTON_START, currentButtons) && ! (KEY_DOWN(BUTTON_START,previousButtons)))
      return NEW_GAME;
    else return START;
  }
  case NEW_LEVEL:
    return PLAY;

  case PLAY:
    if (game->lives == 0)
      return LOSE;
    else if (game->level == NUM_LEVELS + 1)
      return WIN;
    else if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
      return START;
    else
      return PLAY;

  case WIN:
    if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
      return START;

  case LOSE: // LOSE
    if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
      return START;
    else
      return LOSE;
  default:
    return RUN_TEST;
  }
}
void runStartState(void)
{
  waitForVBlank();
  drawFullScreenImageDMA(titleScreen);
}
void runWinState(void)
{
  waitForVBlank();
  drawImageDMA(0, 0, GAME_WIDTH, HEIGHT, winImage);
}
void runLoseState(void)
{
  waitForVBlank();
  drawRectDMA(0, 0, GAME_WIDTH, HEIGHT, BLACK);
  drawString(0, 0, "You died. The aliens recovered your corpse in space to do experiments,", WHITE);
  drawString(10, 0, "but unknowingly contracted the coronavirus from you. They spread it to their civilization", WHITE);
  drawString(20, 0, "and to all other life in the universe which eventually sucumbed to it. Trillions died", WHITE);
  drawString(30, 0, "and it was all because you couldn't destroy a few unarmed ships.", WHITE);
}
void runNewLevelState(Game *game)
{
  makeLevel(game);
  drawLevel();
}
/** Delays the game
 * @param delayTime the amount of time you want to delay the game in tenths of a second */
void delay(int delayTime)
{
  volatile int x = 0;
  for (int i = 0; i < delayTime * 8000; i++)
    x++;
}
