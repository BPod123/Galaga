#include "testing.h"
#include "game.h"
#include "gba.h"
#include "images/titleScreen.h"
void runTest(void) {
    waitForVBlank();
    //setPixel(1, 1, WHITE);
    drawFullScreenImageDMA(titleScreen);
    while(1);
}