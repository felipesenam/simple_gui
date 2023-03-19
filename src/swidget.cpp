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
    void WidgetManager::render()
    {
        for (auto widget : widgets)
            widget->render();
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
    template <>
    const int Object<Widget>::err = std::atexit(Object<Widget>::atexit_handler);

    Widget::Widget(Window &window) : window(window), geometry(self), font(window.config.defaultFontPath, window.config.defaultFontSize)
    {
        window.container.add(self);
    }

    void Widget::handleGenericEvents(const SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_WINDOWEVENT:
        {
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                events["windowSizeChanged"].triggered = true;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                events["windowResized"].triggered = true;
                break;
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            SDL_Point mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            bool isInside = SDL_PointInRect(&mousePos, &geometry.dest);
            if (isInside)
            {
                if (!m_isHovered)
                {
                    events["hover"].triggered = m_isHovered = true;
                }
            }
            else
            {
                m_isHovered = false;
            }

            events["hovering"].triggered = isInside;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            events["pressed"].triggered = events["hovering"].triggered;
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (m_isPressed)
            {
                events["clicked"].triggered = true;
            }
            break;
        }
        }
    }

    void Widget::handleEvent(const SDL_Event &e)
    {
        handleGenericEvents(e);
    }
    void Widget::render()
    {
    }
    void Widget::update()
    {
        events.perform();
    }
    void Widget::draw()
    {
        window.renderer.drawFillRectangle(geometry.dest, Colors::Red);
    }
}