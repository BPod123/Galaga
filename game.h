#ifndef GAME_H
#define GAME_H

#include "gba.h"
// * This is where all the settings for the game are defined. In one place
// The number of steps in a route
#define POINTS_PER_KILL 100
#define INSTRUCTION_ERROR_PERCENT 30
#define NUM_LEVELS 6
#define NUM_LIVES 3
#define MAX_ENEMIES 30
#define MAX_MISSILES 10
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
  LOSE,
  RUN_TEST
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
        RETURNING_HOME, // For ships after attack run and player when getting new life and missiles
        EXPLODING
} Activity;
/** Holds a command for a ship to follow
 * Either in a target to move to, or a set of directions */
struct instruction;
typedef struct instruction {
        /** if true, then the ship will be guidted to the target, else it will make movements in the directions */
        int gotoTarget;
        /** The address of the target to route to */
        Cords *target;
        /** The current index in the array of directions to move to */
        int currentDirection;
        /** The number of direcitons in the array of directions */
        int numDirections;
        /** An array of directions for a ship to move in */
        Direction **directions;
        /** If the instruction is more complex and can have both targets to goto and directions to follow */
        struct instruction *nextPart;
        /** If true, then the next part of the instruction will be executed when this one is done */
        int hasNextPart;
} Instruction;
/** This is everything having to do with a ships actions */
typedef struct route {
        Instruction **path; // The last stop in all enemy ship routes is its home
        // The index of the current target in path
        int currentStep;
        // If positive, the s
        int exploding;
        int pathLength;
        Activity activity;
} Route;
typedef struct ship {
    Cords *cords;
    Cords home;
    ShipType shipType;
    Direction *direction;
    Route route;
    int isActive;
} Ship;
// counts the number of loops through the state machine, also used to make the distribution of enemy types random.
extern int count;
// A random number that changes frequently, and the random seed changes based on what count is when the player takes certain actions
extern int random;

void delay(int delayTime);
Game *getGame(void);
#endif
