#ifndef __SCOLOR_H__
#define __SCOLOR_H__

#include "score.hpp"

namespace PROJECT_NAMESPACE
{
    class Color : public SDL_Color
    {
    private:
    public:
        Color();
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        Color(int hex, Uint8 a = 255);
        Color(const Color &color);

        Color &hsl(float h, float s, float l, Uint8 a = 255);
        Color &hsv(float h, float s, float v, Uint8 a = 255);
        Color &rgb(float r, float g, float b, Uint8 a = 255);
        Color &hex(int hex, Uint8 a = 255);

        Color &operator=(int hex);
        Color &operator=(const Color &color);
    };
}
#endif // __SCOLOR_H__