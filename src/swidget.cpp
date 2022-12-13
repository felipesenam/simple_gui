#include "swidget.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    WidgetManager::WidgetManager(Window &window) : window(window)
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
    Widget::Widget(Window &window) : window(window)
    {
    }
    void Widget::handleEvent(const SDL_Event &)
    {
    }
    void Widget::update()
    {
    }
    void Widget::draw()
    {
    }
}