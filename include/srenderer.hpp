#ifndef __SRENDERER_H__
#define __SRENDERER_H__

#include "scolor.hpp"
#include "score.hpp"
#include "sfont.hpp"
#include "sgeometry.hpp"

namespace PROJECT_NAMESPACE
{
    using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
    using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;
    class Window;
    struct RendererConfig
    {
        int index = -1;
        Color drawColor;

        struct Options
        {
            // SDL_RENDERER_ACCELERATED
            bool usesHardwareAcceleration = false;

            // SDL_RENDERER_PRESENTVSYNC
            bool usesVSync = true;

            Uint32 get() const noexcept
            {
                return (
                    (self.usesHardwareAcceleration ? SDL_RENDERER_ACCELERATED : 0) |
                    (self.usesVSync ? SDL_RENDERER_PRESENTVSYNC : 0));
            }
        };
        Options options;

        enum BlendMode
        {
            none = SDL_BLENDMODE_NONE,
            alpha = SDL_BLENDMODE_BLEND,
            additive = SDL_BLENDMODE_ADD,
            modulate = SDL_BLENDMODE_MOD,
            multiply = SDL_BLENDMODE_MUL
        };
        BlendMode blendMode = alpha;
    };
    class Renderer
    {
    private:
        SDL_Renderer *renderer = nullptr;

    public:
        Renderer();
        ~Renderer();

        Color drawColor;

        bool isCreated() const noexcept;

        void create(Window &, const RendererConfig &);
        void destroy();

        void clear();
        void present();

        SDL_Texture *createTextureFromSurface(SDL_Surface *surface);
        void drawTexture(SDL_Texture *texture, Rect *src, Rect *dest);
        void drawRectangle(const Rect &dest, const Color &color);
        void drawCross(const Rect &dest, const Color &color);
        void drawFillRectangle(const Rect &dest, const Color &color);
        SDL_Texture *renderText(const std::string &text, Font &font, Geometry &geometry, const Color &background, Uint32 wrapLenght = 0);
    };
}

#endif // __SRENDERER_H__