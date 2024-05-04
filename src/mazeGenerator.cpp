#include <iostream>
#include <raylib.h>
#include <vector>
#include <array>
#include <cstdint>

using std::array,std::vector,std::cout,std::endl;

struct coordinate
{
    uint16_t x;
    uint16_t y;
};

enum pos
{
    LEFT=0,
    RIGHT=1,
    TOP=2,
    BOTTOM=3,
    ERROR=4
};

struct node
{
    coordinate position;
    bool neighbours[4];         //left,right,top,bottom
    bool visited=false;
    bool walls[4]={true,true,true,true};
};

constexpr uint16_t row=30,column=30,squareSize=20;
constexpr uint16_t screenWidth=700,screenHeight=700;

array <node,row*column> cellArray;
vector <coordinate> stack;

void initializeCells();
void draw(coordinate &current);
void removeBoundary(coordinate& currentIndex);
void removeBoundary(coordinate&, coordinate&,pos);
pos getNextIndex(coordinate currentIndex,coordinate &nextIndex);
void randomDFS(coordinate &currentIndex);


void initializeCells()
{
    srand(time(NULL));
    int X=0,Y=0;
    for(int i=0;i<row*column;i++)
    {
        cellArray[i].position.x=X;cellArray[i].position.y=Y;
        cellArray[i].visited=false;
        for(int x=0;x<4;x++)
            cellArray[i].neighbours[x]=true;

        if(X==0) cellArray[i].neighbours[LEFT]=false;
        if(X==column-1) cellArray[i].neighbours[RIGHT]=false;
        if(Y==0) cellArray[i].neighbours[TOP]=false;
        if(Y==row-1) cellArray[i].neighbours[BOTTOM]=false;


        if((i+1)%column==0)
        {
            X=0;Y++;
        }
        else 
            X++;       
        
    }
}

void draw(coordinate &current)
{
    Vector2 start={(screenWidth-column*squareSize)/2,(screenHeight-row*squareSize)/2};
    // Vector2 startX={(screenWidth-column*squareSize)/2,(screenHeight-row*squareSize)/2};

    for(auto index=0;index<row*column;index++)
    {
        // Draw squares
        Vector2 rectPos={start.x+cellArray[index].position.x*squareSize,start.y+cellArray[index].position.y*squareSize};
        Vector2 rectSize={squareSize,squareSize};
        DrawRectangleV(rectPos,rectSize,GRAY);

        // Draw Walls
        // TOP line
        float thickness=1.2;
        Vector2 lineStart={start.x+cellArray[index].position.x*squareSize,start.y+cellArray[index].position.y*squareSize};
        Vector2 lineEnd={lineStart.x+squareSize,lineStart.y};
        if(cellArray[index].walls[TOP])
            DrawLineEx(lineStart,lineEnd,thickness,RED);

        // LEFT line
        lineStart={start.x+cellArray[index].position.x*squareSize,start.y+cellArray[index].position.y*squareSize};
        lineEnd={lineStart.x,lineStart.y+squareSize};
        if(cellArray[index].walls[LEFT])
        DrawLineEx(lineStart,lineEnd,thickness,RED);

        // RIGHT LINE
        lineStart={start.x+cellArray[index].position.x*squareSize+squareSize,start.y+cellArray[index].position.y*squareSize};
        lineEnd={lineStart.x,lineStart.y+squareSize};
        if(cellArray[index].walls[RIGHT])
        DrawLineEx(lineStart,lineEnd,thickness,RED);

        // BOTTOM LINE
        lineStart={start.x+cellArray[index].position.x*squareSize,start.y+cellArray[index].position.y*squareSize+squareSize};
        lineEnd={lineStart.x+squareSize,lineStart.y};
        if(cellArray[index].walls[BOTTOM])
        DrawLineEx(lineStart,lineEnd,thickness,RED);
    }

    // Highlight current cell
    Vector2 rectPos={++start.x+current.x*squareSize,++start.y+current.y*squareSize};
    Vector2 rectSize={squareSize-2,squareSize-2};
    DrawRectangleV(rectPos,rectSize,LIME);

}

