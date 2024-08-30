#include "swindow.hpp"

#include "swidgets.hpp"
#include "scolors.hpp"

namespace sgui
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

    ApplicationWindow &WindowManager::get(const std::string &id)
    {
        return *windows[id];
    }

    ApplicationWindow &WindowManager::create(const std::string &id, const ApplicationWindowConfig &windowConfig)
    {
        auto it = windows.find(id);
        if (it == windows.end())
        {
            windows[id] = new ApplicationWindow(windowConfig);
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

namespace sgui
{
    ApplicationWindow::ApplicationWindow(const ApplicationWindowConfig &config) : config(config)
    {
        this->container = new Flex(*this);
        this->container->geometry.behavior = Geometry::fill;
    }
    ApplicationWindow::~ApplicationWindow()
    {
        delete container;
        delete renderer;
        delete window;
    }
    bool ApplicationWindow::isActive() const
    {
        return active;
    }
    bool ApplicationWindow::isShown() const
    {
        return shown;
    }
    void ApplicationWindow::show()
    {
        shown = true;
        window->show();
    }
    void ApplicationWindow::hide()
    {
        shown = false;
        window->hide();
    }
    void ApplicationWindow::destroy()
    {
        active = false;
    }
    void ApplicationWindow::init()
    {
        window = new Window(config.window);
        renderer = new Renderer(*window, config.renderer);

        Debug("Window created: " << this->uid << " (" << config.window.width << "x" << config.window.height << ")");
    }
    void ApplicationWindow::handleEvent(const SDL_Event &event)
    {
        if (
            event.type == SDL_WINDOWEVENT &&
            window->getId() == event.window.windowID)
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
    void ApplicationWindow::update()
    {
        renderer->clear();

        container->render();
        container->update();
    }
    void ApplicationWindow::draw()
    {
        container->drawCommonElements();
        container->preUpdate();
        container->draw();
        renderer->present();
    }

    void to_json(json &j, const ApplicationWindow &p)
    {
        j["config"] = p.config;
        j["container"] = *p.container;
    }

    void from_json(const json &j, ApplicationWindow &p)
    {
        SETATTR_IF_JSON_CONTAINS(j, p, config);
        if (j.contains("container"))
        {
            from_json(j["container"], *p.container);
        }
    }
}
