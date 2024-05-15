#include <iostream>
#include <raylib.h>
#include <array>
#include <random>

using std::array, std::cout, std::endl;
constexpr int screenWidth = 900, screenHeight = 900, unitSize = 15, guiPadding = 40;
constexpr int column = screenWidth / unitSize, row = (screenHeight - guiPadding) / unitSize;

array<char, row * column> buffer, tempBuffer;
bool start = false;

void initializeWindow();
void drawBoard();
void update();
void processInput();
bool checkNeighbours(int x, int y);

int main()
{
    initializeWindow();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        drawBoard();
        if (start)
            update();
        processInput();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initializeWindow()
{
    InitWindow(screenHeight, screenWidth, "GAME OF LIFE");
    SetTargetFPS(24);

    std::default_random_engine random_engine(std::random_device{}());

    for (int i = 0; i < column * row; i++)
    {
        std::uniform_int_distribution<int> num(1, 100);
        buffer.at(i) = num(random_engine) % 2;
        // buffer.at(i) = 0;
    }
}

void drawBoard()
{
    array<Color, 2> cellColor = {GRAY, YELLOW};
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            char currentCellValue = buffer.at(j + i * column);
            if (currentCellValue < 2)
                DrawRectangle(j * unitSize, i * unitSize, unitSize - 1, unitSize - 1, cellColor[currentCellValue]);
            else
            {
                Color highlighedColor = cellColor[currentCellValue - 2];
                highlighedColor.a *= 0.8;
                DrawRectangle(j * unitSize, i * unitSize, unitSize - 1, unitSize - 1, WHITE);
                DrawRectangle(j * unitSize, i * unitSize, unitSize - 1, unitSize - 1, highlighedColor);
                buffer.at(j + i * column) -= 2;
            }
        }
    }
    DrawText("PRESS X TO START/STOP THE ANIMATION", guiPadding * 6, screenHeight - guiPadding / 1.5, 20, RED);
}

// returns true if the cell survives else false
bool checkNeighbours(int x, int y)
{
    int count = 0;
    bool currentCellAlive;
    if(tempBuffer.at(x + y * column) == 1||tempBuffer.at(x + y * column)-2==1)
        currentCellAlive=true;
    else
        currentCellAlive=false;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int xCheckIndex = x + j, yCheckIndex = y + i;
            if (xCheckIndex == x && yCheckIndex == y)
                continue;
            if (xCheckIndex < 0)
                xCheckIndex = column - 1;
            else if (xCheckIndex >= column)
                xCheckIndex = 0;
            if (yCheckIndex < 0)
                yCheckIndex = row - 1;
            else if (yCheckIndex >= row)
                yCheckIndex = 0;

            if (tempBuffer.at(xCheckIndex + yCheckIndex * column) == 1 || tempBuffer.at(xCheckIndex + yCheckIndex * column) - 2 == 1)
                count++;
        }
    }
    if (currentCellAlive)
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if(count==3)
            return true;
        else
            return false;
    }
}

void update()
{
    tempBuffer = buffer;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            // cout<<(int)tempBuffer.at(j+i*column)<<"-"<<(int)buffer.at(j+i*column)<<" ";
            if (checkNeighbours(j, i))
                buffer.at(j + i * column) = 1;
            else
                buffer.at(j + i * column) = 0;
        }
        cout << endl;
    }
}

void processInput()
{
    Vector2 cellHighlightPosition = GetMousePosition();
    cellHighlightPosition = {(cellHighlightPosition.x) / unitSize, (cellHighlightPosition.y) / unitSize};

    if (cellHighlightPosition.x < column && cellHighlightPosition.y < row && cellHighlightPosition.x >= 0 && cellHighlightPosition.y >= 0)
        buffer.at((int)cellHighlightPosition.x + (int)cellHighlightPosition.y * column) += 2;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (cellHighlightPosition.x < column && cellHighlightPosition.y < row && cellHighlightPosition.x >= 0 && cellHighlightPosition.y >= 0)
            buffer.at((int)cellHighlightPosition.x + (int)cellHighlightPosition.y * column) = 1;
    }
    if (IsKeyPressed(KEY_X))
        start = !start;
}