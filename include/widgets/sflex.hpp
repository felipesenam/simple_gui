#ifndef B3562FE8_D320_4D08_B652_824BBE86B6AD
#define B3562FE8_D320_4D08_B652_824BBE86B6AD

#include <type_traits>

#include "slabel.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    enum Direction
    {
        horizontal,
        vertical
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Direction, {
                                                {Direction::horizontal, "horizontal"},
                                                {Direction::vertical, "vertical"},
                                            })

    enum HorizontalAlign
    {
        left,
        center,
        right
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(HorizontalAlign, {
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
    NLOHMANN_JSON_SERIALIZE_ENUM(JustifyContent, {
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
    NLOHMANN_JSON_SERIALIZE_ENUM(VerticalAlign, {
                                                    {VerticalAlign::top, "top"},
                                                    {VerticalAlign::middle, "middle"},
                                                    {VerticalAlign::bottom, "bottom"},
                                                })

    enum Layout
    {
        normal,
        fixed
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Layout, {
                                             {Layout::normal, "normal"},
                                             {Layout::fixed, "fixed"},
                                         })

    class Flex : public WidgetManager, public Object<Flex>
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

        template <typename T>
        static bool emplace(const std::pair<std::string, Widget *> &widget, json &j)
        {
            auto ptr = dynamic_cast<T *>(widget.second);
            if (ptr)
                j[widget.first] = *ptr;

            return ptr;
        }
        static void try_emplace(const std::pair<std::string, Widget *> &widget, json &j)
        {
            if (emplace<Label>(widget, j))
                return;
        }

        template <typename T>
        void get_from(const std::string &uid, const json &j)
        {
            T &widget = window.create<T>();
            this->add(uid, widget);
            from_json(j, widget);
        }
        void try_get_from(const std::string &uid, const json &j)
        {
            std::string type = j["type"];
            if (type == "sgui::Label")
            {
                get_from<Label>(uid, j);
                return;
            }
        }

        friend void to_json(json &j, const Flex &p)
        {
            j["type"] = type(p);
            j["style"] = p.style;
            for (auto &widget : p.widgets)
            {
                try_emplace(widget, j["widgets"]);
            }
        }
        friend void from_json(const json &j, Flex &p)
        {
            SETATTR_IF_JSON_CONTAINS(j, p, style);
            if (j.contains("widgets"))
            {
                for (auto widget : j["widgets"].items())
                {
                    Debug(widget.value());
                    p.try_get_from(widget.key(), widget.value());
                }
            }
        }
    };

    class Column : public Flex, public Object<Column>
    {
    public:
        Column(Window &window);

        unsigned size = 0;

        void render() override;
    };

    class Row : public Flex, public Object<Row>
    {
    public:
        Row(Window &window);

        unsigned size = 12;

        void render() override;
    };
}

#endif /* B3562FE8_D320_4D08_B652_824BBE86B6AD */
