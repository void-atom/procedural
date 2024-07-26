#include <iostream>
#include <raylib.h>

using std::array, std::cout, std::endl;

bool gameOver = false;
Vector2 mouseState = {-1, -1};

struct int2d
{
    int x;
    int y;
};

// class inputHandling;
class board
{
private:
    int2d screen;
    int row, column, unitSize,hoveringCell;
    Vector2 plotStart, plotEnd, hLineStart, hLineEnd, vLineStart, vLineEnd;
    bool hover;

public:    
    board();    
    ~board();

    void drawBoard();       // Draws the necessary graphics each frame
    void setup();           // Setup the required raylib window
    int hoverCheck();

};

class inputHandling
{
private:
    char currentKeyPress;
    int2d mouseCursorPosition;
    bool mouseButtonPressed;
    int currentCell;
public:
    inputHandling();
    ~inputHandling();
    
    char getCurrentKeyPressed();
    int2d getMouseCursorPosition();
    bool getMouseButtonPressed();

    // int cellHover();            // Checks if mouse is within a cell and returns the cell number;


};


board::board()
{
    row = 9, column = 9, unitSize = 50,hoveringCell=-1;
    screen = {900, 900};
    hover=false;

    // x,y coordinates of the leftmost and topmost point from where the graphics starts
    plotStart = {float(screen.x) / 2 - column * unitSize / 2, float(screen.y) / 2 - row * unitSize / 2};

    // x,y coordinates of the rightmost and bottommost point from where the graphics end
    plotEnd = {float(screen.x) / 2 + column * unitSize / 2, float(screen.y) / 2 + row * unitSize / 2};

    hLineStart = plotStart;
    hLineEnd = {plotEnd.x, plotStart.y};

    vLineStart = plotStart;
    vLineEnd = {plotStart.x, plotEnd.y};
}

void board::drawBoard()
{
    Color affectedCells={226, 235, 243,255};
    Color selectedCell={187, 222, 251,255};
    
    // Drawing the hovering cell highligh
    int hoveringIndex=hoverCheck();
    if(hover)
    {
        Rectangle hoverRect={plotStart.x+(hoveringIndex%9)*unitSize,plotStart.y+(hoveringIndex/9)*unitSize,unitSize*1.0f-1,unitSize*1.0f-1};
        // cout<<hoveringIndex<<endl;
        DrawRectangleRec(hoverRect,selectedCell);        
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
    srand(time(NULL));
}

board::~board()
{
    return;
}

int board::hoverCheck()
{
    // Checking if hovering
    inputHandling input;
    int2d mouse = input.getMouseCursorPosition();
    int hoveringCellIndex=-1;
    if((mouse.x>=plotStart.x && mouse.x<=plotEnd.x)&&(mouse.y>=plotStart.y && mouse.y<=plotEnd.y))
    {
        hover=true;
        hoveringCellIndex=int(mouse.x-plotStart.x)/unitSize+int(mouse.y-plotStart.y)/unitSize*column;
    }
    else
    {
        hover = false;
        hoveringCellIndex=-1;
    }
    return hoveringCellIndex;    
}


inputHandling::inputHandling()
{
    currentKeyPress=GetKeyPressed();
    mouseCursorPosition.x=GetMouseX();
    mouseCursorPosition.y=GetMouseY();
    mouseButtonPressed=IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    // mouseButtonPressed=!IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

inputHandling::~inputHandling()
{
    return;
}

char inputHandling::getCurrentKeyPressed(){return currentKeyPress;}

int2d inputHandling::getMouseCursorPosition()
{
    mouseCursorPosition.x=GetMouseX();
    mouseCursorPosition.y=GetMouseY();

    return mouseCursorPosition;
}

bool inputHandling::getMouseButtonPressed()
{
    mouseButtonPressed=IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    return mouseButtonPressed;
}



int main()
{
    board sudoku;
    sudoku.setup();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        sudoku.drawBoard();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}