#ifndef __SCOLORS_H__
#define __SCOLORS_H__

#include "score.hpp"

namespace sgui
{
    namespace Colors
    {
        enum pinkColors
        {
            Pink = 0xFFC0CBFF,
            HotPink = 0xFF69B4FF,
            LightPink = 0xFFB6C1FF,
            DeepPink = 0xFF1493FF,
            PaleVioletRed = 0xDB7093FF,
            MediumVioletRed = 0xC71585FF,
        };
        enum purpleColors
        {
            Lavender = 0xE6E6FAFF,
            Thistle = 0xD8BFD8FF,
            Plum = 0xDDA0DDFF,
            Orchid = 0xDA70D6FF,
            Violet = 0xEE82EEFF,
            Fuchsia = 0xFF00FFFF,
            Magenta = 0xFF00FFFF,
            MediumOrchid = 0xBA55D3FF,
            DarkOrchid = 0x9932CCFF,
            DarkViolet = 0x9400D3FF,
            BlueViolet = 0x8A2BE2FF,
            DarkMagenta = 0x8B008BFF,
            Purple = 0x800080FF,
            MediumPurple = 0x9370DBFF,
            MediumSlateBlue = 0x7B68EEFF,
            SlateBlue = 0x6A5ACDFF,
            DarkSlateBlue = 0x483D8BFF,
            RebeccaPurple = 0x663399FF,
            Indigo = 0x4B0082FF,
        };
        enum redColors
        {
            LightSalmon = 0xFFA07AFF,
            Salmon = 0xFA8072FF,
            DarkSalmon = 0xE9967AFF,
            LightCoral = 0xF08080FF,
            IndianRed = 0xCD5C5CFF,
            Crimson = 0xDC143CFF,
            Red = 0xFF0000FF,
            FireBrick = 0xB22222FF,
            DarkRed = 0x8B0000FF,
        };
        enum orangeColors
        {
            Orange = 0xFFA500FF,
            DarkOrange = 0xFF8C00FF,
            Coral = 0xFF7F50FF,
            Tomato = 0xFF6347FF,
            OrangeRed = 0xFF4500FF,
        };
        enum yellowColors
        {
            Gold = 0xFFD700FF,
            Yellow = 0xFFFF00FF,
            LightYellow = 0xFFFFE0FF,
            LemonChiffon = 0xFFFACDFF,
            LightGoldenrodYellow = 0xFAFAD2FF,
            PapayaWhip = 0xFFEFD5FF,
            Moccasin = 0xFFE4B5FF,
            PeachPuff = 0xFFDAB9FF,
            PaleGoldenrod = 0xEEE8AAFF,
            Khaki = 0xF0E68CFF,
            DarkKhaki = 0xBD536BFF,
        };
        enum greenColors
        {
            GreenYellow = 0xADFF2FFF,
            Chartreuse = 0x7FFF00FF,
            LawnGreen = 0x7CFC00FF,
            Lime = 0x00FF00FF,
            LimeGreen = 0x32CD32FF,
            PaleGreen = 0x98FB98FF,
            LightGreen = 0x90EE90FF,
            MediumSpringGreen = 0x00FA9AFF,
            SpringGreen = 0x00FF7FFF,
            MediumSeaGreen = 0x3CB371FF,
            SeaGreen = 0x2E8B57FF,
            ForestGreen = 0x228B22FF,
            Green = 0x008000FF,
            DarkGreen = 0x006400FF,
            YellowGreen = 0x9ACD32FF,
            OliveDrab = 0x6B8E23FF,
            DarkOliveGreen = 0x556B2FFF,
            MediumAquamarine = 0x66CDAAFF,
            DarkSeaGreen = 0x8FBC8FFF,
            LightSeaGreen = 0x20B2AAFF,
            DarkCyan = 0x008B8BFF,
            Teal = 0x008080FF,
        };
        enum cyanColors
        {
            Aqua = 0x00FFFFFF,
            Cyan = 0x00FFFFFF,
            LightCyan = 0xE0FFFFFF,
            PaleTurquoise = 0xAFEEEEFF,
            Aquamarine = 0x7FFFD4FF,
            Turquoise = 0x40E0D0FF,
            MediumTurquoise = 0x48D1CCFF,
            DarkTurquoise = 0x00CED1FF,
        };
        enum blueColors
        {
            CadetBlue = 0x5F9EA0FF,
            SteelBlue = 0x4682B4FF,
            LightSteelBlue = 0xB0C4DEFF,
            LightBlue = 0xADD8E6FF,
            PowderBlue = 0xB0E0E6FF,
            LightSkyBlue = 0x87CEFAFF,
            SkyBlue = 0x87CEEBFF,
            CornflowerBlue = 0x6495EDFF,
            DeepSkyBlue = 0x00BFFFFF,
            DodgerBlue = 0x1E90FFFF,
            RoyalBlue = 0x4169E1FF,
            Blue = 0x0000FFFF,
            MediumBlue = 0x0000CDFF,
            DarkBlue = 0x00008BFF,
            Navy = 0x000080FF,
            MidnightBlue = 0x191970FF,
        };
        enum brownColors
        {
            Cornsilk = 0xFFF8DCFF,
            BlanchedAlmond = 0xFFEBCDFF,
            Bisque = 0xFFE4C4FF,
            NavajoWhite = 0xFFDEADFF,
            Wheat = 0xF5DEB3FF,
            BurlyWood = 0xDEB887FF,
            Tan = 0xD2B48CFF,
            RosyBrown = 0xBC8F8FFF,
            SandyBrown = 0xF4A460FF,
            Goldenrod = 0xDAA520FF,
            DarkGoldenrod = 0xB8860BFF,
            Peru = 0xCD853FFF,
            Chocolate = 0xD2691EFF,
            Olive = 0x808000FF,
            SaddleBrown = 0x8B4513FF,
            Sienna = 0xA0522DFF,
            Brown = 0xA52A2AFF,
            Maroon = 0x800000FF,
        };
        enum whiteColors
        {
            White = 0xFFFFFFFF,
            Snow = 0xFFFAFAFF,
            Honeydew = 0xF0FFF0FF,
            MintCream = 0xF5FFFAFF,
            Azure = 0xF0FFFFFF,
            AliceBlue = 0xF0F8FFFF,
            GhostWhite = 0xF8F8FFFF,
            WhiteSmoke = 0xF5F5F5FF,
            Seashell = 0xFFF5EEFF,
            Beige = 0xF5F5DCFF,
            OldLace = 0xFDF5E6FF,
            FloralWhite = 0xFFFAF0FF,
            Ivory = 0xFFFFF0FF,
            AntiqueWhite = 0xFAEBD7FF,
            Linen = 0xFAF0E6FF,
            LavenderBlush = 0xFFF0F5FF,
            MistyRose = 0xFFE4E1FF,
        };
        enum greyColors
        {
            Gainsboro = 0xDCDCDCFF,
            LightGray = 0xD3D3D3FF,
            Silver = 0xC0C0C0FF,
            DarkGray = 0xA9A9A9FF,
            DimGray = 0x696969FF,
            Gray = 0x808080FF,
            LightSlateGray = 0x778899FF,
            SlateGray = 0x708090FF,
            DarkSlateGray = 0x2F4F4FFF,
            Black = 0x000000FF
        };
    }
}

#endif // __SCOLORS_H__