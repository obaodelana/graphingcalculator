#include "../Headers/Global.h"

float midPointX = screenWidth / 2, midPointY = screenHeight / 2;
// Number of boxes to draw on the x and y axis
int numberXBox = screenWidth / boxSize, numberYBox = screenHeight / boxSize; 

Vector2 startMousePos = (Vector2) {screenWidth / 2, screenHeight / 2};
// Keeps track of changes in position made when panning
Vector2 changeInPos = (Vector2) {0.0f, 0.0f};
// Camera for panning
Camera2D graphCam = {0};

typedef struct Boundary
{
    float minX, maxX;
    float minY, maxY;
} Boundary;

void UpdateDrawFrame(void);
void DragGraph();
void DrawGraphPaper(void);
void DrawInfoText(void);

int main()
{
    InitWindow(screenWidth, screenHeight, "Raylib Graphing Calculator");
    // Set up panning camera
    graphCam = (Camera2D) {.target = startMousePos, .offset = startMousePos, .rotation = 0.0f, .zoom = 1.0f};

    // Web compilation
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
    #else
    // Normal compilation
        SetTargetFPS(60);
        // Main game loop
        while(!WindowShouldClose())
        {
            UpdateDrawFrame();
        }
    #endif

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void)
{
    TypeText();
    DragGraph();

    BeginDrawing();

        ClearBackground(WHITE);

        // Camera mode
        BeginMode2D(graphCam);
            DrawGraphPaper();
            PlotGraph();
        EndMode2D();

        DrawInfoText();
        DrawTextbox();

    EndDrawing();
}

void LimitGraph(Vector2 *vecs[], Boundary bounds[])
{
    // Go through each vector
    for (int i = 0; vecs[i] != NULL; i++)
    {
        // Limit movement on x and y axis using specified boundary
        if (vecs[i]->x <= bounds[i].minX)
            vecs[i]->x = bounds[i].minX;
        else if (vecs[i]->x >= bounds[i].maxX)
            vecs[i]->x = bounds[i].maxX;
        else if (vecs[i]->y <= bounds[i].minY)
            vecs[i]->y = bounds[i].minY;
        else if (vecs[i]->y >= bounds[i].maxY)
            vecs[i]->y = bounds[i].maxY;
    }
}

void DragGraph()
{
    // When the left mouse button is clicked, keep track of it's position
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        startMousePos = GetMousePosition();
    }

    // When the left mouse button is held down
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        // Change in position from when clicked to now
        Vector2 dir = (Vector2) {startMousePos.x - GetMousePosition().x, startMousePos.y - GetMousePosition().y};
        // Move camera to new position (dir)
        graphCam.target = (Vector2) {dir.x + graphCam.target.x, dir.y + graphCam.target.y};
        // Kepp track of change in position
        changeInPos = (Vector2) {changeInPos.x + dir.x, changeInPos.y + dir.y};

        Vector2 *vecs[] = { &graphCam.target, &changeInPos, NULL };
        // Boundary for camera and changeInPos
        Boundary bounds[] = {(Boundary) {-925.0f, 2000.0f, -1420.0f, 1988.0f}, (Boundary) {-1475.0f, 1450.0f, -1720.0, 1688.0f}};
        LimitGraph(vecs, bounds);

        // Set start position to current mouse position
        startMousePos = GetMousePosition();
    }

    // When right mouse button is clicked, reset everything
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        changeInPos = (Vector2) {0.0f, 0.0f};
        graphCam.target = (Vector2) {screenWidth / 2, screenHeight / 2};
    }
}

void DrawGraphPaper(void)
{
    // Boxes
    for (int y = 0; y < numberYBox; y++)
    {
        for (int x = 0; x < numberXBox; x++)
        {
            // Draw boxes and add change in pos to make sure it moves with player's mouse pos
            DrawRectangleLines(x * boxSize + changeInPos.x, y * boxSize + changeInPos.y, boxSize, boxSize, RAYWHITE);
        }
    }
    // Draw vertical line and add change in pos to make sure it moves with player's mouse pos
    DrawLine(screenWidth / 2, changeInPos.y, screenWidth / 2, screenHeight + changeInPos.y, GRAY);
    // Draw horizontal line and add change in pos to make sure it moves with player's mouse pos
    DrawLine(changeInPos.x, screenHeight / 2, screenWidth + changeInPos.x, screenHeight / 2, GRAY);

     // Draw text markers
    for (int i = -200; i <= 200; i += 5)
    {
        // Don't draw text when at 0
        if (i == 0) continue;

        // Get the string form of the number
        const char *text = TextFormat("%i",i);
        // Get the width of the text
        int textSpace = MeasureText(text, 10);

        // Draw x text markers with spacing
        DrawText(text, (midPointX - textSpace) + (i * boxSize), midPointY, 10, GRAY);
        // Draw y text markers with spacing
        DrawText(text, midPointX, (midPointY - textSpace) - (i * boxSize), 10, GRAY);
    }
}

// Displays all functions
void DrawInfoText(void)
{
    DrawText("s - sin\nc - cos\nt - tan\nas - asin\nac - acos\nat - atan\nr - sqrt\nab - abs\nlo - log\nln - ln\np - pi\ne - e\n# - rand\n^ - exponnents\n! - factorial", 
        screenWidth - 100, screenHeight - 280, 13, DARKGRAY);
}
