#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "score.hpp"

namespace sgui
{
    enum Direction
    {
        horizontal,
        vertical
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Direction,
                                 {
                                     {horizontal, "horizontal"},
                                     {vertical, "vertical"},
                                 })

    enum Overflow
    {
        visible,
        hidden,
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Overflow, {
                                               {visible, "visible"},
                                               {hidden, "hidden"},
                                           })

    enum HorizontalAlign
    {
        left,
        center,
        right
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(HorizontalAlign,
                                 {
                                     {left, "left"},
                                     {center, "center"},
                                     {right, "right"},
                                 })

    enum JustifyContent
    {
        none,
        between,
        around
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(JustifyContent,
                                 {
                                     {none, "none"},
                                     {between, "between"},
                                     {around, "around"},
                                 })

    enum VerticalAlign
    {
        top,
        middle,
        bottom
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(VerticalAlign,
                                 {
                                     {top, "top"},
                                     {middle, "middle"},
                                     {bottom, "bottom"},
                                 })

    enum Behavior
    {
        normal,
        hug,
        fill,
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Behavior, {
                                               {normal, "normal"},
                                               {hug, "hug"},
                                           })

    class Box
    {
    public:
        Box() {}
        Box(int size) : top(size), left(size), bottom(size), right(size) {}
        Box(int top, int left, int bottom, int right) : top(top), left(left), bottom(bottom), right(right) {}

        int top = 0;
        int left = 0;
        int bottom = 0;
        int right = 0;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Box, top, left, bottom, right);

        bool operator==(const Box &box)
        {
            return this->top == box.top && this->left == box.left && this->bottom == box.bottom && this->right == box.right;
        }

        Box &operator=(int value)
        {
            top = bottom = left = right = value;
            return *this;
        }

        int x() const noexcept
        {
            return this->left + this->right;
        }
        void x(int x)
        {
            this->left = x;
            this->right = x;
        }
        int y() const noexcept
        {
            return this->top + this->bottom;
        }
        void y(int y)
        {
            this->top = y;
            this->bottom = y;
        }
        int dx() const noexcept
        {
            return this->left - this->right;
        }
        int dy() const noexcept
        {
            return this->top - this->bottom;
        }
    };

    class Widget;
    class Geometry
    {
    private:
        Widget &widget;

        void confine(int &srcp, int &srcd, int &destp, int &destd, int boxp, int boxd, int absd)
        {
            srcp = 0;
            destd = srcd = absd;
            if (destp >= boxp && destp + absd < boxp + boxd)
            {
                srcp = 0;
                destd = srcd = absd;
            }
            else
            {
                if (destp < boxp && destp + absd > boxp)
                {
                    destd = srcd = (destp + absd) - boxp;
                    srcp = boxp - destp;
                    destp = boxp;
                }
                if (destp < boxp + boxd && destp + absd > boxp + boxd)
                {
                    destd = srcd = (boxp + boxd) - destp;
                }
            }
        }

    public:
        Geometry(Widget &widget) : widget(widget)
        {
        }

        int x = 0, y = 0;
        int width = 0, height = 0;

        Rect clip, src, dest;

        Behavior behavior = hug;
        Overflow overflow = visible;

        float aspect = 1.0f;

        Box margin;
        Box padding;

        Rect marginRect() const noexcept;
        Rect paddingRect() const noexcept;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Geometry, src, dest, behavior, aspect, margin, padding);

        void normalize();

        void confine(Geometry &geometry);
    };
    class ApplicationWindow;
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
        ApplicationWindow &window;

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
        Widget(ApplicationWindow &window);

        Geometry geometry;
        Font font;

        bool show = true;

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

        virtual void handleGenericEvents(const SDL_Event &e);
        virtual void drawCommonElements();
        virtual void preUpdate();

        virtual void handleEvent(const SDL_Event &e);
        virtual void render();
        virtual void update();
        virtual void draw();

        virtual int contentHeight() const noexcept
        {
            return geometry.height + geometry.padding.y();
        }
        virtual int height() const noexcept
        {
            return contentHeight() + geometry.margin.y();
        }

        virtual int contentWidth() const noexcept
        {
            return geometry.width + geometry.padding.x();
        }
        virtual int width() const noexcept
        {
            return contentWidth() + geometry.margin.x();
        }
    };

