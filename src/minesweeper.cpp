#include <iostream>
#include <array>
#include <raylib.h>
#include <vector>
#include <algorithm>

using std::array,std::cout,std::endl;

constexpr int screenWidth=900,screenHeight=900;
constexpr int row=15,column=15,unitSize=40;
bool gameOver=false;
int flags=row*column*0.25,gameTime=0;


array<char,row*column> board;
Vector2 highlightedCell={-1,-1};

enum cellState
{
    HIDDEN=0,
    REVEALED=1,
    FLAGGED=2,
    MINE=3,
    FLAGGED_MINE=4   
};


void drawBoard();
bool initializeMines(Vector2 initialClick,int,int numberOfBombs=row*column*0.25);
int processInput(Vector2 &);
void updateBoard(Vector2 mouseInfo,int);
void openCompletelySafeCells(int currentCellX,int currentCellY);
void setup();
int assignNumbers(int x, int y);
void gameOverDisplay();
bool winCheck();



void drawBoard()
{    
    array<Color,4> cellColor{GREEN,BROWN,GREEN,GREEN};    

    Vector2 start={screenWidth/2-column*unitSize/2,screenHeight/2-row*unitSize/2};
    Vector2 size={unitSize-1,unitSize-1};

    int flagFontSize=30,numberFontSize=20;
    DrawText(TextFormat("FLAGS: %d", flags),start.x+unitSize/3,start.y/2,flagFontSize,BLACK);    
    DrawText(TextFormat("TIME: %d",gameTime),start.x+unitSize*column/2,start.y/2,flagFontSize,BLACK);

    for(int y=0;y<row;y++)
    {
        for(int x=0;x<column;x++)
        {
            //Hidden and revealed cell            
            int index=x+y*column;
            DrawRectangleV(start,size,cellColor[board.at(index)]);

            if(board.at(index)==REVEALED)
            {
                if(assignNumbers(x,y))
                {
                    int cellNumber=assignNumbers(x,y)+'0';                                
                    DrawText(TextFormat("%c",cellNumber),start.x+unitSize/3,start.y+unitSize/3,numberFontSize,BLACK);

                }
                

            }

            // Flagged cell
            if(board.at(index)==FLAGGED || board.at(index)==FLAGGED_MINE)
            {
                DrawRectangleV(start,size,YELLOW);
                Vector2 triangleV1={start.x+unitSize/4,start.y+unitSize/4};
                Vector2 triangleV3={start.x+3*unitSize/4,start.y+unitSize/2};
                Vector2 triangleV2={start.x+unitSize/4,start.y+3*unitSize/4};
                DrawTriangle(triangleV1,triangleV2,triangleV3,RED);
            }
            // Mines
            if(gameOver && !winCheck())
            {
                if(board.at(index)==MINE || board.at(index)==FLAGGED_MINE)
                {
                    DrawRectangleV(start,size,YELLOW);

                    Vector2 center={start.x+unitSize/2,start.y+unitSize/2};
                    float radius=unitSize/2 -2;
                    DrawCircleV(center,radius,BLACK);            
                }

            }
            

            start.x+=unitSize;            

        }
        start.x=screenWidth/2-column*unitSize/2;
        start.y+=unitSize;

    }
    if(gameOver)
    {
        gameOverDisplay();
        return;
    }
    // Highlighted cell
    if(highlightedCell.x+highlightedCell.y*column<row*column && highlightedCell.x+highlightedCell.y*column >=0)
    {
        float highlightFactor=0.8;
        Vector2 highlightStart={screenWidth/2-column*unitSize/2,screenHeight/2-row*unitSize/2};
        int highlightedCellState=board.at(highlightedCell.x+highlightedCell.y*column);
        Color highlighted=cellColor[highlightedCellState];

        highlighted.a*=highlightFactor;
        highlightStart.x+=highlightedCell.x*unitSize;
        highlightStart.y+=highlightedCell.y*unitSize;

        DrawRectangleV(highlightStart,size,WHITE);
        DrawRectangleV(highlightStart,size,highlighted);


        if(highlightedCellState==REVEALED)
        {
            // cout<<"Highlighted cell: "<<highlightedCell.x<<" "<<highlightedCell.y<<endl;
            // cout<<highlightedCell.x<<" "<<highlightedCell.y<<cellNumber<<endl;
            
            DrawRectangleV(highlightStart,size,WHITE);
            DrawRectangleV(highlightStart,size,highlighted);
            if(assignNumbers(highlightedCell.x,highlightedCell.y))
            {
                char cellNumber[]={char(assignNumbers((int)highlightedCell.x,(int)highlightedCell.y)+'0'),'\0'};
                DrawText(cellNumber,highlightStart.x+unitSize/3,highlightStart.y+unitSize/3,20,BLACK);
            }
           
        }

        if(highlightedCellState==FLAGGED || highlightedCellState==FLAGGED_MINE )
        {
            Vector2 triangleV1={highlightStart.x+unitSize/4,highlightStart.y+unitSize/4};
            Vector2 triangleV3={highlightStart.x+3*unitSize/4,highlightStart.y+unitSize/2};
            Vector2 triangleV2={highlightStart.x+unitSize/4,highlightStart.y+3*unitSize/4};
            Color Flag=RED; Flag.a*=highlightFactor;

            DrawTriangle(triangleV1,triangleV2,triangleV3,Flag);

        }
    }

}


