#ifndef UTIL_H
#define UTIL_H
#include "game.h"

char *directionToString(Direction direction);
void itoa(int num, char *buffer);
char *shipTypeToString(ShipType ship);
char *activityToString(Activity activity);
int strLen(char *str);
void strCpy(char *src, char *dst, int sourceStart, int destStart, int length);
char *cordsToString(Cords *cords);
char *concat(char *str1, char *str2);
void logError(char *error);
void report(char *log);
void logShipData(Ship *ship);
extern int errorLog;
extern int normalLog;
#endif