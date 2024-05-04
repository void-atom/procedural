#include <iostream>
#include <raylib.h>

const int screenWidth = 900;
const int screenHeight = 700;

const int pongWidth = 20;
const int pongHeight = 80;
const float ballRadius = 8;
int paddleSpeed = 9;

Vector2 ballSpeed = {8, 8};
Vector2 speedState = {1, 1};

void drawBoard(Vector2 &playerOne, Vector2 &playerTwo, Vector2 &ball)
{
    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

    // Draw player pong
    DrawRectangle(playerOne.x, playerOne.y, pongWidth, pongHeight, WHITE);

    // Draw CPU pong
    DrawRectangle(playerTwo.x, playerTwo.y, pongWidth, pongHeight, WHITE);

    // Draw ball
    DrawCircle(ball.x, ball.y, ballRadius, WHITE);
}

void updateBall(Vector2 &ball, Vector2 &player1, Vector2 &player2)
{
    Rectangle paddle1 = {player1.x, player1.y, pongWidth, pongHeight}, paddle2 = {player2.x, player2.y, pongWidth, pongHeight};
    if (ball.x <= 0 || ball.x >= screenWidth)
        speedState.x *= -1;
    // ball={screenWidth/2,screenHeight/2};

    if (ball.y <= 0 || ball.y >= screenHeight)
        speedState.y *= -1;

    if (CheckCollisionCircleRec(ball, ballRadius, paddle1) || CheckCollisionCircleRec(ball, ballRadius, paddle2))
        speedState.x *= -1;

    ball.x += ballSpeed.x * speedState.x;
    ball.y += ballSpeed.y * speedState.y;
}

void processInput(Vector2 &player1)
{
    if (IsKeyDown(KEY_DOWN))
    {
        paddleSpeed = paddleSpeed > 15 ? paddleSpeed++ : paddleSpeed;
        player1.y += paddleSpeed;
        if (player1.y + pongHeight >= screenHeight)
            player1.y = screenHeight - pongHeight;
    }
    else if (IsKeyDown(KEY_UP))
    {
        paddleSpeed = paddleSpeed <= 8 ? paddleSpeed : paddleSpeed--;
        player1.y -= paddleSpeed;
        if (player1.y <= 0)
            player1.y = 0;
    }
}

void updateCPU(Vector2 &player2, Vector2 &ball)
{
    int tempX = screenWidth;

    if (speedState.x > 0)
    {
        float slope = float(speedState.y * ballSpeed.y) / float(speedState.x * ballSpeed.x);
        int projectedY = int(float(tempX - ball.x) * slope + ball.y);
        if (projectedY > screenHeight)
        {
            // std::cout<<projectedY<<" ";
            int tempY2 = screenHeight;
            int tempX2 = ball.x + (tempY2 - ball.y) * 1 / slope;
            int slope2 = -slope;
            int projectedY2 = int(float(screenWidth - tempX2) * slope2 + tempY2);
            if (projectedY2 > player2.y)
                player2.y += paddleSpeed;
            else if (projectedY2 < player2.y)
                player2.y -= paddleSpeed;
            
            return;
        }
        if (projectedY > player2.y)
            player2.y += paddleSpeed;
        else if (projectedY < player2.y)
            player2.y -= paddleSpeed;
        if (player2.y + pongHeight >= screenHeight)
            player2.y = screenHeight - pongHeight;
        else if (player2.y <= 0)
            player2.y = 0;
    }
}
void collisionCheck(Vector2 &ball, Vector2 &player1, Vector2 &player2)
{
}

int main()
{
    Vector2 player1 = {10, (screenHeight - pongHeight) / 2};
    Vector2 player2 = {screenWidth - 10 - pongWidth, (screenHeight - pongHeight) / 2};
    Vector2 ball = {screenWidth / 2, screenHeight / 2};

    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        processInput(player1);
        updateCPU(player2, ball);

        BeginDrawing();
        ClearBackground(BLACK);
        drawBoard(player1, player2, ball);
        updateBall(ball, player1, player2);
        EndDrawing();
    }
    CloseWindow();


    return 0;
}