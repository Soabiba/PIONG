#include <raylib.h>
#include "ball.cpp"
#include "paddle.cpp"
#include "block.cpp"
#include "gamestate.cpp"

typedef enum GameScreen { TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void)
{
    const int screenWidth = 850;
    const int screenHeight = 450;

    GameState state{ screenWidth, screenHeight };
    const char* losertext = nullptr;
    const char* restart = nullptr;

    int score = 0;
    int lives = 3;

    Rectangle screenbg = { 0, 0, screenWidth, screenHeight };

    InitWindow(screenWidth, screenHeight, "Piong");
    InitAudioDevice();

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    // C:\Users\frostfox\wkspaces\PIONG\assignment\Source\Resources

    Texture2D player;
    player = LoadTexture("./Source/Resources/player.png");
    
    Sound sound;
    sound = LoadSound("./Source/Resources/sound.wav");

    
    int framesCounter = 0;
    int gameResult = -1;
    bool gamePaused = false;
    GameScreen screen = TITLE;


    while (!WindowShouldClose())
    {
        switch (screen)
        {
        case TITLE:
        {
            framesCounter++;

            if (IsKeyPressed(KEY_SPACE))
            {
                state.reset();
                screen = GAMEPLAY;
            }

        } break;

        case GAMEPLAY:
        {
            Ball& ball = state.ball;
            Paddle& paddle = state.paddle;
            Block*& blocks = state.blocks;

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
                    PlaySoundMulti(sound);
                    ball.speedy *= -1;
                }

                bool shouldGameContinue = false;
                for (int i = 0; i < state.blockCount; i++)
                {
                    auto& block = state.blocks[i];
                    if (block.isAlive)
                    {
                        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
                            block.GetRectangle()))
                        {
                            block.isAlive = false;
                            score++;
                            ball.speedy *= -1;
                        }
                        shouldGameContinue = true;
                    }
                }

                if (!shouldGameContinue)
                {
                    lives = 3;
                    score = 0;
                    losertext = "YOU WIN!!!!";
                    restart = "PRESS SPACE TO PLAY AGAIN";
                    screen = ENDING;
                }
            }
            if (ball.y >= screenHeight)
            {
                ball.ResetBall();
                lives--;

                if (lives <= 0)
                {
                    lives = 3;
                    score = 0;
                    losertext = "YOU LOSE!! HAHAHAHA"
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
            Ball& ball = state.ball;
            Paddle& paddle = state.paddle;

            DrawTexturePro(player, screenbg, screenbg, Vector2{ 0, 0 }, 0, RAYWHITE);

            ball.Draw();

            paddle.Draw();


            for (int i = 0; i < state.blockCount; i++)
            {
                auto& block = state.blocks[i];
                if (block.isAlive)
                {
                    block.Draw();
                }
            }

            DrawText(TextFormat("Lives =""% i", lives), 150, 10, 20, RAYWHITE);
            DrawText(TextFormat("Score =""% i", score), screenWidth - 150, 10, 20, RAYWHITE);
        } break;

        case ENDING:
        {
            if (losertext)
            {
                int textwidth = MeasureText(losertext, 60);
                int rtextwidth = MeasureText(restart, 30);
                DrawText(losertext, screenWidth / 2 - textwidth / 2, screenHeight / 2 - 30, 60, RED);
                DrawText(restart, screenWidth / 2 - rtextwidth / 2, screenHeight / 2 + 35, 30, YELLOW);
            }
        } break;

        default: break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    StopSoundMulti();

    UnloadTexture(player);
    UnloadSound(sound);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------


    return 0;
}
