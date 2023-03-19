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
        TTF_Font *ttf = nullptr;

        friend class Renderer;

    public:
        Font(const std::string &path, unsigned size = 12)
        {
            open(path, size);
        }
        ~Font()
        {
            TTF_CloseFont(ttf);
        }

        RenderType renderType = blended;

        void open(const std::string &path, unsigned size);
    };
}

#endif /* E23CAAA8_EE02_4059_A09F_C6B585C1316A */
