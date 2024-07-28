#include <iostream>
#include <raylib.h>
#include <array>

using std::array, std::cout, std::endl;

bool gameOver = false;
Vector2 mouseState = {-1, -1};

struct int2d
{
    int x;
    int y;
};

class inputHandling;
class board
{
private:
    int2d screen;
    int row, column, unitSize;
    Vector2 plotStart, plotEnd, hLineStart, hLineEnd, vLineStart, vLineEnd;
    bool hover,cellSelected;
    inputHandling *input;
    int hoveringIndex;
    array<char, 9 * 9> cellValues;

public:
    board();
    ~board();

    void drawBoard(); // Draws the necessary graphics each frame
    void setup();     // Setup the required raylib window
    int hoverCheck();
};

class inputHandling
{
private:
    char currentKeyPress;
    int2d mouseCursorPosition;
    bool mouseButtonPressed;
    int currentCell;
    bool firstClick;

public:
    inputHandling();
    ~inputHandling();

    int getCurrentKeyPressed();
    int2d getMouseCursorPosition();
    bool getMouseButtonPressed();  
    bool getFirstClick();
    bool setFirstClick();  

    // int cellHover();            // Checks if mouse is within a cell and returns the cell number;
};

board::board()
{
    input = new inputHandling();
    row = 9, column = 9, unitSize = 50; //,hoveringCell = -1;
    screen = {900, 900};
    hover = false;
    cellSelected=false;
    hoveringIndex=-1;

    // x,y coordinates of the leftmost and topmost point from where the graphics starts
    plotStart = {float(screen.x) / 2 - column * unitSize / 2, float(screen.y) / 2 - row * unitSize / 2};

    // x,y coordinates of the rightmost and bottommost point from where the graphics end
    plotEnd = {float(screen.x) / 2 + column * unitSize / 2, float(screen.y) / 2 + row * unitSize / 2};

    hLineStart = plotStart;
    hLineEnd = {plotEnd.x, plotStart.y};

    vLineStart = plotStart;
    vLineEnd = {plotStart.x, plotEnd.y};

    // initialize the board
    for(auto &value:cellValues)    
        value=0;
    
}

