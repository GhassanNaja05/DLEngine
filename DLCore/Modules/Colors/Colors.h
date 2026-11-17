#ifndef COLORS_H
#define COLORS_H
#include <string>
#include <raylib.h>


class Colors {
public:
    Colors() = delete;
    ~Colors() = delete;

    /// Create a color with r g b values
    static Color CreateColor(int r, int g, int b);

    /// Create a color from a hex string
    static Color CreateColorHex(const std::string& hex);

    /// Set the transparency of a color to a given value
    static void SetTransparency(Color& c, int a);

    static Color CreateRandomColor();
};



#endif
