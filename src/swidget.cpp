#include "swidget.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    WidgetManager::WidgetManager(Window &window) : Widget(window)
    {
    }

    WidgetManager::~WidgetManager()
    {
        for (auto widget : widgets)
            delete widget;
    }

    void WidgetManager::handleEvent(const SDL_Event &e)
    {
        for (auto widget : widgets)
            widget->handleEvent(e);
    }
    void WidgetManager::update()
    {
        for (auto widget : widgets)
            widget->update();
    }
    void WidgetManager::draw()
    {
        for (auto widget : widgets)
            widget->draw();
    }

}

namespace PROJECT_NAMESPACE
{
    Widget::Widget(Window &window) : parent(&window.container), window(window), geometry(self), font(window.config.defaultFontPath, window.config.defaultFontSize)
    {
    }

    void Widget::handleWindowEvents(const SDL_Event &e)
    {
        if (e.type == SDL_WINDOWEVENT)
        {
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                events.onWindowSizeChanged(e.window.data1, e.window.data2);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                events.onWindowResized(e.window.data1, e.window.data2);
                break;
            }
        }
    }

    void Widget::handleEvent(const SDL_Event &)
    {
    }
    void Widget::render()
    {
    }
    void Widget::update()
    {
    }
    void Widget::draw()
    {
    }
}