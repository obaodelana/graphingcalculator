#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "raylib.h"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define screenWidth 1100
#define screenHeight 600

// Size of graph boxes
#define boxSize 10

// minimum x value, maximum x value
extern int x1, x2;

// Textfield class
void TypeText(void);
void DrawTextbox(void);
void SendEquation(void);

// Equation class
void StripEquation(char *equationText);
int ProcessEquation(float *yVals, int startX, int endX);

// XRange class
void DrawXs(void);
void ChangeTextValues(void);

// Graph class
void GetYValues(void);
void ClearIndex(void);
void PlotGraph(void);