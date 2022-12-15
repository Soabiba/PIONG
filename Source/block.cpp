#include <raylib.h>

Color Light_Green = Color{ 34,139,34, 255 };

class Block
{
public:
    float x, y;
    float width, height;
    int   rowIndex;
    int   columnIndex;
    bool  isAlive;
    Vector2 position;
    Vector2 size;

    Rectangle GetRectangle()
    {
        return Rectangle{ x , y , width, height };
    }

    void Draw()
    {
        DrawRectangleRec(GetRectangle(), Light_Green);
    }
};