void removeBoundary(coordinate& currentIndex)
{
    // Manual Control
    if(IsKeyPressed(KEY_DOWN) &&currentIndex.y!=row-1)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[BOTTOM]=false;
        currentIndex.y++;
        cellArray[currentIndex.x+currentIndex.y*row].walls[TOP]=false;
    }
    
    else if(IsKeyPressed(KEY_RIGHT) &&currentIndex.x!=column-1)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[RIGHT]=false;
        currentIndex.x++;
        cellArray[currentIndex.x+currentIndex.y*row].walls[LEFT]=false;        
    }
    else if(IsKeyPressed(KEY_LEFT) && currentIndex.x>0)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[LEFT]=false;
        currentIndex.x--;
        cellArray[currentIndex.x+currentIndex.y*row].walls[RIGHT]=false;        
    }
    else if(IsKeyPressed(KEY_UP) &&currentIndex.y>0)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[TOP]=false;
        currentIndex.y--;
        cellArray[currentIndex.x+currentIndex.y*row].walls[BOTTOM]=false;
    }
    
}

void removeBoundary(coordinate& currentIndex, coordinate &nextIndex,pos wall)
{
    // This function removes the adjacent boundary walls between currentIndex and nextIndex
    if(wall==BOTTOM)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[BOTTOM]=false;    
        cellArray[nextIndex.x+nextIndex.y*row].walls[TOP]=false;
    }
    else if(wall==RIGHT)    
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[RIGHT]=false;
        cellArray[nextIndex.x+nextIndex.y*row].walls[LEFT]=false;        
    }
    else if(wall==LEFT)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[LEFT]=false;
        cellArray[nextIndex.x+nextIndex.y*row].walls[RIGHT]=false;        
    }
    else if(wall==TOP)
    {
        cellArray[currentIndex.x+currentIndex.y*row].walls[TOP]=false;
        cellArray[nextIndex.x+nextIndex.y*row].walls[BOTTOM]=false;
    }
}

pos getNextIndex(coordinate currentIndex,coordinate &nextIndex)
{
    nextIndex=currentIndex;

    //This array is used to just make sure that all the children of currentIndex are tried atleast once.
    //After each successful trial, the value of the array index is changed to 1, so if all 4 children are tried
    //the sum will  be 4.

    array<int,4> list{-1,-1,-1,-1};    
    
    for(int loop=0,index=rand()%4;loop<1000;loop++,index=rand()%4)
    {
        if(index==LEFT)
        {
            list[index]=1;
            if(!cellArray.at(currentIndex.x+currentIndex.y*row).neighbours[LEFT]) continue;
            nextIndex.x--;
            if(!cellArray.at(nextIndex.x+nextIndex.y*row).visited)                
                return LEFT;
            nextIndex=currentIndex;            
        }

        else if(index==RIGHT)
        {
            list[index]=1;
            if(!cellArray.at(currentIndex.x+currentIndex.y*row).neighbours[RIGHT]) continue;
            nextIndex.x++;
            if(!cellArray.at(nextIndex.x+nextIndex.y*row).visited) 
                return RIGHT;
            nextIndex=currentIndex;
            
        }
        else if(index==TOP)
        {
            list[index]=1;
            if(!cellArray.at(currentIndex.x+currentIndex.y*row).neighbours[TOP]) continue;
            nextIndex.y--;
            if(!cellArray.at(nextIndex.x+nextIndex.y*row).visited) 
                return TOP;
            nextIndex=currentIndex;
            
        }
        else if(index==BOTTOM)
        {
            list[index]=1;
            if(!cellArray.at(currentIndex.x+currentIndex.y*row).neighbours[BOTTOM]) continue;
            nextIndex.y++;
            if(!cellArray.at(nextIndex.x+nextIndex.y*row).visited) 
                return BOTTOM;
            nextIndex=currentIndex;
        }
        for(int i=0,sum=0;i<4;i++)
        {
            sum+=list[i];
            if(sum==4)
                return ERROR;
        }        
    }
    return ERROR;
}

void randomDFS(coordinate &currentIndex)
{
    coordinate nextIndex;
    pos direction=getNextIndex(currentIndex,nextIndex);

    // This checks if the current node has any unvisited children
    if(direction<4)
    {
        cellArray[nextIndex.x+nextIndex.y*row].visited=true;
        stack.push_back(currentIndex);
        removeBoundary(currentIndex,nextIndex,direction);
        currentIndex=nextIndex;
    }
    // If no children of current node are unvisited, then pop from stack
    else
    {
        if(stack.empty())
            return;
        nextIndex=stack.back();
        stack.pop_back();
        currentIndex=nextIndex;
    }
}

int main()
{
    initializeCells();
    
    coordinate currentIndex={0,0};
    node *current=&cellArray[currentIndex.x+currentIndex.y*row];

    InitWindow(screenWidth,screenHeight,"MAZE");
    SetTargetFPS(30);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        draw(currentIndex);
        randomDFS(currentIndex);
                
        EndDrawing();

    }
    CloseWindow();


    return 0;
}