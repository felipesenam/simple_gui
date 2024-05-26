#include "ssdl.hpp"

namespace sgui
{
    Font::Font(const std::string &path, unsigned size)
    {
        open(path, size);
    }
    Font::~Font()
    {
        TTF_CloseFont(ttf);
    }

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
    Surface *Font::render(const std::string &text, unsigned wrapLenght)
    {
        SDL_Surface *surface = nullptr;
        if (wrapLenght)
        {
            switch (renderType)
            {
            case Font::blended:
                surface = TTF_RenderUTF8_Blended_Wrapped(ttf, text.c_str(), {255, 255, 255, 255}, wrapLenght);
                break;
            case Font::solid:
                surface = TTF_RenderUTF8_Solid_Wrapped(ttf, text.c_str(), {255, 255, 255, 255}, wrapLenght);
                break;
            case Font::shaded:
                surface = TTF_RenderUTF8_Shaded_Wrapped(ttf, text.c_str(), {255, 255, 255, 255}, background, wrapLenght);
                break;
            }
        }
        else
        {
            switch (renderType)
            {
            case Font::blended:
                surface = TTF_RenderUTF8_Blended(ttf, text.c_str(), {255, 255, 255, 255});
                break;
            case Font::solid:
                surface = TTF_RenderUTF8_Solid(ttf, text.c_str(), {255, 255, 255, 255});
                break;
            case Font::shaded:
                surface = TTF_RenderUTF8_Shaded(ttf, text.c_str(), {255, 255, 255, 255}, background);
                break;
            }
        }

        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
        return new Surface(surface);
    }
}

namespace sgui
{
    Renderer::Renderer(Window &window, const RendererConfig &rendererConfig)
    {
        this->create(window, rendererConfig);
    }

    Renderer::~Renderer()
    {
        this->destroy();
    }

    bool Renderer::isCreated() const noexcept
    {
        return renderer != nullptr;
    }

    void Renderer::create(Window &window, const RendererConfig &rendererConfig)
    {
        if (!renderer)
        {
            renderer = SDL_CreateRenderer(
                window.window,
                rendererConfig.index,
                rendererConfig.options.get());
            this->drawColor = rendererConfig.drawColor;
            if (SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(rendererConfig.blendMode)))
            {
                Warn(SDL_GetError());
                SDL_ClearError();
            }
        }
    }

    void Renderer::destroy()
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    void Renderer::clear()
    {
        SDL_SetRenderDrawColor(renderer, RGBA(drawColor));
        SDL_RenderClear(renderer);
    }
    void Renderer::present()
    {
        SDL_RenderPresent(renderer);
    }

    Texture *Renderer::createTextureFromSurface(Surface &surface)
    {
        return new Texture(*this, surface);
    }
    void Renderer::drawTexture(Texture &texture, Rect *src, Rect *dest)
    {

        if (SDL_RenderCopy(renderer, texture.texture, src, dest) < 0)
        {
            SDL_PrintError(Error);
            drawFillRectangle(*dest, Color(255, 0, 0, 122));
        }

        DebugFrame(*dest, Color(255, 0, 0));
        if (dest->w <= 0 || dest->h <= 0)
            return;
    }

    void Renderer::drawRectangle(const Rect &dest, const Color &color)
    {
        if (color.a == 0)
            return;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &dest);
        SDL_SetRenderDrawColor(renderer, RGBA(drawColor));
    }

    void Renderer::drawCross(const Rect &dest, const Color &color)
    {
        if (color.a == 0)
            return;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        const int mw = dest.w / 2;
        const int mh = dest.h / 2;
        SDL_RenderDrawLine(renderer, dest.x + mw, dest.y, dest.x + mw, dest.y + dest.h);
        SDL_RenderDrawLine(renderer, dest.x, dest.y + mh, dest.x + dest.w, dest.y + mh);
        SDL_SetRenderDrawColor(renderer, RGBA(drawColor));
    }

    void Renderer::drawFillRectangle(const Rect &dest, const Color &color)
    {
        if (color.a == 0)
            return;

        SDL_SetRenderDrawColor(renderer, RGBA(color));
        SDL_RenderFillRect(renderer, &dest);
        SDL_SetRenderDrawColor(renderer, RGBA(drawColor));
    }

    Texture *Renderer::renderText(const std::string &text, Font &font, Rect &rect, Uint32 wrapLenght)
    {
        if (font.ttf == nullptr)
        {
            Warn("Invalid font.");
            return nullptr;
        }

        Surface *surface = font.render(text, wrapLenght);

        Texture *texture = this->createTextureFromSurface(*surface);
        delete surface;

        texture->query(nullptr, nullptr, &rect.w, &rect.h);

        return texture;
    }
}

