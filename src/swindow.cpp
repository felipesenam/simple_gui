#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Window::Window(const WindowConfig &config)
    {
        window = SDL_CreateWindow(
            config.title.c_str(),
            config.x,
            config.y,
            config.width,
            config.height,
            config.options.get());
        renderer.create(self, config.renderer);
    }
    Window::~Window()
    {
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
    void Window::handleEvent(const SDL_Event &event)
    {
        if (
            event.type == SDL_WINDOWEVENT &&
            SDL_GetWindowID(window) == event.window.windowID)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                if (behavior.destroyOnClose)
                    destroy();
                else
                    hide();
                break;

            default:
                break;
            }
        }
    }
    void Window::update()
    {
        // UPDATE LOGIC
    }
    void Window::draw()
    {
        renderer.clear();
        // DRAW LOGIC
        renderer.present();
    }
}