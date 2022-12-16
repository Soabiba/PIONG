#include "raylib.h"
#include "ball.cpp"
#include "paddle.cpp"
#include "block.cpp"

typedef enum GameScreen { TITLE, GAMEPLAY, ENDING } GameScreen;

class GameState
{
public:
    GameState() = default;

    // prevent copy construction, use .reset() instead
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
    {
        reset();
    }

    void reset()
    {
        maxRows = 8;
        maxColumns = 16;

        ball.x = screenWidth / 2;
        ball.y = screenHeight / 2;
        ball.radius = 10;
        ball.speedx = 300;
        ball.speedy = 300;

        paddle.x = screenWidth / 2;
        paddle.y = screenHeight - 20;
        paddle.width = 150;
        paddle.height = 15;
        paddle.speed = 500;

        Vector2 blockPositions[] =
        {
            { 1, 1 },
            { 1, 2 },
            { 1, 3 },
            { 2, 4 },
        };

        if (blocks)
        {
            delete[] blocks;
        }

        blockCount = sizeof(blockPositions) / sizeof(*blockPositions);
        blocks = new Block[blockCount];
        for (int i = 0; i < blockCount; i++)
        {
            blocks[i] = Block(blockPositions[i].x, blockPositions[i].y, maxRows, maxColumns, screenWidth, screenHeight);
        }
    }

    ~GameState()
    {
        if (blocks)
        {
            delete[] blocks;
        }
    }

    int maxRows{};
    int maxColumns{};
    int screenWidth, screenHeight;
    Ball ball{};
    Paddle paddle{};
    Block* blocks{};
    int blockCount{};
};

int main(void)
{
    const int screenWidth = 800;
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

    Texture2D player;
    player = LoadTexture("C:\\Users\\frostfox\\wkspaces\\\\PIONG\\assignment\\Source\\Resources\\player.jpg");
    Sound sound;
    sound = LoadSound("C:\\Users\\frostfox\\wkspaces\\\\PIONG\\assignment\\Source\\Resources\\sound.wav");
    
    GameScreen screen = TITLE;
    int framesCounter = 0;
    int gameResult = -1;
    bool gamePaused = false;

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
                            ball.speedy *= -1;
                        }
                        shouldGameContinue = true;
                    }
                }

                if (!shouldGameContinue)
                {
                    lives = 3;
                    losertext = "YOU WIN!!!";
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

    UnloadTexture(player);
    UnloadSound(sound);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}