namespace sgui
{
    Color::Color() : SDL_Color{0, 0, 0, 255}
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

namespace sgui
{
    Texture::Texture() {}
    Texture::Texture(Renderer &renderer, Surface &surface)
    {
        texture = SDL_CreateTextureFromSurface(renderer.renderer, surface.surface);
        if (texture == nullptr)
        {
            SDL_PrintError(Error);
        }
    }
    Texture::~Texture()
    {
        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
            SDL_PrintIfError(Warn);
        }
    }

    void Texture::modColor(const Color &color)
    {
        if (SDL_SetTextureColorMod(texture, RGB(color)) != 0)
        {
            Warn(SDL_GetError());
        }
    }
    void Texture::query(Uint32 *format, int *access, int *w, int *h)
    {
        if (texture != nullptr)
            SDL_QueryTexture(texture, format, access, w, h);
    }
}

namespace sgui
{
    Surface::Surface(const std::string &file) : Surface(IMG_Load(file.c_str()))
    {
        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
    }
    Surface::Surface(SDL_Surface *surface) : surface(surface)
    {
        if (surface)
        {
            m_width = surface->w;
            m_height = surface->h;
        }
    }
    Surface::Surface(int width, int height) : m_width(width), m_height(height)
    {
        // TODO: Understand why this make wireframe to not work
        surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
        // surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
    }
    Surface::~Surface()
    {
        SDL_FreeSurface(surface);
    }

    void Surface::toJPG(const std::string &file, int quality) const
    {
        if (IMG_SaveJPG(surface, file.c_str(), quality) < 0)
        {
            SDL_PrintError(Error);
        }
    }
    void Surface::toPNG(const std::string &file) const
    {
        if (IMG_SavePNG(surface, file.c_str()) < 0)
        {
            SDL_PrintError(Error);
        }
    }
    void Surface::toBMP(const std::string &file) const
    {
        if (SDL_SaveBMP(surface, file.c_str()) < 0)
        {
            SDL_PrintError(Error);
        }
    }

    void Surface::toPPMA(const std::string &file) const
    {
        std::ofstream save;
        save.open(file);

        size_t w = width();
        size_t h = height();

        save << "P3\r";
        save << w << " " << h << "\r";
        save << "255\r";

        for (size_t i = 0; i < h; ++i)
        {
            for (size_t j = 0; j < w; ++j)
            {
                auto pixel = get(i, j);
                save << pixel.r << " " << pixel.g << " " << pixel.b << "\r";
            }
        }

        save.close();
    }
    void Surface::toPPMB(const std::string &file) const
    {
        std::ofstream save;
        save.open(file, std::ios_base::binary);

        size_t w = width();
        size_t h = height();

        save << "P6\r";
        save << w << " " << h << "\r";
        save << "255\r";

        for (size_t i = 0; i < h; ++i)
        {
            for (size_t j = 0; j < w; ++j)
            {
                auto pixel = get(i, j);
                save << (unsigned char)pixel.r << (unsigned char)pixel.g << (unsigned char)pixel.b;
            }
        }

        save.close();
    }
}