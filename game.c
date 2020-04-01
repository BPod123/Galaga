#include "game.h"
#include "playState.h"
#include <stdio.h>
#include <stdlib.h>
#include "images/winImage.h"
#include "gba.h"
#include "level.h"
#include "images/titleScreen.h"
#include "testing.h"
#include "images/loseImage.h"
void runStartState(void);
void runNewLevelState(void);
void runPlayState(u32 currentButtons, u32 previousButtons);
void runLoseState(void);
void runWinState(void);
GBAState selectState(GBAState state, Game *game, u32 currentButtons, u32 previousButtons);

// Counts the  number of iterations of the loop in main
int count = 0;
// Changes with every iteration of main and is used to generate random numbers. The seed for this changes based on the value of count when the user takes certain actions
int random = 0;
Game *game;
GBAState state = START; //RUN_TEST;//START;
int main(void)
{
  /* TODO: */ // DONE
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  srand(count);
  random = rand();
  while (1)
  {
    GBAState currState = state;
    count++;
    // This helps with randomizing the ships because it is bases on when the user does stuff, taking advantage of their unpredictability.
    srand(random);
    random = rand();

    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    switch (state)
    {
    case START:
      if (game)
        free(game);
      runStartState();
      // state = ?
      break;
    case NEW_GAME:
    {
      game = (Game *)(calloc(1, sizeof(Game)));
      game->level = 1;
      game->score = 0;
      game->lives = NUM_LIVES;
    }
    case NEW_LEVEL:
    {
      runNewLevelState();
      state = PLAY;
      continue;
    }
    case PLAY:
      runPlayState(currentButtons, previousButtons);
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
    if (currState != state)
      continue;
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
    if (KEY_DOWN(BUTTON_START, currentButtons) && !(KEY_DOWN(BUTTON_START, previousButtons)))
      return NEW_GAME;
    else
      return START;
  }
  case NEW_LEVEL:
    return PLAY;

  case PLAY:
    if (game->lives == 0)
      return LOSE;
    else if (game->level == NUM_LEVELS + 1)
      return WIN;
    else if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
      return START;
      deconstructLevel();
    }
    else
      return PLAY;

  case WIN:
    if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
      deconstructLevel();
      return START;
    }
    else 
    return WIN;

  case LOSE: // LOSE
    if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
      deconstructLevel();
      return START;
    }
    else
      return LOSE;

  case NEW_GAME:
    return NEW_LEVEL;
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
  drawFullScreenImageDMA(winImage);
}
void runLoseState(void)
{
  waitForVBlank();
  drawFullScreenImageDMA(LoseImage);
  char *score = intToString(getGame()->score, (char *)calloc(5, sizeof(char)));
  char *message = "Your score was ";
  realloc(message, sizeof(score) / sizeof(char) + 1);
  for (int i = 15, j = 0; (unsigned int)i < sizeof(score) / sizeof(char); i++, j++)
    message[i] = score[j];
  drawCenteredString((HEIGHT - 10) / 2, 0, WIDTH, 10, message, GREEN);
}
void runNewLevelState(void)
{
  waitForVBlank();
  makeLevel();
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
Game *getGame(void)
{
  return game;
}
GBAState *getState(void)
{
  return &state;
}
