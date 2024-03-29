#include "sfont.hpp"

namespace sgui
{
    void Font::open(const std::string &path, unsigned size)
    {
        if (ttf)
        {
            TTF_CloseFont(ttf);
        }

        ttf = TTF_OpenFont(path.c_str(), size);
        if (!ttf)
        {
            Error(TTF_GetError());
        }
    }
}