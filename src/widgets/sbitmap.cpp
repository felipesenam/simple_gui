
#include "widgets/sbitmap.hpp"

#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Bitmap::Bitmap(Window &window, size_t w, size_t h) : Widget(window)
    {
        alloc(w, h);
    }

    Bitmap::~Bitmap()
    {
        erase();
    }

    Color &Bitmap::at(size_t x, size_t y)
    {
        m_render = true;
        return data[y][x];
    }

    void Bitmap::alloc(size_t width, size_t height)
    {
        erase();
        geometry.abs.w = this->w = width;
        geometry.abs.h = this->h = height;

        data = new Color *[height];
        for (size_t i = 0; i < height; ++i)
            data[i] = new Color[width];
        surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
        m_render = true;
    }

    void Bitmap::erase()
    {
        if (w == 0 || h == 0)
            return;

        SDL_FreeSurface(surface);
        if (tex != nullptr)
            SDL_DestroyTexture(tex);
        SDL_PrintIfError(Warn);

        geometry.abs.w = w = 0;
        geometry.abs.h = h = 0;

        data = nullptr;
    }

    void Bitmap::render()
    {
        if (m_render)
        {
            if (tex != NULL)
                SDL_DestroyTexture(tex);
            SDL_PrintIfError(Warn);

            updateSurface();
            tex = window.renderer.createTextureFromSurface(surface);

            m_render = false;
            this->geometry.normalize();
        }
    }

    void Bitmap::draw()
    {
        window.renderer.drawTexture(tex, &geometry.src, &geometry.dest);
    }
}