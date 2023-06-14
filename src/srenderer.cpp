#include "srenderer.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
        self.destroy();
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
            self.drawColor = rendererConfig.drawColor;
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
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, RGBA(drawColor));
    }
    void Renderer::present()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *Renderer::createTextureFromSurface(SDL_Surface *surface)
    {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
        if (tex == nullptr)
        {
            SDL_PrintError(Error);
        }
        return tex;
    }
    void Renderer::drawTexture(SDL_Texture *texture, Rect *src, Rect *dest)
    {
        DebugFrame(*dest, Color(255, 0, 0));
        if (dest->w <= 0 || dest->h <= 0)
            return;

        if (SDL_RenderCopy(renderer, texture, src, dest) < 0)
        {
            SDL_PrintError(Error);
            drawFillRectangle(*dest, Color(255, 0, 0, 122));
        }
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

    SDL_Texture *Renderer::renderText(const std::string &text, Font &font, Geometry &geometry, const Color &background, Uint32 wrapLenght)
    {
        if (font.ttf == nullptr)
        {
            Warn("Invalid font.");
            return nullptr;
        }

        SDL_Surface *textSurface = nullptr;
        if (wrapLenght)
        {
            switch (font.renderType)
            {
            case Font::blended:
                textSurface = TTF_RenderUTF8_Blended_Wrapped(font.ttf, text.c_str(), {255, 255, 255, 255}, wrapLenght);
                break;
            case Font::solid:
                textSurface = TTF_RenderUTF8_Solid_Wrapped(font.ttf, text.c_str(), {255, 255, 255, 255}, wrapLenght);
                break;
            case Font::shaded:
                textSurface = TTF_RenderUTF8_Shaded_Wrapped(font.ttf, text.c_str(), {255, 255, 255, 255}, background, wrapLenght);
                break;
            }
        }
        else
        {
            switch (font.renderType)
            {
            case Font::blended:
                textSurface = TTF_RenderUTF8_Blended(font.ttf, text.c_str(), {255, 255, 255, 255});
                break;
            case Font::solid:
                textSurface = TTF_RenderUTF8_Solid(font.ttf, text.c_str(), {255, 255, 255, 255});
                break;
            case Font::shaded:
                textSurface = TTF_RenderUTF8_Shaded(font.ttf, text.c_str(), {255, 255, 255, 255}, background);
                break;
            }
        }
        if (textSurface == nullptr)
        {
            SDL_PrintError(Error);
        }

        SDL_Texture *texture = nullptr;
        texture = createTextureFromSurface(textSurface);
        SDL_FreeSurface(textSurface);

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        geometry.abs.w = w;
        geometry.abs.h = h;

        return texture;
    }
}