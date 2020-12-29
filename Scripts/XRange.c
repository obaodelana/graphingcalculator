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
    GetYValues();
}

void ChangeTextValues(void)
{
    // Box of text
    Rectangle value1 = (Rectangle) {.x = text1.x, .y = text1.y, 50, 50};
    Rectangle value2 = (Rectangle) {.x = text2.x, .y = text2.y, 50, 50};
    // Box of pointer
    Rectangle mouse = (Rectangle) {.x = GetMousePosition().x, .y = GetMousePosition().y, 10, 10};

    int scrollWheel = GetMouseWheelMove();

    // If mouse colliding with x1 and scroll wheel is scrolled
    if (CheckCollisionRecs(value1, mouse) && scrollWheel)
    {
        // Increase x value
        x1 += scrollWheel;
        
        // Make sure x1 is less than x2
        if (x1 >= x2)
            x1 = x2 - 1;
        // Limit its value
        LimitValue(&x1, -50, 50);
    }

    // If mouse colliding with x2 and scroll wheel is scrolled
    else if (CheckCollisionRecs(value2, mouse) && scrollWheel)
    {
        // Increase x value
        x2 += scrollWheel;

        // Make sure x2 is greater than x1
        if (x2 <= x1)
            x2 = x1 + 1;
        // Limit its value
        LimitValue(&x2, -50, 50);
    }
}

// Draw text
void DrawXs(void)
{
    DrawText(TextFormat("x1: %i", x1), text1.x, text1.y, 20, GRAY);
    DrawText(TextFormat("x2: %i", x2), text2.x, text2.y, 20, GRAY);
}