bool initializeMines(Vector2 initialClick,int mouseButton,int numberOfBombs)
{
    if(mouseButton!=MOUSE_BUTTON_LEFT)
        return false;

    std::vector<int> mineList;    
   
    for(int i=0;i<numberOfBombs;i++)
    {
        int x=rand()%column,y=rand()%row;
        int bufferSize=2;

        bool noMineInFirstClick=(x-bufferSize<initialClick.x &&x+bufferSize>initialClick.x && y-bufferSize<initialClick.y && y+bufferSize>initialClick.y);     
        bool notInCurrentList=(std::find(std::begin(mineList), std::end(mineList), (x+y*column)) != std::end(mineList));
        
        if(noMineInFirstClick || notInCurrentList)
        {
            while(noMineInFirstClick || notInCurrentList)
            {
                x=rand()%column;
                y=rand()%row;
                noMineInFirstClick=(x-bufferSize<initialClick.x &&x+bufferSize>initialClick.x && y-bufferSize<initialClick.y && y+bufferSize>initialClick.y);
                notInCurrentList=(std::find(std::begin(mineList), std::end(mineList), (x+y*column)) != std::end(mineList));
            }
                
        }        
        board.at(x+column*y)=MINE;
        mineList.push_back(x+column*y);
        cout<<x<<" "<<y<<endl;
    }
    cout<<"Total mines:"<<mineList.size()<<endl;

    
    return true;
    
}

void cheat()
{
    if(IsKeyPressed(KEY_C))
    {        
        // reveal some hidden cells
        int counter=0,Xindex=rand()%column,Yindex=rand()%row;
        for(int i=0;i<row*column/2;i++)
        {
            if((Xindex+Yindex*column)>row*column)
                break;
            if(board.at(Xindex+Yindex*column)==HIDDEN)
            {
                openCompletelySafeCells(Xindex,Yindex);
                counter++;
            }
            Xindex++;
            if((Xindex)%column==0)
            {
                Xindex=0;
                Yindex++;
            }
            if(counter==8 || Yindex>=row)
                break;                

        }        
        
    }
    
}

int processInput(Vector2& mousePosition)
{
    if(gameOver || winCheck())
    {
        gameOver=true;
        return -1;
    }
    gameTime=(int)GetTime();
    cheat();
    int startX=screenWidth/2-column*unitSize/2;
    int startY=screenHeight/2-row*unitSize/2;

    Vector2 cellHighlightPosition=GetMousePosition();
    Vector2 tempPosition={(cellHighlightPosition.x-startX)/unitSize,(cellHighlightPosition.y-startY)/unitSize};
    
    if(tempPosition.x>=column || tempPosition.y >=row || tempPosition.x<0 ||tempPosition.y<0)
        highlightedCell=Vector2{-1,-1};
    else
        {
            int tempX=tempPosition.x,tempY=tempPosition.y;
            highlightedCell={tempX*1.0f,tempY*1.0f};
        }    


    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouseData=GetMousePosition();       

        Vector2 tempPosition={(cellHighlightPosition.x-startX)/unitSize,(cellHighlightPosition.y-startY)/unitSize};
        
        // cout<<mouseData.x<<" "<<mouseData.y<<endl;
        // cout<<tempPosition.x<<" "<<tempPosition.y<<endl;

        if(tempPosition.x>=column || tempPosition.y >=row || tempPosition.x<0 ||tempPosition.y<0)
        {
            mousePosition= Vector2{-1,-1};
            return -1;
        }
        
        int cellIndex=(int)tempPosition.x+(int)tempPosition.y*column;
        
        mousePosition=tempPosition;
        return MOUSE_BUTTON_LEFT;

    }    
    else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Vector2 mouseData=GetMousePosition();
        Vector2 tempPosition={(cellHighlightPosition.x-startX)/unitSize,(cellHighlightPosition.y-startY)/unitSize};
        if(tempPosition.x>=column || tempPosition.y >=row || tempPosition.x<0 ||tempPosition.y<0)
        {
            mousePosition= Vector2{-1,-1}; 
            return -1;            
        }
        
        int cellIndex=(int)tempPosition.x+(int)tempPosition.y*column;

        mousePosition=tempPosition;

        return MOUSE_BUTTON_RIGHT;
    }
    else if(IsKeyPressed(KEY_X))
    {
        Vector2 mouseData=GetMousePosition();
        Vector2 tempPosition={(cellHighlightPosition.x-startX)/unitSize,(cellHighlightPosition.y-startY)/unitSize};
        if(tempPosition.x>=column || tempPosition.y >=row || tempPosition.x<0 ||tempPosition.y<0)
        {
            mousePosition= Vector2{-1,-1};       
            return -1;
        }
        
        int cellIndex=(int)tempPosition.x+(int)tempPosition.y*column;
        mousePosition=tempPosition;

        return MOUSE_BUTTON_RIGHT;

    }    
    else
    {
        mousePosition= Vector2{-1,-1};  
        return -1;

    }
    return -1;
}

