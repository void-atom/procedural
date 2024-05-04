#include <iostream>
#include <vector>
#include <random>
#include "raylib.h"

using std::cout,std::endl,std::string,std::array;

//Global Variables
#define row 20             
#define column 10
#define cellSize 40 

unsigned char board[row*column];
string blocks[7];
Color tetromino[8]={GRAY,MAROON,ORANGE,PURPLE,RED,MAGENTA,PINK,SKYBLUE};    

const int screenWidth = 1000;
const int screenHeight = 990;
bool gameOver=false;
int score=0;


std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator

//Function declarations
void initializeBoard();
void drawBoard(Vector2 position,int &current,int rotation);
void processInput(Vector2 &position,float,int&,int&,bool&);
int rotate(int,int,int);
int checkCollision(Vector2,int&,int);
void updateBoardData(Vector2&, int&,int);
std::vector<int> lineClear();
void removeLine(std::vector<int>);
void cheats(Vector2 &postion,float deltaTime,int& rotation,int& current);
char* intToString(int);
void IfGameOver();

int randomBlockGenerator()
{
    std::uniform_int_distribution<> distr(0, 6); // define the range
    int randomNum=distr(gen); // generate numbers
    return randomNum;
}

unsigned int gameSpeed()
{
    int time =GetTime();
    if(time<60)
        return 60;
    else if(time<120)
        return 20;
    else if(time<180)
        return 10;
    else
        return 5;
}

int main()
{
    Vector2 currentBlockPosition={0,0};
    initializeBoard();    
    InitWindow(screenWidth, screenHeight, "Tetris");
    // cout<<++currentBlockPosition.x<<","<<++currentBlockPosition.y<<endl;

    int rotation=0;unsigned int numberOfFrames=0;
    int current=randomBlockGenerator();
    bool tick=false;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        processInput(currentBlockPosition,deltaTime,rotation,current,tick);
        cheats(currentBlockPosition,deltaTime,rotation,current);
        // if(lineClear())
        removeLine(lineClear());

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLUE);
        drawBoard(currentBlockPosition,current,rotation);

        EndDrawing();
        numberOfFrames++;
        //This moves the block down every frame
        if(numberOfFrames>=gameSpeed()) // 1second       
            {
                numberOfFrames=0;
                tick=true;
            }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------   


    return 0;
}

void initializeBoard()
{   
    // blocks data
    //I shape
    blocks[0]=  "0100"
                "0100"
                "0100"
                "0100";
    
    //J shape
    blocks[1]=  "0100"
                "0100"
                "1100"
                "0000";

    //L shape
    blocks[2]=  "0100"
                "0100"
                "0110"
                "0000";

    //O shape
    blocks[3]=  "0000"
                "0110"
                "0110"
                "0000";
    //S shape
    blocks[4]=  "0000"
                "0110"
                "1100"
                "0000";
    //T shape
    blocks[5]=  "1110"
                "0100"
                "0000"
                "0000";
    //Z shape
    blocks[6]=  "1100"
                "0110"
                "0000"
                "0000";
    

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            board[i*column+j]=0;
        }
    }


}

void drawBoard(Vector2 position,int &current,int rotation)
{
    int startX=(screenWidth-column*cellSize)/2;
    int startY=(screenHeight-row*cellSize)/2;

    //Draw static board elements
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            DrawRectangle(j*cellSize+startX,i*cellSize+startY,cellSize-1,cellSize-1,tetromino[board[j+i*column]]);            
        }
    }
    char *temp=intToString(score);

    int tempX=int(((screenWidth-column*cellSize)/2-120)/2);
    int tempY=int((screenHeight+row*cellSize)/3);
    DrawRectangle(tempX,tempY,120,1.5*120,GRAY);
    DrawText("SCORE:",tempX+10,tempY+10,15,RED);
    DrawText(temp,tempX+10,tempY+30,15,RED);
    delete [] temp;
    
    //Draw dynamic tetromino block 
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            int index=rotate(j,i,rotation);
            if(blocks[current][index]!='0')
            {
                DrawRectangle((j+int(position.x))*cellSize+startX,(i+int(position.y))*cellSize+startY,cellSize-1,cellSize-1,tetromino[current+1]);
            }
        }
    }  
    if(gameOver)
    {
        IfGameOver();
        return;
    }  
    

}

void processInput(Vector2 &position,float deltaTime,int& rotation,int& current,bool& moveDown)
{
    if(!gameOver)
    {
        Vector2 tempPos=position;    
    if(IsKeyDown(KEY_RIGHT))
    {
        tempPos.x+=1;    
        if(checkCollision(tempPos,current,rotation)==1)
            position.x+=0.3;//1.5*cellSize*deltaTime;
        
    }
    else if(IsKeyDown(KEY_LEFT))
    {
        tempPos.x-=1; 
        if(checkCollision(tempPos,current,rotation)==1)
            position.x-=0.3;//1.5*cellSize*deltaTime;        
    }
    else if((IsKeyDown(KEY_DOWN)&&position.y>0)||moveDown)
    {
        tempPos.y++; 
        if(checkCollision(tempPos,current,rotation)==1)
        {            
            position.y+=0.3;
            if(moveDown)
                position.y+=0.71;            

        }
        else if(checkCollision(tempPos,current,rotation)==-1)
        {
            updateBoardData(position,current,rotation);
        }
        
        moveDown=false;

    }
    
    else if(IsKeyPressed(KEY_UP))
    {
        rotation++; 
        if(checkCollision(tempPos,current,rotation)==0)
            rotation--;
        else if(checkCollision(tempPos,current,rotation)==-1)
        {
            updateBoardData(position,current,rotation);
        }
        
    }
    // else if(IsKeyPressed(KEY_A)) 0;
        // cheats(position,deltaTime,rotation,current); 

    }
        
}

