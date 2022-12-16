#include <raylib.h>

class Paddle
{
public:
    float x, y;
    float speed;
    float width, height;

    Rectangle GetRectangle()
    {
        return Rectangle{ x - width / 2, y - height / 2, width, height };
    }

    void Draw()
    {
        DrawRectangleRec(GetRectangle(), RAYWHITE);
    }
};
