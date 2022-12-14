#ifndef E23CAAA8_EE02_4059_A09F_C6B585C1316A
#define E23CAAA8_EE02_4059_A09F_C6B585C1316A

#include "scolor.hpp"
#include "score.hpp"

namespace PROJECT_NAMESPACE
{

    enum RenderType
    {
        blended,
        solid,
        shaded
    };

    class Font
    {
    private:
        static std::map<std::pair<std::string, unsigned>, TTF_Font *> fonts;

        TTF_Font *font = nullptr;

        friend class Renderer;

        static int err;
        static void atexit_handler()
        {
            Debug("Closing fonts");
            for (auto pair : fonts)
            {
                TTF_CloseFont(pair.second);
            }
        }

    public:
        Font(const std::string &path, unsigned size = 12)
        {
            open(path, size);
        }

        Color color, background;
        RenderType renderType = blended;

        void open(const std::string &path, unsigned size);
    };
}

#endif /* E23CAAA8_EE02_4059_A09F_C6B585C1316A */
