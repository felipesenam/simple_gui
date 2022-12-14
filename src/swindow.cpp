#include "swindow.hpp"
namespace PROJECT_NAMESPACE
{
    WindowManager::WindowManager()
    {
    }

    WindowManager::~WindowManager()
    {
        for (auto i = windows.begin(); i != windows.end();)
        {
            delete i->second;
            i = windows.erase(i);
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
    Window::Window(const WindowConfig &config) : widgets(self), config(config)
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
                if (config.behavior.destroyOnClose)
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
        widgets.update();
    }
    void Window::draw()
    {
        renderer.clear();
        widgets.draw();
        renderer.present();
    }
}