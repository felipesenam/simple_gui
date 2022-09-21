#include "sapplication.hpp"

namespace PROJECT_NAMESPACE
{
    Application::WindowManager::WindowManager()
    {
    }

    Application::WindowManager::~WindowManager()
    {
        for (auto i = windows.begin(); i != windows.end();)
        {
            delete i->second;
            i = windows.erase(i);
        }
    }

    Window &Application::WindowManager::get(const std::string &id)
    {
        return *windows[id];
    }

    Window &Application::WindowManager::create(const std::string &id, const WindowConfig &windowConfig)
    {
        auto it = windows.find(id);
        if (it == windows.end())
        {
            windows[id] = new Window(windowConfig);
            return *windows[id];
        }
        return *it->second;
    }

    void Application::WindowManager::handleEvent(const SDL_Event &event)
    {
        for (auto &window : windows)
            window.second->handleEvent(event);
    }

    void Application::WindowManager::runLogic()
    {

        for (auto it = windows.begin(); it != windows.end();)
        {
            auto window = it->second;
            if (!window->isActive() && window->behavior.destroyOnClose)
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
    bool Application::WindowManager::hasActiveWindows() const
    {
        for (auto &it : windows)
        {
            auto window = it.second;
            if (window && window->isActive() && window->isShown())
                return true;
        }

        return false;
    }

    Application::Application(const WindowConfig &windowConfig)
    {
        windows.create("main", windowConfig);
    }
    Application::~Application()
    {
    }
    void Application::stop()
    {
        running = false;
    }
    int Application::run()
    {
        running = true;

        Uint32 fStart, frameTime;
        const double frameDelay = 1000 / (double)targetFPS;
        while (running && windows.hasActiveWindows())
        {
            fStart = SDL_GetTicks();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    break;
                }
                windows.handleEvent(event);
            }
            windows.runLogic();

            frameTime = SDL_GetTicks() - fStart;

            const float elapsed = frameTime / (float)SDL_GetPerformanceFrequency() * 1000.0f;

            SDL_Delay(std::floor(frameDelay - elapsed));
        }

        running = false;

        return 0;
    }

    int Application::operator()()
    {
        return run();
    }

    bool Application::isRunning() const
    {
        return running;
    }
}
