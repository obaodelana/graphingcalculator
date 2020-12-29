// Plots a graph by using lines

#include <math.h>
#include "../Headers/Global.h"

Vector2 points[101];
float yValues[101];

int yIndex = -1;

// Reset all variables
void ClearIndex(void)
{
    for (int i = 0; i < yIndex + 1; i++)
    {
        yValues[i] = 0;
        points[i] = (Vector2) {0.0f, 0.0f};
    }

    yIndex = -1;
}

void GetYValues(void)
{
    // Set y index to length of array and mutate y values array
    yIndex = ProcessEquation(yValues, x1, x2);
    
    // Loop till the end of array and set points array
    for (int i = 0, x = x1; i < yIndex + 1; i++, x++)
    {
        // Get x, multiply it by box size to make it visible and add half of the screen to it, to center it
        int xPos = screenWidth / 2 + (x * boxSize);
        // Get y, multiply it by box size to make it visible and remove half of the screen to it, to center it
        float yPos = screenHeight / 2 - (yValues[i] * boxSize);

        // Set vector array
        points[i] = (Vector2) {xPos, yPos};
    }
}

void PlotGraph(void)
{
    // If array not empty
    if (yIndex >= 0)
    {
        // Loop points array and skip the last point (yIndex has already been decremented when setting it's value)
        for (int i = 0; i < yIndex; i++)
            // Draw a line from the current point to next point
            DrawLineEx(points[i], points[i + 1], 2.0f, BLUE);
    }
}
