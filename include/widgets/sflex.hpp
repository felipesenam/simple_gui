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
    enum VerticalAlign
    {
        top,
        middle,
        bottom
    };

    class Flex : public Container, public Object<Flex>
    {
    private:
    public:
        Flex(Window &window);

        Direction direction = vertical;
        VerticalAlign verticalAlign = top;
        HorizontalAlign horizontalAlign = left;

        void handleEvent(const SDL_Event &e) override;
        void update() override;
        void draw() override;
    };

}

#endif /* B3562FE8_D320_4D08_B652_824BBE86B6AD */
