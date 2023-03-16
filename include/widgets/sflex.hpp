#ifndef B3562FE8_D320_4D08_B652_824BBE86B6AD
#define B3562FE8_D320_4D08_B652_824BBE86B6AD

#include "../score.hpp"
#include "../swidget.hpp"
#include "scolors.hpp"
#include "scontainer.hpp"

namespace PROJECT_NAMESPACE
{
    enum Direction
    {
        horizontal,
        vertical
    };

    enum HorizontalAlign
    {
        left,
        center,
        right
    };

    enum JustifyContent
    {
        none,
        between,
        around
    };

    enum VerticalAlign
    {
        top,
        middle,
        bottom
    };

    enum Layout
    {
        normal,
        fixed
    };

    class Flex : public Container<Widget>, public Object<Flex>
    {
    protected:
        void posWidgetHorizontal(int &lx, int &ly, int currentWidth, Widget &widget, const int spaceBetween, const int spaceAround);
        void posWidgetVertical(int &lx, int &ly, int currentHeight, Widget &widget, const int spaceBetween, const int spaceAround);
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
        };

        Style style;

        void handleEvent(const SDL_Event &e) override;
        void update() override;
        void draw() override;
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

        void handleEvent(const SDL_Event &e) override;
        void render() override;
        void update() override;
        void draw() override;
    };

}

#endif /* B3562FE8_D320_4D08_B652_824BBE86B6AD */