int rotate(int x, int y,int numberOfRotation)
{
    int i=0;
    switch (numberOfRotation%4)
    {
    case 0:        
        i=y*4+x;
        break;
    
    case 1:
        i=12+y-x*4;
        break;
    
    case 2:
        i=15-4*y-x;
        break;
     
    case 3:
        i=3-y+x*4;
        break;
    
    default:
        break;
    }
    
    return i;   

}

int checkCollision(Vector2 position,int &current,int rotation)
{
    // cout<<"Entering"<<endl;
    // 1=no collision    0 = wall collision     -1= block or bottom collision
    int collisionFlag=1;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            int index=rotate(j,i,rotation);
            if(blocks[current][index]!='0')
            {
                int tempX=j+int(position.x);
                int tempY=i+int(position.y);
                // cout<<tempX<<" "<<tempY<<endl;
                //wall collision
                if(tempX<0 || tempX>=column ||tempY<0)
                {
                    // cout<<tempX<<" "<<tempY<<endl;
                    collisionFlag=0;
                    return collisionFlag;
                }
                // block or bottom collision                
                else if(board[tempX+(int(position.y)+i)*column]!=0||tempY>=row)
                {
                    // cout<<tempX<<" "<<tempY<<endl;
                    collisionFlag=-1;
                    return collisionFlag;
                }

            }
        }
    }
    return collisionFlag;
    

}

void boardPrint()
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
            cout<<int(board[i*column+j])<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void updateBoardData(Vector2 &position,int& currentBlock, int rotation)
{
    if(gameOver)
        return;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(blocks[currentBlock][rotate(j,i,rotation)]!='0')
            {
                if(i+int(position.y)<=0)
                {
                    gameOver=true;
                    return;
                }

                if(board[int(position.x)+j+(int(position.y)+i)*column]==0)
                    board[int(position.x)+j+(int(position.y)+i)*column]=currentBlock+1; 
                    // boardPrint();                   
            }
        }
    }
    position={0,0};
    currentBlock=randomBlockGenerator();

}

std::vector<int> lineClear()
{
    std::vector<int> lowestCompleteRow;
    for(int j=row-1;j>-1;j--)
    {
        bool flag=true;
        for(int i=0;i<column;i++)
        {
            if(board[i+j*column]==0)
            {
                flag=false;
                break;
            }
        }
        if(flag)
            lowestCompleteRow.push_back(j);
    }
    return lowestCompleteRow;
}

void removeLine(std::vector<int> completeRow)
{
    int scoreIndex=1;
    int tempScore=0;
    for(auto lastCompleteRow=completeRow.rbegin();lastCompleteRow!=completeRow.rend();lastCompleteRow++,scoreIndex++)
    {
        // std::cout<<*lastCompleteRow<<std::endl;
        for(int i=*lastCompleteRow-1,index=0;i>-1;index++,i--)
        {
            for(int j=0;j<column;j++)
            {
                board[(i+1)*column+j]=board[j+i*column];
            }
        }
        //initialize the top rows to 0
        for(int i=0;i<column;i++)
            board[i]=0;
        tempScore+=scoreIndex*scoreIndex*100;
    }
    score+=tempScore;
  
}

void cheats(Vector2 &postion,float deltaTime,int& rotation,int& current)
{
    if(IsKeyPressed(KEY_KP_0))
    {
        current=0;        
    }
    else if(IsKeyPressed(KEY_KP_1))
    {
        current=1;       
    }
    else if(IsKeyPressed(KEY_KP_2))
    {
        current=2;       
    }
    else if(IsKeyPressed(KEY_KP_3))
    {
        current=3;       
    }
    else if(IsKeyPressed(KEY_KP_4))
    {
        current=4;       
    }
    else if(IsKeyPressed(KEY_KP_5))
    {
        current=5;       
    }
    else if(IsKeyPressed(KEY_KP_6))
    {
        current=6;       
    }
    
    else if(IsKeyPressed(KEY_EQUAL)&& IsKeyPressed(KEY_MINUS))
    {        
        postion.y--;
    }   

}
void IfGameOver()
{
    std::cout<<"gameOver"<<std::endl;
    DrawText("GAME-OVER",screenWidth/2,screenHeight/2,50,RED);
}
char* intToString(int integer)
{
    // std::string value;
    char* value=new char[8]{'\0'};
    int remainder=0,i=0,number=integer;    

    do
    {
        remainder=number%10;
        number=number/10;
        value[i]=remainder+'0';
        // value.push_back(char(remainder+'0'));
        i++;
    } while (number>0);
    
    for(int latterIndex=i-1,formerIndex=0;latterIndex>formerIndex;latterIndex--,formerIndex++)
    {
        char temp=value[formerIndex];
        value[formerIndex]=value[latterIndex];
        value[latterIndex]=temp;

    }
    // std::reverse(value.begin(),value.end());
    return (value);
}
