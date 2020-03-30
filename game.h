#ifndef GAME_H
#define GAME_H

#include "gba.h"
// * This is where all the settings for the game are defined. In one place
// The number of steps in a route
#define POINTS_PER_KILL 100
#define NUM_LEVELS 6
#define NUM_LIVES 3
#define MAX_ENEMIES 30
#define MAX_MISSILES 5
#define DELAY_TIME 2
#define ROUTE_COMPLEXITY 10
#define EXPLOSION_FRAMES 4
#define CORDS_EQUAL(c1, c2) (((c1).col == (c2).col) && ((c1).row == (c2).row))
typedef enum {
  START,
  NEW_GAME,
  NEW_LEVEL,
  PLAY,
  WIN,
  LOSE
} GBAState;
typedef struct gameData {
        int lives;
        int level;
        int score;
} Game;
typedef struct cords {
        int row;
        int col;
} Cords;
typedef enum directions {
    UP, DOWN, LEFT, RIGHT,
    UL, UR, DL, DR, NEUTRAL
} Direction;
typedef enum shipType {
    BLUE_ANT,
    RED_ANT,
    BLUE_FLY,
    RED_FLY,
    MISSILE,
    PLAYER,
    NONE
} ShipType;
// Defines what the ship is doing
typedef enum shipActivity {
        FLOATING, // Floating around its home
        ATTACKRUN, // Doing and attack run
        CONTROLLING, // Player Control
        RETRUNING_HOME, // For ships after attack run and player when getting new life and missiles
        EXPLODING
} Activity;
/** This is everything having to do with a ships actions */
typedef struct route {
        Cords **path; // The last stop in all enemy ship routes is its home
        // The index of the current target in path
        int currentStep;
        // If positive, the s
        int exploding;
        int pathLength;
        Activity activity;
} Route;
typedef struct ship {
    Cords cords;
    Cords home;
    ShipType shipType;
    Direction direction;
    Route route;
    int isActive;
} Ship;
Game *getGame(void);
// counts the number of loops through the state machine, also used to make the distribution of enemy types random.
extern int count;
// A random number that changes frequently, and the random seed changes based on what count is when the player takes certain actions
extern int random;

void delay(int delayTime);
#endif
