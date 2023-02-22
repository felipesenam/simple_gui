#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "score.hpp"
#include "sfont.hpp"
#include "sgeometry.hpp"

namespace PROJECT_NAMESPACE
{
    class Window;
    class WidgetManager;
    class Container;

    class Widget : public Object<Widget>
    {
    private:
        friend class WidgetManager;
        friend class Container;

    protected:
        Container *parent = nullptr;
        Window &window;

    public:
        Widget(Window &window);

        Geometry geometry;
        Font font;

        Color color, background;

        virtual void handleEvent(const SDL_Event &e);
        virtual void update();
        virtual void draw();
    };

    class WidgetManager : public Widget
    {
    private:
        friend class Container;

    protected:
        std::vector<Widget *> widgets;

    public:
        WidgetManager(Window &window);
        virtual ~WidgetManager();

        template <typename T, typename... Args>
        T &create(Args &&...args)
        {
            T *widget = new T(window, args...);
            widgets.emplace_back(widget);
            return *widget;
        }
        size_t count() const noexcept
        {
            return widgets.size();
        }

        void handleEvent(const SDL_Event &e) override;
        void update() override;
        void draw() override;
    };
}

#endif // __WIDGET_H__