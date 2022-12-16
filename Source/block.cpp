#include <raylib.h>

Color Light_Green = Color{ 34,139,34, 255 };

class Block
{
public:
    Block() = default;

    Block(int rowIndex, int columnIndex, int maxRows, int maxColumns, int screenWidth, int screenHeight)
    {
        position.x = (static_cast<float>(rowIndex) / maxRows) * screenWidth;
        position.y = (static_cast<float>(columnIndex) / maxColumns) * screenHeight;
        size.x = static_cast<float>(screenWidth) / maxRows;
        size.y = static_cast<float>(screenHeight) / maxColumns;

        // 0.80 == 80% of the available grid cell
        const float widthPadding = 0.80f;
        const float heightPadding = 0.80f;
        position.x += size.x * (1.f - widthPadding);
        position.y += size.y * (1.f - heightPadding);
        size.x *= widthPadding;
        size.y *= heightPadding;
    }

    bool  isAlive = true;
    Vector2 position{};
    Vector2 size{};

    Rectangle GetRectangle()
    {
        return Rectangle{ position.x, position.y , size.x, size.y };
    }

    void Draw()
    {
        DrawRectangleRec(GetRectangle(), Light_Green);
    }
};
