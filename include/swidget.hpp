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
        friend class Geometry;

    protected:
        Container *parent = nullptr;
        Window &window;

        template <typename T, typename... Args>
        class Event
        {
        private:
            using Type = T(Args...);
            std::function<Type> function;

        public:
            T operator()(Args... args)
            {
                invoke(args...);
            }
            T invoke(Args... args)
            {
                if (function)
                {
                    function(args...);
                }
            }
            std::function<Type> operator=(std::function<Type> function)
            {
                return this->function = function;
            }
        };
        struct Events
        {
            Event<void> onHovering;
            Event<void> onClicked;
            Event<void> onHover;
            Event<void> onMouseDown;
            Event<void> onMouseUp;
            Event<void> onMouseLeave;
            Event<void> onFocus;
            Event<void> onLostFocus;

            Event<void> onCaretMoved;
            Event<void> onTextModified;

            Event<void> onKeydown;
            Event<void> onKeyup;
            Event<void> onMouseWheelMoved;

            Event<void, int, int> onWindowSizeChanged;
            Event<void, int, int> onWindowResized;
        };

    public:
        Widget(Window &window);

        Geometry geometry;
        Font font;

        Color color, background;

        Events events;

        virtual void handleWindowEvents(const SDL_Event &e);

        virtual void handleEvent(const SDL_Event &e);
        virtual void render();
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