#include "swidget.hpp"
#include "swindow.hpp"
#include "sscheme.hpp"

namespace PROJECT_NAMESPACE
{
    WidgetManager::WidgetManager(Window &window) : Widget(window)
    {
        scheme = UI_TRANSPARENT_COLOR_SCHEME;
    }

    WidgetManager::~WidgetManager()
    {
        for (auto widget : widgets)
            delete widget;
    }

    void WidgetManager::handleEvent(const SDL_Event &e)
    {
        for (auto widget : widgets)
        {
            widget->handleGenericEvents(e);
            widget->handleEvent(e);
        }
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
        {
            widget->drawCommonElements();
            widget->draw();
        }
    }

}

namespace PROJECT_NAMESPACE
{
    template <>
    const int Object<Widget>::err = std::atexit(Object<Widget>::atexit_handler);

    Widget::Widget(Window &window) : window(window), geometry(self), font(window.config.defaultFontPath, window.config.defaultFontSize)
    {
        if (&window.container != this)
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
            events["pressed"].triggered = m_isPressed = m_isHovered;
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (m_isPressed && m_isHovered)
            {
                events["clicked"].triggered = true;
            }
            break;
        }
        }
    }

    void Widget::drawCommonElements()
    {
        auto scheme = self.getCurrentColorScheme();
        window.renderer.drawFillRectangle(geometry.dest, scheme.background);
        window.renderer.drawRectangle(geometry.dest, scheme.border);
#ifdef DEBUG
        window.renderer.drawRectangle(geometry.dest, Colors::Red);
        window.renderer.drawCross(geometry.dest, {Colors::Red, 122});
#endif
    }

    void Widget::handleEvent(const SDL_Event &)
    {
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
    }
}