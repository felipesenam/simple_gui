#ifndef B3562FE8_D320_4D08_B652_824BBE86B6AD
#define B3562FE8_D320_4D08_B652_824BBE86B6AD

#include "../score.hpp"
#include "../swidget.hpp"
#include "scontainer.hpp"

namespace PROJECT_NAMESPACE
{
    enum Direction
    {
        horizontal,
        vertical
    };

    class Flex : public Container, public Object<Flex>
    {
    private:
    public:
        Flex(Window &window) : Container(window)
        {
            // geometry.abs.w =
        }

        Direction direction = vertical;

        void handleEvent(const SDL_Event &e) override
        {
            for (auto &widget : widgets)
            {
                widget->handleEvent(e);
            }
        }
        void update() override
        {
            Rect last;

            int mx = geometry.dest.w / 2, lx = geometry.dest.x;
            int ly = geometry.dest.y;
            for (auto &widget : widgets)
            {
                widget->update();
                widget->geometry.dest.x = lx + mx - (widget->geometry.abs.w / 2);
                widget->geometry.dest.y = ly;

                ly += widget->geometry.dest.h;
            }
        }
        void draw() override
        {
            for (auto &widget : widgets)
            {
                widget->draw();
            }
        }
    };

}

#endif /* B3562FE8_D320_4D08_B652_824BBE86B6AD */
