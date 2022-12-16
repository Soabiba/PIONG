#include <raylib.h>
#include "block.h"

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
        maxRows = 7;
        maxColumns = 15;

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