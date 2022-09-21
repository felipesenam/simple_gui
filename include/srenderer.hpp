#ifndef __SRENDERER_H__
#define __SRENDERER_H__

#include "score.hpp"
#include "scolor.hpp"

namespace PROJECT_NAMESPACE
{
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
                    (this->usesHardwareAcceleration ? SDL_RENDERER_ACCELERATED : 0) |
                    (this->usesVSync ? SDL_RENDERER_PRESENTVSYNC : 0));
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
    };
}

#endif // __SRENDERER_H__