void board::drawBoard()
{
    char enteredNumber = input->getCurrentKeyPressed();

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))    
    {        
        if(input->getFirstClick())
            cellSelected=false;            
        
        else
        {
            cellSelected=true;
        }
        
        input->setFirstClick();        
    }               

    Color affectedCells = {226, 235, 243, 255};
    Color affectedCellMain = {68, 114, 202, 128};
    Color selectedCell = {187, 222, 251, 255};

    Rectangle highlightingEffect;

    if(cellSelected)
    {
        if(enteredNumber)
            cellValues.at(hoveringIndex)=enteredNumber;
        cout<<cellValues.at(hoveringIndex)<<endl;
    }        
    else
        hoveringIndex=hoverCheck();
    

    if (hover)
    {
        // Highlighting affected cells
        highlightingEffect = {plotStart.x, plotStart.y + (hoveringIndex / 9) * unitSize, unitSize * 1.0f * column, unitSize * 1.0f};
        DrawRectangleRec(highlightingEffect, affectedCells);

        highlightingEffect = {plotStart.x + (hoveringIndex % 9) * unitSize, plotStart.y, unitSize * 1.0f, unitSize * 1.0f * row};
        DrawRectangleRec(highlightingEffect, affectedCells);

        highlightingEffect = {plotStart.x + ((hoveringIndex % 9) / 3) * unitSize * 3, plotStart.y + (hoveringIndex / 27) * unitSize * 3, unitSize * column / 3.0f, unitSize * row / 3.0f};
        DrawRectangleRec(highlightingEffect, affectedCells);

        // Drawing the hovering cell highlight
        highlightingEffect = {plotStart.x + (hoveringIndex % 9) * unitSize, plotStart.y + (hoveringIndex / 9) * unitSize, unitSize * 1.0f - 1, unitSize * 1.0f - 1};
        DrawRectangleRec(highlightingEffect, WHITE);
        DrawRectangleRec(highlightingEffect, affectedCellMain);

        if (cellSelected)
        {
            DrawRectangleRec(highlightingEffect, WHITE);
            DrawRectangleRec(highlightingEffect, selectedCell);
        }
        
    }

    // Drawing the 9 cells
    for (int i = 0; i < row + 1; i++)
    {
        int strokeSize = 1;
        if ((i) % 3 == 0)
            strokeSize = 2;
        else
            strokeSize = 1;

        // Drawing horizontal and vertical lines
        DrawLineEx(hLineStart, hLineEnd, strokeSize, BLACK);
        DrawLineEx(vLineStart, vLineEnd, strokeSize, BLACK);

        // Update line parameters
        hLineStart.y += unitSize;
        hLineEnd.y += unitSize;

        vLineStart.x += unitSize;
        vLineEnd.x += unitSize;
    }
    // Displaying the numbers
    for(int i=0;i<row*column;i++)
    {
        char text[2]={cellValues.at(i),'\0'};       

        Rectangle highlightingEffect = {plotStart.x + (i % 9) * unitSize, plotStart.y + (i / 9) * unitSize, unitSize * 1.0f - 1, unitSize * 1.0f - 1};
        DrawText(text, highlightingEffect.x + unitSize / 2, highlightingEffect.y + unitSize / 2, 20, RED);
    }
    
    plotStart = {float(screen.x) / 2 - column * unitSize / 2, float(screen.y) / 2 - row * unitSize / 2};
    plotEnd = {float(screen.x) / 2 + column * unitSize / 2, float(screen.y) / 2 + row * unitSize / 2};

    hLineStart = plotStart;
    hLineEnd = {plotEnd.x, plotStart.y};

    vLineStart = plotStart;
    vLineEnd = {plotStart.x, plotEnd.y};
}

void board::setup()
{
    // Window settings
    InitWindow(screen.x, screen.y, "Sudoku");
    SetTargetFPS(60);
}

board::~board()
{
    delete input;
    return;
}

int board::hoverCheck()
{
    // Checking if hovering
    int2d mouse = input->getMouseCursorPosition();
    int hoveringCellIndex = -1;
    if ((mouse.x >= plotStart.x && mouse.x <= plotEnd.x) && (mouse.y >= plotStart.y && mouse.y <= plotEnd.y))
    {
        hover = true;
        hoveringCellIndex = int(mouse.x - plotStart.x) / unitSize + int(mouse.y - plotStart.y) / unitSize * column;
    }
    else
    {
        hover = false;
        hoveringCellIndex = -1;
    }
    return hoveringCellIndex;
}

inputHandling::inputHandling()
{
    currentKeyPress = GetKeyPressed();
    mouseCursorPosition.x = GetMouseX();
    mouseCursorPosition.y = GetMouseY();
    mouseButtonPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    firstClick=false;
    // mouseButtonPressed=!IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

inputHandling::~inputHandling()
{
    return;
}

int inputHandling::getCurrentKeyPressed()
{
    currentKeyPress = GetKeyPressed();
    return currentKeyPress;
}

int2d inputHandling::getMouseCursorPosition()
{
    mouseCursorPosition.x = GetMouseX();
    mouseCursorPosition.y = GetMouseY();

    return mouseCursorPosition;
}

bool inputHandling::getMouseButtonPressed()
{
    mouseButtonPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    return mouseButtonPressed;
}

bool inputHandling::getFirstClick()
{
    return firstClick;
}

bool inputHandling::setFirstClick()
{
    firstClick=!firstClick;
    return firstClick;
}

int main()
{
    board sudoku;
    inputHandling inp;
    sudoku.setup();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        // inp.getCurrentKeyPressed();
        sudoku.drawBoard();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}