void openCompletelySafeCells(int currentCellX,int currentCellY)
{
    int surroundingMines=assignNumbers(currentCellX,currentCellY);
    if(currentCellX<0 || currentCellX>=column || currentCellY<0 || currentCellY>=row)
        return;

    if(surroundingMines!=0 || board.at(currentCellX+currentCellY*column)==REVEALED)
    {
        board.at(currentCellX+currentCellY*column)=REVEALED;
        return;
    }    
    
    else
    {
        board.at(currentCellX+currentCellY*column)=REVEALED;

        // CounterClockwise rotation
        openCompletelySafeCells(currentCellX+1,currentCellY+0);
        openCompletelySafeCells(currentCellX+1,currentCellY-1);
        openCompletelySafeCells(currentCellX+0,currentCellY-1);
        openCompletelySafeCells(currentCellX-1,currentCellY-1);
        openCompletelySafeCells(currentCellX-1,currentCellY+0);
        openCompletelySafeCells(currentCellX-1,currentCellY+1);
        openCompletelySafeCells(currentCellX+0,currentCellY+1);
        openCompletelySafeCells(currentCellX+1,currentCellY+1);

        
    }
}

void updateBoard(Vector2 mouseInfo,int mouseClick)
{    
    if(mouseClick<0)
        return;

    int cellIndex=(int)mouseInfo.x+(int)mouseInfo.y*column;
    Vector2 temp={(float)cellIndex,0.0f};

    if(int(mouseClick)==MOUSE_BUTTON_LEFT)
    {
        if(board.at(int(temp.x))==HIDDEN)
        {
            int tempX=mouseInfo.x,tempY=mouseInfo.y;
            openCompletelySafeCells(mouseInfo.x,mouseInfo.y);
        }
        
        if(board.at(int(temp.x))==MINE)
            gameOver=true;;
    }
    else if(int(mouseClick==MOUSE_BUTTON_RIGHT))
    {
        if(board.at(int(temp.x))==FLAGGED)
        {
            board.at(int(temp.x))=HIDDEN;
            flags++;
        }

        else if(board.at(int(temp.x))==FLAGGED_MINE)
        {
            board.at(int(temp.x))=MINE;        
            flags++;

        }

        else if(board.at(int(temp.x))==HIDDEN)
        {
            board.at(int(temp.x))=FLAGGED;
            flags--;
        }

        else if(board.at(int(temp.x))==MINE)
        {
            board.at(int(temp.x))=FLAGGED_MINE;        
            flags--;
        }
    }      
}

bool winCheck()
{
    bool flag=false;
    for(int i=0;i<row*column;i++)
    {
        if(board.at(i)==HIDDEN || board.at(i)==MINE)
        {
            flag=false;
            return flag;
        }

    }
    return true;
}

void gameOverDisplay()
{
    int postGameFont=50,padding=10;


    if(winCheck())
    {
        std::cout<<"Yeah You win"<<std::endl;
        DrawText("CONGRATULATIONS",screenWidth/2 -postGameFont-padding,screenHeight-postGameFont-padding,postGameFont,RED);
        return;
    }
    std::cout<<"gameOver"<<std::endl;
    DrawText("GAME-OVER",screenWidth/2 -postGameFont-padding,screenHeight-postGameFont-padding,postGameFont,RED);
}

int assignNumbers(int x, int y)
{
    int numberOfSurroundingMines=0;
    for(int i=-1;i<2;i++)
    {
        for(int j=-1;j<2;j++)
        {            
            if((x+j)>=0 && (x+j)<column && (y+i)>=0 && (y+i)<row)
            {
                int index=(x+j)+(y+i)*column;
                if(board.at(index)==MINE || board.at(index)==FLAGGED_MINE)
                    numberOfSurroundingMines++;
            }
            
        }
    }
    return numberOfSurroundingMines;
}



void setup()
{
    // Window settings
    InitWindow(screenWidth,screenHeight,"Minesweeper");
    SetTargetFPS(60);
    srand(time(NULL));

    // Board initialization
    for(int i=0;i<row*column;i++)    
        board.at(i)=HIDDEN;

}

int main()
{    
    setup();
    Vector2 mousePosition;
    bool IsMinesSet=false;
    int mouseButton=-1;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        drawBoard();
        mouseButton=processInput(mousePosition);
        if(!IsMinesSet)
            IsMinesSet=initializeMines(mousePosition,mouseButton);        
        updateBoard(mousePosition,mouseButton);
        
        EndDrawing();
    }
    CloseWindow();


    return 0;
}