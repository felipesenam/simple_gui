#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "score.hpp"
#include "sfont.hpp"
#include "sgeometry.hpp"

namespace PROJECT_NAMESPACE
{
    class Window;
    class WidgetManager;

    class Widget : public Object<Widget>
    {
    private:
        friend class WidgetManager;
        friend class Geometry;

        bool m_isHovered = false;
        bool m_isPressed = false;
        bool m_isFocused = false;
        bool m_isEnabled = true;

    protected:
        WidgetManager *parent = nullptr;
        Window &window;

        class Event
        {
        private:
            std::function<void()> m_function;

        public:
            bool triggered = false;

            void operator()()
            {
                if (triggered && m_function)
                {
                    m_function();
                    triggered = false;
                }
            }

            void invoke()
            {
                m_function();
            }

            std::function<void()> operator=(std::function<void()> function)
            {
                return self.m_function = function;
            }
        };

    public:
        Widget(Window &window);

        Geometry geometry;
        Font font;

        struct WidgetColorScheme
        {
            struct ColorScheme
            {
                Color color;
                Color background;
                Color border;
            };

            ColorScheme normal;
            ColorScheme hover;
            ColorScheme pressed;
            ColorScheme focused;
            ColorScheme disabled;
        };
        WidgetColorScheme scheme;

        class EventManager
        {
        public:
            std::map<std::string, Event> events;

            void perform()
            {
                for (auto &tuple : events)
                {
                    auto &event = tuple.second;
                    event();
                }
            }

            Event &operator[](const std::string &index)
            {
                return events[index];
            }
        };
        EventManager events;

        virtual void handleGenericEvents(const SDL_Event &e);
        virtual void drawCommonElements();

        bool isHovered() const noexcept { return m_isHovered; }
        bool isPressed() const noexcept { return m_isPressed; }
        bool isFocused() const noexcept { return m_isFocused; }
        bool isEnabled() const noexcept { return m_isEnabled; }

        const WidgetColorScheme::ColorScheme &getCurrentColorScheme() const noexcept
        {
            if (!m_isEnabled)
                return scheme.disabled;
            if (m_isHovered)
                return scheme.hover;
            if (m_isPressed)
                return scheme.pressed;
            if (m_isFocused)
                return scheme.focused;

            return scheme.normal;
        }

        virtual void handleEvent(const SDL_Event &e);
        virtual void render();
        virtual void update();
        virtual void draw();
    };

    class WidgetManager : public Widget
    {
    private:
        void push()
        {
            Debug("End of interaction");
        }

        template <typename... Args>
        void push(Widget &widget, Args &&...args)
        {
            if (widget.parent)
            {
                auto &pwidgets = widget.parent->widgets;
                pwidgets.erase(std::remove_if(
                    pwidgets.begin(), pwidgets.end(),
                    [this, &widget](const Widget *ptr)
                    {
                        if (&widget == ptr)
                        {
                            Debug("Moving object " << widget.getName());
                            widget.parent = this;
                            self.widgets.push_back(&widget);
                            return true;
                        }
                        return false;
                    }));
            }
            else
            {
                widget.parent = this;
                self.widgets.push_back(&widget);
            }

            return push(args...);
        }

    protected:
        std::vector<Widget *> widgets;

    public:
        WidgetManager(Window &window);
        virtual ~WidgetManager();

        size_t count() const noexcept
        {
            return widgets.size();
        }

        struct Dimensions
        {
            int width = 0;
            int height = 0;
        };

        Dimensions query_content_vertical() const noexcept
        {
            Dimensions dimensions;
            for (auto &widget : widgets)
            {
                dimensions.width = std::max(dimensions.width, widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x());
                dimensions.height += widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y();
            }
            return dimensions;
        }
        Dimensions query_content_horizontal() const noexcept
        {
            Dimensions dimensions;
            for (auto &widget : widgets)
            {
                dimensions.width += widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x();
                dimensions.height = std::max(dimensions.height, widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y());
            }
            return dimensions;
        }

        template <typename... Args>
        void add(Args &&...widgets)
        {
            self.push(widgets...);
        }

        Widget &operator[](size_t index)
        {
            return *widgets[index];
        }

        void handleEvent(const SDL_Event &e) override;
        void render() override;
        void update() override;
        void draw() override;
    };
}

#endif // __WIDGET_H__