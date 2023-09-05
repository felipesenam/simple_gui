#include "swindow.hpp"

#include "widgets/sflex.hpp"
#include "sfont.hpp"
#include "scolors.hpp"

namespace PROJECT_NAMESPACE
{
    WindowManager::WindowManager()
    {
    }

    WindowManager::~WindowManager()
    {
        for (auto i = windows.begin(); i != windows.end(); ++i)
        {
            delete i->second;
        }
    }

    Window &WindowManager::get(const std::string &id)
    {
        return *windows[id];
    }

    Window &WindowManager::create(const std::string &id, const WindowConfig &windowConfig)
    {
        auto it = windows.find(id);
        if (it == windows.end())
        {
            windows[id] = new Window(windowConfig);
            return *windows[id];
        }
        return *it->second;
    }
    void WindowManager::init()
    {
        Debug("Initializing windows... (" << windows.size() << ")");
        for (auto &window : windows)
            window.second->init();
    }
    void WindowManager::handleEvent(const SDL_Event &event)
    {
        for (auto &window : windows)
            window.second->handleEvent(event);
    }

    void WindowManager::runLogic()
    {
        for (auto it = windows.begin(); it != windows.end();)
        {
            auto window = it->second;
            if (!window->isActive() && window->config.behavior.destroyOnClose)
            {
                delete window;
                it = windows.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto &window : windows)
        {
            window.second->update();
            window.second->draw();
        }
    }
    bool WindowManager::hasActiveWindows() const
    {
        for (auto &it : windows)
        {
            auto window = it.second;
            if (window && window->isActive() && window->isShown())
                return true;
        }

        return false;
    }
}

namespace PROJECT_NAMESPACE
{
    Window::Window(const WindowConfig &config) : config(config)
    {
        this->container = new Flex(*this);
    }
    Window::~Window()
    {
        delete container;

        SDL_DestroyWindow(window);
    }
    bool Window::isActive() const
    {
        return active;
    }
    bool Window::isShown() const
    {
        return shown;
    }
    void Window::show()
    {
        shown = true;
        SDL_ShowWindow(window);
    }
    void Window::hide()
    {
        shown = false;
        SDL_HideWindow(window);
    }
    void Window::destroy()
    {
        active = false;
    }
    void Window::init()
    {
        window = SDL_CreateWindow(
            config.title.c_str(),
            config.x,
            config.y,
            config.width,
            config.height,
            config.options.get());
        renderer.create(*this, config.renderer);

        container->events["windowSizeChanged"] = [&]()
        {
            auto size = this->size();
            container->geometry.abs.w = size.first;
            container->geometry.abs.h = size.second;
            container->geometry.normalize();
        };
        container->events["windowSizeChanged"].invoke();

        Debug("Window created: " << this->uid << " (" << config.width << "x" << config.height << ")");
    }
    void Window::handleEvent(const SDL_Event &event)
    {
        if (
            event.type == SDL_WINDOWEVENT &&
            SDL_GetWindowID(window) == event.window.windowID)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                if (config.behavior.destroyOnClose)
                    destroy();
                else
                    hide();
                break;

            default:
                break;
            }
        }
        keyboard.handleKeyboardEvent(event);

        container->handleGenericEvents(event);
        container->handleEvent(event);
    }
    void Window::update()
    {
        container->render();
        container->update();
    }
    void Window::draw()
    {
        renderer.clear();
        container->draw();
        renderer.present();
    }

    void to_json(json &j, const Window &p)
    {
        j["config"] = p.config;
        j["container"] = *p.container;
    }

    void from_json(const json &j, Window &p)
    {
        SETATTR_IF_JSON_CONTAINS(j, p, config);
        if (j.contains("container"))
        {
            from_json(j["container"], *p.container);
        }
    }
}

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