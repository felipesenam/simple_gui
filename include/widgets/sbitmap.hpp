#ifndef F485EFCF_8079_4F33_91F7_A134EA3822B3
#define F485EFCF_8079_4F33_91F7_A134EA3822B3

#include "score.hpp"
#include "swidget.hpp"
#include "scolorgrid.hpp"

namespace PROJECT_NAMESPACE
{
    class Bitmap : public ColorGrid, public Widget, public Object<Bitmap>
    {
    private:
        SDL_Texture *tex = nullptr;

        bool m_render = false;

    public:
        Bitmap(Window &window, size_t w, size_t h);
        ~Bitmap();

        Color &at(size_t x, size_t y) override;

        void alloc(size_t width, size_t height) override;

        void erase() override;

        void render();
        void draw();
    };
}
#endif /* F485EFCF_8079_4F33_91F7_A134EA3822B3 */