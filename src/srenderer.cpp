#include "srenderer.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Renderer::Renderer()
    {
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
            SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(rendererConfig.blendMode));
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
        SDL_SetRenderDrawColor(
            renderer,
            drawColor.r,
            drawColor.g,
            drawColor.b,
            drawColor.a);
    }
    void Renderer::present()
    {
        SDL_RenderPresent(renderer);
    }
}