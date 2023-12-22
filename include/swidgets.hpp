#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "score.hpp"
#include "sfont.hpp"
#include "sgeometry.hpp"
#include "scolorgrid.hpp"

namespace sgui
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
            std::function<void(Widget &)> m_function;

        public:
            bool triggered = false;

            void operator()(Widget &widget)
            {
                if (triggered && m_function)
                {
                    m_function(widget);
                    triggered = false;
                }
            }

            void invoke(Widget &widget)
            {
                m_function(widget);
            }

            std::function<void(Widget &)> operator=(std::function<void(Widget &)> function)
            {
                return this->m_function = function;
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

                NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ColorScheme, color, background, border);
            };

            ColorScheme normal;
            ColorScheme hover;
            ColorScheme pressed;
            ColorScheme focused;
            ColorScheme disabled;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WidgetColorScheme, normal, hover, pressed, focused, disabled);
        };
        WidgetColorScheme scheme;

        class EventManager
        {
        private:
            Widget &widget;

        public:
            EventManager(Widget &Widget) : widget(Widget) {}

            std::map<std::string, Event> events;

            void perform()
            {
                for (auto &tuple : events)
                {
                    auto &event = tuple.second;
                    event(widget);
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

            friend std::ostream &operator<<(std::ostream &os, const Dimensions &dimensions)
            {
                return os << "Dimensions{width=" << dimensions.width << ", height=" << dimensions.height << "}";
            }
        };

        Dimensions query_content() const noexcept
        {
            Dimensions dimensions;
            for (auto widget : widgets)
            {
                dimensions.width += widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x();
                dimensions.height += widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y();
            }
            return dimensions;
        }
        Dimensions query_content_vertical() const noexcept
        {
            Dimensions dimensions;
            for (auto widget : widgets)
            {
                dimensions.width = std::max(dimensions.width, widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x());
                dimensions.height += widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y();
            }
            return dimensions;
        }
        Dimensions query_content_horizontal() const noexcept
        {
            Dimensions dimensions;
            for (auto widget : widgets)
            {
                dimensions.width += widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x();
                dimensions.height = std::max(dimensions.height, widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y());
            }
            return dimensions;
        }

        template <typename T>
        T &get(const std::string &uid)
        {
            auto it = std::find_if(widgets.begin(), widgets.end(), [&uid](const auto &obj)
                                   { return obj->uid == uid; });

            if (it == widgets.end())
                throw std::runtime_error("Widget not found '" + uid + "'");

            return *dynamic_cast<T *>(*it);
        }
        template <typename T>
        T &operator[](const std::string &uid)
        {
            return this->get<T>(uid);
        }

        void add(Widget &widget)
        {
            this->add(widget.uid, widget);
        }
        void add(const std::string &uid, Widget &widget)
        {
            if (widget.parent)
            {
                auto &pwidgets = widget.parent->widgets;
                pwidgets.erase(std::find_if(
                    pwidgets.begin(), pwidgets.end(),
                    [this, &widget, &uid](const auto &obj)
                    {
                        if (&widget == obj)
                        {
                            widget.parent = this;
                            this->widgets.emplace_back(&widget);

                            Debug("Moved object " << widget);

                            return true;
                        }
                        return false;
                    }));
            }
            else
            {
                widget.parent = this;
                this->widgets.emplace_back(&widget);

                Debug("Orphan object moved " << widget);
            }
        }

        template <typename T, typename... Args>
        T &create(const std::string &uid, Args &&...args)
        {
            T *widget = new T(this->window, args...);
            this->add(uid, *widget);
            return *widget;
        }

        void handleEvent(const SDL_Event &e) override;
        void render() override;
        void update() override;
        void draw() override;
    };

    enum Direction
    {
        horizontal,
        vertical
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Direction,
                                 {
                                     {Direction::horizontal, "horizontal"},
                                     {Direction::vertical, "vertical"},
                                 })

    enum HorizontalAlign
    {
        left,
        center,
        right
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(HorizontalAlign,
                                 {
                                     {HorizontalAlign::left, "left"},
                                     {HorizontalAlign::center, "center"},
                                     {HorizontalAlign::right, "right"},
                                 })

    enum JustifyContent
    {
        none,
        between,
        around
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(JustifyContent,
                                 {
                                     {JustifyContent::none, "none"},
                                     {JustifyContent::between, "between"},
                                     {JustifyContent::around, "around"},
                                 })

    enum VerticalAlign
    {
        top,
        middle,
        bottom
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(VerticalAlign,
                                 {
                                     {VerticalAlign::top, "top"},
                                     {VerticalAlign::middle, "middle"},
                                     {VerticalAlign::bottom, "bottom"},
                                 })

    enum Layout
    {
        normal,
        fixed
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Layout,
                                 {
                                     {Layout::normal, "normal"},
                                     {Layout::fixed, "fixed"},
                                 })

    class Row;
    class Column;
    class Flex : public WidgetManager
    {
    protected:
        void posWidgetHorizontal(int &lx, int &ly, int &currentWidth, Widget &widget, const int spaceBetween, const int spaceAround);
        void posWidgetVertical(int &lx, int &ly, int &currentHeight, Widget &widget, const int spaceBetween, const int spaceAround);
        Dimensions dimensions;
        void getDimensions();

    public:
        Flex(Window &window);
        struct Style
        {
            Direction direction = vertical;
            VerticalAlign verticalAlign = top;
            HorizontalAlign horizontalAlign = left;
            JustifyContent justifyContent = none;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Style, direction, verticalAlign, horizontalAlign, justifyContent);
        };
        Style style;

        void update() override;

        friend void to_json(json &j, const Flex &p);
        friend void from_json(const json &j, Flex &p);
    };

    class Column : public Flex
    {
    public:
        Column(Window &window);

        unsigned size = 0;

        void render() override;

        friend void to_json(json &j, const Column &col);
        friend void from_json(const json &j, Column &col);
    };

    class Row : public Flex
    {
    public:
        Row(Window &window);

        unsigned size = 12;

        void render() override;

        friend void to_json(json &j, const Row &row);
        friend void from_json(const json &j, Row &row);
    };

    class Label : public Widget
    {
    private:
        SDL_Texture *textTexture = nullptr;

        Color renderedColor;
        std::string renderedText;
        // TTF* renderedFont = nullptr;

    public:
        Label(Window &window);
        ~Label();

        std::string text;
        SDL_Texture *texture = nullptr;

        void render() override;
        void update() override;
        void draw() override;

        friend void to_json(json &j, const Label &p)
        {
            j["type"] = demangle(typeid(Label).name());

            j["uid"] = p.uid;
            j["text"] = p.text;
            j["geometry"] = p.geometry;
            j["scheme"] = p.scheme;
        }
        friend void from_json(const json &j, Label &p)
        {
            SETATTR_IF_JSON_CONTAINS(j, p, uid);
            SETATTR_IF_JSON_CONTAINS(j, p, text);
            SETATTR_IF_JSON_CONTAINS(j, p, geometry);
            SETATTR_IF_JSON_CONTAINS(j, p, scheme);
        }
    };

    class Bitmap : public ColorGrid, public Widget
    {
    private:
        SDL_Texture *tex = nullptr;

        bool m_render = false;

    public:
        Bitmap(Window &window);
        Bitmap(Window &window, size_t width, size_t height);
        ~Bitmap();

        Color &at(size_t x, size_t y) override;

        void alloc(size_t width, size_t height) override;

        void erase() override;

        void render();
        void draw();

        friend void to_json(json &j, const Bitmap &col);
        friend void from_json(const json &j, Bitmap &col);
    };

    template <typename T>
    static bool emplace(json &j, const Widget *widget)
    {
        const T *ptr = dynamic_cast<const T *>(widget);
        if (ptr)
        {
            Debug("Saving " << *widget << " as " << demangle(typeid(T).name()));
            j.emplace_back(*ptr);
        }

        return ptr;
    }

    template <typename T>
    bool get_from(const json &j, const std::string &uid, WidgetManager *manager)
    {
        std::string type = j["type"];
        if (type == demangle(typeid(T).name()))
        {
            T &widget = manager->create<T>(uid);
            from_json(j, widget);
            Debug("New " << type << " from json");
            return true;
        }
        return false;
    }

    void to_json(json &j, const std::vector<Widget *> &p);
    void from_json(const json &j, WidgetManager *man);
}

#endif // __WIDGET_H__