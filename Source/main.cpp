#include "raylib.h"
#include "ball.cpp"
#include "paddle.cpp"
#include "block.cpp"

typedef enum GameScreen { TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void)
{
    const int screenWidth  = 800;
    const int screenHeight = 450;
    const char* losertext  = nullptr;
    const char* restart    = nullptr;

    int score = 0;
    int lives = 3;

    ball.x      = screenWidth / 2;
    ball.y      = screenHeight / 2;
    ball.radius = 10;
    ball.speedx = 300;
    ball.speedy = 300;

    paddle.x      = screenWidth / 2;
    paddle.y      = screenHeight - 20;
    paddle.width  = 150;
    paddle.height = 15;
    paddle.speed  = 500;

    block.x           = 10;
    block.y           = 40;
    block.width       = 100;
    block.height      = 30;
    block.rowIndex    = 5;
    block.columnIndex = 5;
    block.isAlive = true;

    InitWindow(screenWidth, screenHeight, "Piong");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    GameScreen screen = TITLE;      
    int framesCounter = 0;          
    int gameResult = -1;            
    bool gamePaused = false;       

    while (!WindowShouldClose() )
    {
        switch (screen)
        {
        case TITLE:
        {
            framesCounter++;

            if (IsKeyPressed(KEY_SPACE))
            {
                screen = GAMEPLAY;
            }

        } break;

        case GAMEPLAY:
        {
            if (!gamePaused)
            {
                // Update
                //----------------------------------------------------------------------------------
                //BALL
                int index = 0;

                ball.x += ball.speedx * GetFrameTime();
                ball.y += ball.speedy * GetFrameTime();

                if (ball.x <= ball.radius || ball.x >= screenWidth - ball.radius)
                {
                    index++;
                    ball.speedx = -ball.speedx;
                }

                if (ball.y <= ball.radius)
                {
                    index++;
                    ball.speedy = -ball.speedy;
                }


                //PADDLE
                if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
                {
                    paddle.x -= paddle.speed * GetFrameTime();
                }
                if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
                {
                    paddle.x += paddle.speed * GetFrameTime();
                }

                if (paddle.x - paddle.width / 2 <= 0)
                {
                    paddle.x = 0 + paddle.width / 2;
                }

                if (paddle.x + paddle.width / 2 >= screenWidth)
                {
                    paddle.x = screenWidth - paddle.width / 2;
                }

                //COLLISION
                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
                    paddle.GetRectangle()))
                {
                    ball.speedy *= -1;
                }

                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
                    block.GetRectangle()))
                {
                    block.isAlive = false;
                    ball.speedy *= -1;
                }

                
            }
            if (ball.y >= screenHeight)
                {
                    ball.ResetBall();
                    lives--;

                    if (lives <= 0)
                    {
                        lives = 3;
                        losertext = "YOU LOSE!!!";
                        restart = "PRESS SPACE TO PLAY AGAIN";
                        screen = ENDING;
                    }
                }

        } break;

        case ENDING:
        {
            //END GAME AND RESTART

            framesCounter++;
            if (IsKeyPressed(KEY_SPACE))
            {
                if (losertext)
                {
                    losertext = nullptr;
                    restart = nullptr;
                    ball.ResetBall();
                }
                screen = TITLE;
            }

        } break;

        default: break;
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
        DrawFPS(10, 10);
        
        switch (screen)
        {
        case TITLE:
        {
            int textwidth = MeasureText(losertext, 60);
            int rtextwidth = MeasureText(restart, 30);
            DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
            DrawText("PIONG", screenWidth / 2.75 - textwidth / 2, screenHeight / 2 - 30, 60, RED);
            DrawText("PRESS SPACE TO START GAME", screenWidth / 5 - rtextwidth / 2, screenHeight / 2 + 35, 30, YELLOW);

        } break;

        case GAMEPLAY:
        {
            ball.Draw();
        
            paddle.Draw();

            if(block.isAlive == true)
            { 
            block.Draw();
            }

            DrawText( TextFormat("Lives =""% i", lives), 150, 10, 20, RAYWHITE);
            DrawText(TextFormat("Score =""% i", score), screenWidth - 150, 10, 20, RAYWHITE);
        } break;

        case ENDING:
        {
            if (losertext)
            {
            int textwidth  = MeasureText(losertext, 60);
            int rtextwidth = MeasureText(restart, 30);
            DrawText(losertext, screenWidth/2 - textwidth/2, screenHeight / 2 - 30, 60, RED);
            DrawText(restart, screenWidth / 2  - rtextwidth / 2, screenHeight / 2 +35, 30, YELLOW);
            }
        } break;

        default: break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
