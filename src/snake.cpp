#include <iostream>
#include <raylib.h>
using std::cout,std::endl;

constexpr int screenWidth = 700;
constexpr int screenHeight = 700;
constexpr Vector2 unitSize = {20,20};
bool gameover=false;

Vector2 direction = {0, -1};
Vector2 snakePosition[int(screenWidth*screenHeight/unitSize.x)]; //screenWidth*screenHeight/unitSize

void drawBoard(Vector2 &, int&);
void processInput(Vector2 &);
void update(int);
void addLength(int );
void checkCollision(int& ,Vector2 &);
void IfGameOver();

int main()
{
    int size = 1,numberOfFrames=0,speed=4;
    int numberOfGrid=screenWidth/int(unitSize.x);
    for(int i=0;i<size;i++)
        snakePosition[i] = {(numberOfGrid/2)*unitSize.x, ((numberOfGrid/2) +i)*unitSize.y};
    Vector2 snakeHead=snakePosition[0];
    Vector2 foodPos = {rand()%numberOfGrid*unitSize.x+unitSize.x/2,rand()%numberOfGrid*unitSize.y+unitSize.y/2};

    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        drawBoard(foodPos, size);
        processInput(snakeHead);
        // Speed control
        if(numberOfFrames%speed==0)  update(size);      
        checkCollision(size,foodPos);
        IfGameOver();
        EndDrawing();
        numberOfFrames++;
    }
    CloseWindow();

    return 0;
}




void drawBoard(Vector2 &food, int &snakeSize)
{
    // Draw Grid

    // for(Vector2 initialPos={0,0},finalPos={initialPos.x,screenHeight};initialPos.x<screenWidth;initialPos.x+=unitSize.x,finalPos.x+=unitSize.x)
    //     DrawLineV(initialPos,finalPos,YELLOW);
    
    // for(Vector2 initialPos={0,0},finalPos={screenHeight,initialPos.y};initialPos.y<screenHeight;initialPos.y+=unitSize.y,finalPos.y+=unitSize.y)
    //     DrawLineV(initialPos,finalPos,YELLOW);
    

    // Draw Snake
    DrawRectangleV(snakePosition[0],unitSize,WHITE);        

    for(int i=1;i<snakeSize;i++)
    {
        DrawRectangleV(snakePosition[i],unitSize,SKYBLUE);        
    }

    // Draw food
    DrawCircleV(food,unitSize.x/2,WHITE);
}

void processInput(Vector2 & snakeHead)
{
    if (IsKeyPressed(KEY_UP)&&direction.y!=1)
    {
        direction.x = 0;
        direction.y = -1;
        return;
        
    }
    else if (IsKeyPressed(KEY_DOWN) && direction.y!=-1)
    {
        direction.x = 0;
        direction.y = 1;
        return;

    }
    else if (IsKeyPressed(KEY_RIGHT)&&direction.x!=-1)
    {
        direction.x = 1;
        direction.y = 0;
        return;

    }
    else if (IsKeyPressed(KEY_LEFT) &&direction.x!=1)
    {
        direction.x = -1;
        direction.y = 0;
        return;

    }
}

void update(int snakeSize)
{
    if(gameover)
        return;
    Vector2 *tempSnakeState= new Vector2[snakeSize];
    std::copy(snakePosition,snakePosition+snakeSize-1,tempSnakeState);

    
    snakePosition[0].x+=direction.x*unitSize.x;
    snakePosition[0].y+=direction.y*unitSize.y;    

    if(snakePosition[0].x<0)
    {
        snakePosition[0].x=screenWidth-unitSize.x;
    }

    if(snakePosition[0].x>screenWidth)
        snakePosition[0].x=0;
    
    if(snakePosition[0].y<0)
        snakePosition[0].y=screenHeight-unitSize.y;

    if(snakePosition[0].y>screenHeight)
        snakePosition[0].y=0;

    for(int i=1;i<snakeSize;i++)
    {
        snakePosition[i]=tempSnakeState[i-1];        
    }
    delete []tempSnakeState;
}

void addLength(int snakeSize)
{
    Vector2 *tempSnakeState= new Vector2[snakeSize];
    std::copy(snakePosition,snakePosition+snakeSize,tempSnakeState);

    snakePosition[0].x+=direction.x*unitSize.x;
    snakePosition[0].y+=direction.y*unitSize.y; 

    for(int i=1;i<=snakeSize;i++)
    {
        snakePosition[i]=tempSnakeState[i-1];
    }
    delete [] tempSnakeState;
}

void checkCollision(int& snakeSize,Vector2 &food)
{
    if(gameover)
        return;
    Rectangle head={snakePosition[0].x,snakePosition[0].y,unitSize.x,unitSize.y};
    for(int i=1;i<snakeSize;i++)
    {
        Rectangle body={snakePosition[i].x,snakePosition[i].y,unitSize.x,unitSize.y};
        if(CheckCollisionRecs(head,body))
        {
            for(int i=0;i<snakeSize;i++)
                cout<<snakePosition[i].x<<" "<<snakePosition[i].y<<endl;
            gameover=true;
            return;
        }       
    }
    if(CheckCollisionCircleRec(food,0.9*unitSize.x/2,head))
    {
        int numberOfGrid=screenWidth/int(unitSize.x);
        addLength(snakeSize);
        snakeSize++;
        food={rand()%numberOfGrid*unitSize.x+unitSize.x/2,rand()%numberOfGrid*unitSize.y+unitSize.y/2};
    }

}

void IfGameOver()
{    
    if(gameover)
    {
            
        DrawText("GAME-OVER",screenWidth/2,screenHeight/2,50,RED);
    }
    
}
