#include "util.h"
#include <stdlib.h>
#include <stdio.h>
int errorLog = 0;
int normalLog = 0;
char *directionToString(Direction direction)
{
    char *str;
    switch (direction)
    {
    case UP:
        str = "UP";
        break;
    case DOWN:
        str = "DOWN";
        break;
    case LEFT:
        str = "LEFT";
        break;
    case RIGHT:
        str = "RIGHT";
        break;
    case UL:
        str = "UL";
        break;
    case DL:
        str = "DL";
        break;
    case UR:
        str = "UR";
        break;
    case DR:
        str = "DR";
        break;
    default: // NEUTRAL
        str = "NEUTRAL";
        break;
    }
    return str;
}
/** Because the library function wouldn't work, I wrote my own that only works for non-negative numbers
 * @param num The integer to be converted to a string
 * @param buffer The char array to store the string in */
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

char *shipTypeToString(ShipType ship)
{
    switch (ship)
    {
    case BLUE_ANT:
        return "BLUE_ANT";
    case BLUE_FLY:
        return "BLUE_FLY";
    case RED_ANT:
        return "RED_ANT";
    case RED_FLY:
        return "RED_FLY";
    case MISSILE:
        return "MISSILE";
    case PLAYER:
        return "PLAYER";
    default: // NONE
        return "NONE";
    }
}
char *activityToString(Activity activity)
{
    switch (activity)
    {
    case FLOATING:
        return "FLOATING";
    case ATTACKRUN:
        return "ATTACKRUN";
    default: //CONTROLLING
        return "CONTROLLING";
    }
}
int strLen(char *str)
{
    int i = 0;
    for (int i = 0; str[i] != 0; i++)
    {
    }
    return i;
}
char *cordsToString(Cords *cords)
{
    char col[3];
    itoa(cords->col, col);
    char row[3];
    itoa(cords->row, row);
    char str[10];
    str[0] = '(';
    int i;
    for (i = 2; i - 2 < strLen(col); i++)
    {
        str[i] = col[i - 2];
    }
    str[i++] = ',';
    str[i++] = ' ';
    for (int n = 0; n < strLen(row); n++, i++)
    {
        str[i] = row[n];
    }
    str[i++] = ')';
    str[i++] = 0;
    return concat(str, "");
}
void strCpy(char *src, char *dst, int sourceStart, int destStart, int length)
{
    for (int i = 0; i < length; i++, sourceStart++, destStart++)
    {
        dst[destStart + i] = src[sourceStart + i];
    }
}
char *concat(char *str1, char *str2)
{
    if (strLen(str1) + strLen(str2) + 1 > 20)
    {
        if (strLen(str1) + strLen(str2) + 1 > 40)
        {
            char *res = (char *)malloc(80 * sizeof(char));
            strCpy(str1, res, 0, 0, strLen(str1));
            strCpy(str2, res, 0, strLen(str1) + 1, strLen(str2) + 1);
            return res;
        }
        char *res = (char *)malloc(40 * sizeof(char));
        strCpy(str1, res, 0, 0, strLen(str1));
        strCpy(str2, res, 0, strLen(str1) + 1, strLen(str2) + 1);
        return res;
    }
    char *res = (char *)malloc(20 * sizeof(char));
    strCpy(str1, res, 0, 0, strLen(str1));
    strCpy(str2, res, 0, strLen(str1) + 1, strLen(str2) + 1);
    return res;
}
void logError(char *error)
{
    FILE *file;
    if (errorLog == 0)
    {
        file = fopen("logs/errorlog.txt", "w+");
    }
    else
    {
        file = fopen("logs/errorlog.txt", "a");
    }
    fprintf(file, "\n%s", error);
    fclose(file);
    errorLog++;
}
void report(char *log)
{
    FILE *file;
    if (normalLog == 0)
    {
        file = fopen("logs/log.txt", "w+");
    }
    else
    {
        file = fopen("logs/log.txt", "a");
    }
    fprintf(file, "\n%s", log);
    fclose(file);
    normalLog++;
}
void logShipData(Ship *ship)
{
    char *cords = cordsToString(&ship->cords);
    char cordsString[17];
    strCpy("Cords: ", cordsString, 0, 0, 7);
    strCpy(cords, cordsString, 0, 8, strLen(cords) + 1);
    char *shipType = concat("Ship Type: ", shipTypeToString(ship->shipType));
    char *direction = concat("Direction: ", directionToString(ship->direction));
    char *isActive = concat("Is Active: ", ship->isActive == 0 ? "false" : "true");
    char csString[2];
    itoa(ship->route.currentStep, csString);
    char *currentStep = concat("Current Step: ", csString);
    char *path = "Path: ";
    char *pathStrings[ROUTE_COMPLEXITY + 1];
    for (int i = 0; i < ROUTE_COMPLEXITY; i++)
    {
        Cords c;
        c.col = ship->route.path[i]->col;
        c.row = ship->route.path[i]->row;
        pathStrings[i] = cordsToString(&c);
        path = concat(path, pathStrings[i]);
        path = concat(path, ", ");
    }
    Cords c;
    c.col = ship->route.path[ROUTE_COMPLEXITY]->col;
    c.row = ship->route.path[ROUTE_COMPLEXITY]->row;
    path = concat(path, cordsToString(&c));

    char *exploding;
    char ex[2];
    itoa(ship->route.exploding, ex);
    exploding = concat("Exploding: ", ex);
    char *activity = activityToString(ship->route.activity);
    //char *temp = (char *)malloc(10 * sizeof(char));
    //char *str = concat("Log: ", itoa(normalLog, temp));
    //report(str);
    report(cordsString);
    report(shipType);
    report(direction);
    report(isActive);
    report(currentStep);
    report(path);
    report(exploding);
    report(activity);
    report("");
}