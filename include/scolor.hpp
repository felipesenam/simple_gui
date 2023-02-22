#ifndef __SCOLOR_H__
#define __SCOLOR_H__

#include "score.hpp"

namespace PROJECT_NAMESPACE
{
    class Color : public SDL_Color
    {
    public:
        Color();
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        Color(unsigned hex);
        Color(const Color &color);

        Color &hsl(float h, float s, float l, Uint8 a = 255);
        Color &hsv(float h, float s, float v, Uint8 a = 255);
        Color &rgb(float r, float g, float b, Uint8 a = 255);
        Color &hex(unsigned hex);

        Color &operator=(unsigned hex);
        Color &operator=(const Color &color);

        friend std::ostream &operator<<(std::ostream &o, const Color &color)
        {
            o << "RGBA(" << static_cast<int>(color.r) << ", "
              << static_cast<int>(color.g) << ", "
              << static_cast<int>(color.b) << ", "
              << static_cast<int>(color.a) << ")";
            return o;
        }
    };
}
#endif // __SCOLOR_H__