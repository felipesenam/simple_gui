#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "score.hpp"
#include "sgeometry.hpp"

namespace PROJECT_NAMESPACE
{
    class Window;
    class Widget;

    class WidgetManager
    {
    private:
        Window &window;
        std::vector<Widget *> widgets;
        friend class Widget;

    public:
        WidgetManager(Window &window);
        ~WidgetManager();

        template <typename T, typename... Args>
        T &create(Args &&...args)
        {
            T *widget = new T(window, args...);
            widgets.emplace_back(widget);
            return *widget;
        }
        void handleEvent(const SDL_Event &e);
        void update();
        void draw();
    };

    class Widget : public Object<Widget>
    {
    protected:
        Window &window;

    public:
        Widget(Window &window);

        Geometry geometry;

        virtual void handleEvent(const SDL_Event &e);
        virtual void update();
        virtual void draw();
    };

}

#endif // __WIDGET_H__