#include "Colors.h"

#include "../Random/Random.h"

Color Colors::CreateColor(const int r, const int g, const int b) { return Color(r, g, b, 255); }

Color Colors::CreateColorHex(const std::string &hex) {
    if (hex.length() != 7 || hex[0] != '#')
        return Color(0, 0, 0);

    // 0 1 2 3 4 5 6 7 8 9 a b c d e f

    auto to_hex = [](char c) -> int {
        c = std::tolower(c);
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        return -1;
    };

    auto convert = [to_hex](char a, char b) {
        a = to_hex(a);
        b = to_hex(b);

        if (a > 15 || b > 15)
            return 0;

        return a * 16 + b;
    };

    return CreateColor(convert(hex[1], hex[2]), convert(hex[3], hex[4]), convert(hex[5], hex[6]));
}

void Colors::SetTransparency(Color &c, const int a) { c.a = a; }

Color Colors::CreateRandomColor() {
    return Color(
        Random::RandomInt(256),
        Random::RandomInt(256),
        Random::RandomInt(256),
        255
    );
}
