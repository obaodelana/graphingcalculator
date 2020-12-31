#include "../Headers/Global.h"

// Position for x1 and x2 text
Vector2 text1 = (Vector2) {76, 27};
Vector2 text2 = (Vector2) {screenWidth - 152, 27};

int x1 = -10, x2 = 10;

// Limit x1 and x2 values to specified values
void LimitValue(int *value, int min, int max)
{
    if (*value < min)
        *value = min;
    else if (*value > max)
        *value = max;

    // Update graph
    SendEquation();
}

void ChangeTextValues(void)
{
    int keyPressed = GetKeyPressed();
    // If a key is pressed
    if (keyPressed == KEY_UP || keyPressed == KEY_DOWN || keyPressed == KEY_RIGHT || keyPressed == KEY_LEFT)
    {
        // Up arrow
        if (keyPressed == KEY_UP)
            x1++;
        // Down arrow
        else if (keyPressed == KEY_DOWN)
            x1--;
        // Right arrow
        else if (keyPressed == KEY_RIGHT)
            x2++;
        // Left arrow
        else if (keyPressed == KEY_LEFT)
            x2--;

        // Make sure x1 is less than x2
        if (x1 >= x2)
            x1 = x2 - 1;
        // Make sure x2 is greater than x1
        else if (x2 <= x1)
            x2 = x1 + 1;

        // Limit their values
        LimitValue(&x1, -50, 50);
        LimitValue(&x2, -50, 50);
    }
}

// Draw text
void DrawXs(void)
{
    DrawText(TextFormat("x1: %i\nUp/down arrows", x1), text1.x, text1.y, 20, GRAY);
    DrawText(TextFormat("x2: %i\nLeft/right arrows", x2), text2.x, text2.y, 20, GRAY);
}