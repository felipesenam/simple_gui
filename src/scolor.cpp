#include "scolor.hpp"

namespace PROJECT_NAMESPACE
{
    Color::Color() : SDL_Color{0, 0, 0, 0}
    {
    }

    Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : SDL_Color{r, g, b, a}
    {
    }

    Color::Color(unsigned hex, Uint8 a)
    {
        this->hex(hex);
        this->a = a;
    }

    Color::Color(const Color &color)
    {
        *this = color;
    }

    Color &Color::hsl(float h, float s, float l, Uint8 a)
    {
        float C = (1 - fabs((2 * l) - 1)) * s;
        float X = C * (1 - fabs(fmod(h / 60., 2) - 1));
        float m = l - (C / 2);

        float rt, gt, bt;
        if (h >= 0 && h < 60)
        {
            rt = C;
            gt = X;
            bt = 0;
        }
        else if (h >= 60 && h < 120)
        {
            rt = X;
            gt = C;
            bt = 0;
        }
        else if (h >= 120 && h < 180)
        {
            rt = 0;
            gt = C;
            bt = X;
        }
        else if (h >= 180 && h < 240)
        {
            rt = 0;
            gt = X;
            bt = C;
        }
        else if (h >= 240 && h < 300)
        {
            rt = X;
            gt = 0;
            bt = C;
        }
        else
        {
            rt = C;
            gt = 0;
            bt = X;
        }

        this->r = (rt + m) * 255;
        this->g = (gt + m) * 255;
        this->b = (bt + m) * 255;
        this->a = a;

        return *this;
    }

    Color &Color::hsv(float h, float s, float v, Uint8 a)
    {
        float C = v * s;
        float X = C * (1 - fabs(fmod(h / 60., 2) - 1));
        float m = v - C;

        float rt, gt, bt;
        if (h >= 0 && h < 60)
        {
            rt = C;
            gt = X;
            bt = 0;
        }
        else if (h >= 60 && h < 120)
        {
            rt = X;
            gt = C;
            bt = 0;
        }
        else if (h >= 120 && h < 180)
        {
            rt = 0;
            gt = C;
            bt = X;
        }
        else if (h >= 180 && h < 240)
        {
            rt = 0;
            gt = X;
            bt = C;
        }
        else if (h >= 240 && h < 300)
        {
            rt = X;
            gt = 0;
            bt = C;
        }
        else
        {
            rt = C;
            gt = 0;
            bt = X;
        }

        this->r = (rt + m) * 255;
        this->g = (gt + m) * 255;
        this->b = (bt + m) * 255;
        this->a = a;

        return *this;
    }

    Color &Color::rgb(float r, float g, float b, Uint8 a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;

        return *this;
    }

    Color &Color::hex(unsigned hex)
    {
        this->r = (hex >> 24) & 0xFF;
        this->g = (hex >> 16) & 0xFF;
        this->b = (hex >> 8) & 0xFF;
        this->a = hex & 0xFF;

        return *this;
    }

    Color &Color::operator=(unsigned hex)
    {
        return this->hex(hex);
    }

    Color &Color::operator=(const Color &color)
    {
        return this->rgb(RGBA(color));
    }
}