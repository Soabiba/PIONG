#include <raylib.h>

class Ball
{
public:
    float x, y;
    float speedx, speedy;
    float radius;

    void Draw()
    {
        DrawCircle((int)x, (int)y, radius, RAYWHITE);
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };
        speedx *= speed_choices[GetRandomValue(0, 1)];
        speedy *= speed_choices[GetRandomValue(0, 1)];
    }
}; 
Ball ball;