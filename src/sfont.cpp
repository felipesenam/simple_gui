#include "sfont.hpp"

namespace PROJECT_NAMESPACE
{
    std::map<std::pair<std::string, unsigned>, TTF_Font *> Font::fonts;
    int Font::err = std::atexit(Font::atexit_handler);

    void Font::open(const std::string &path, unsigned size)
    {
        auto it = std::find_if(
            fonts.begin(), fonts.end(),
            [&](std::pair<std::pair<std::string, unsigned>, TTF_Font *> pair)
            {
                auto info = pair.first;
                return info.first == path && info.second == size;
            });

        if (it != fonts.end())
        {
            font = it->second;
        }
        else
        {
            font = TTF_OpenFont(path.c_str(), size);
            if (font)
            {
                fonts.emplace(std::pair<std::string, int>(path, size), font);
            }
            else
            {
                Error("'" << path << "' is not a path for a valid font. (did you enter the correct path?)");
                Error(TTF_GetError());
            }
        }
    }
}