    class WidgetManager : public Widget
    {
    private:
    protected:
        std::vector<Widget *> widgets;

    public:
        WidgetManager(ApplicationWindow &window);
        virtual ~WidgetManager();

        size_t count() const noexcept
        {
            return widgets.size();
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

    class Row;
    class Column;
    class Flex : public WidgetManager
    {
    protected:
        void posWidgetHorizontal(int &lx, int &ly, int &currentWidth, Widget &widget, const int spaceBetween, const int spaceAround);
        void posWidgetVertical(int &lx, int &ly, int &currentHeight, Widget &widget, const int spaceBetween, const int spaceAround);

    public:
        Flex(ApplicationWindow &window);
        struct Style
        {
            Direction direction = vertical;
            VerticalAlign verticalAlign = middle;
            HorizontalAlign horizontalAlign = center;
            JustifyContent justifyContent = none;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Style, direction, verticalAlign, horizontalAlign, justifyContent);
        };
        Style style;

        int gap = 0;

        void update() override;

        friend void to_json(json &j, const Flex &p);
        friend void from_json(const json &j, Flex &p);

        int widgetHeight() const noexcept
        {
            int height = 0;
            switch (this->style.direction)
            {
            case horizontal:
                for (auto &widget : widgets)
                    height = std::max(height, widget->height());
                break;
            case vertical:
                height += (widgets.size() - 1) * gap;
                for (auto &widget : widgets)
                    height += widget->height();
                break;
            }
            return height;
        }

        int contentHeight() const noexcept override
        {
            switch (geometry.behavior)
            {
            case hug:
                return widgetHeight() + geometry.padding.y();

            default:
                return geometry.height + geometry.padding.y();
            }
        }

        int widgetWidth() const noexcept
        {
            int width = 0;
            switch (this->style.direction)
            {
            case horizontal:
                width += (widgets.size() - 1) * gap;
                for (auto &widget : widgets)
                    width += widget->width();
                break;
            case vertical:
                for (auto &widget : widgets)
                    width = std::max(width, widget->width());
                break;
            }

            return width;
        }

        int contentWidth() const noexcept override
        {
            switch (geometry.behavior)
            {
            case hug:
                return widgetWidth() + geometry.padding.x();

            default:
                return geometry.width + geometry.padding.x();
            }
        }
    };

    class Column : public Flex
    {
    public:
        Column(ApplicationWindow &window);

        unsigned size = 0;

        void render() override;

        friend void to_json(json &j, const Column &col);
        friend void from_json(const json &j, Column &col);
    };

    class Row : public Flex
    {
    public:
        Row(ApplicationWindow &window);

        unsigned size = 12;

        void render() override;

        friend void to_json(json &j, const Row &row);
        friend void from_json(const json &j, Row &row);
    };

    class Label : public Widget
    {
    private:
        Texture *textTexture = nullptr;

        std::string renderedText;

    public:
        Label(ApplicationWindow &window, const std::string &text);
        Label(ApplicationWindow &window);
        ~Label();

        std::string text;
        Texture *texture = nullptr;

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

    class TextBox : public Widget
    {
    private:
        Texture *textTexture = nullptr;

        std::string renderedText;

    public:
        TextBox(ApplicationWindow &window) : Widget(window)
        {
        }

        std::string text;

        void render() override;
        void update() override;
        void draw() override;

        friend void to_json(json &j, const TextBox &p)
        {
            j["type"] = demangle(typeid(TextBox).name());

            j["uid"] = p.uid;
            j["geometry"] = p.geometry;
            j["scheme"] = p.scheme;
        }
        friend void from_json(const json &j, TextBox &p)
        {
            SETATTR_IF_JSON_CONTAINS(j, p, uid);
            SETATTR_IF_JSON_CONTAINS(j, p, geometry);
            SETATTR_IF_JSON_CONTAINS(j, p, scheme);
        }
    };

    class Bitmap : public Widget
    {
    private:
        Texture *m_texture = nullptr;

        bool m_render = false;

    public:
        Bitmap(ApplicationWindow &window, size_t width, size_t height);
        Bitmap(ApplicationWindow &window);
        ~Bitmap();

        Surface *surface = nullptr;

        void alloc(size_t width, size_t height);
        void open(const std::string &file);

        void erase();

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