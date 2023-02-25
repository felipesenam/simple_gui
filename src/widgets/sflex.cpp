#include "widgets/sflex.hpp"
#include "swindow.hpp"
namespace PROJECT_NAMESPACE
{

    Flex::Flex(Window &window) : Container(window)
    {
        // geometry.abs.w =
    }

    void Flex::handleEvent(const SDL_Event &e)
    {
        handleWindowEvents(e);

        for (auto &widget : widgets)
        {
            widget->handleEvent(e);
        }
    }

    void Flex::update()
    {
        int totalWidth = 0;
        int totalHeight = 0;
        for (auto &widget : widgets)
        {
            widget->render();
            totalWidth += widget->geometry.dest.w + widget->geometry.margin.x() + widget->geometry.padding.x();
            totalHeight += widget->geometry.dest.h + widget->geometry.margin.y() + widget->geometry.padding.y();
        }

        int lx, ly;
        if (direction == vertical)
        {
            int currentHeight = 0;
            for (auto &widget : widgets)
            {
                switch (horizontalAlign)
                {
                case left:
                    lx = geometry.dest.x;
                    widget->geometry.dest.x = lx + widget->geometry.margin.left + widget->geometry.padding.left;
                    break;
                case center:
                    lx = geometry.dest.x + (geometry.dest.w / 2) - (widget->geometry.dest.w / 2);
                    widget->geometry.dest.x = lx + widget->geometry.padding.left - widget->geometry.padding.right;
                    break;
                case right:
                    lx = geometry.dest.x + geometry.dest.w - widget->geometry.dest.w;
                    widget->geometry.dest.x = lx - (widget->geometry.margin.right + widget->geometry.padding.right);
                    break;
                }
                switch (verticalAlign)
                {
                case top:
                    ly = currentHeight + geometry.dest.y;
                    widget->geometry.dest.y = ly + widget->geometry.margin.top + widget->geometry.padding.top;
                    break;
                case middle:
                    ly = currentHeight + geometry.dest.y + (geometry.dest.h / 2) - (totalHeight / 2);
                    widget->geometry.dest.y = ly + widget->geometry.margin.top + widget->geometry.padding.top;
                    break;
                case bottom:
                    ly = currentHeight + geometry.dest.y + geometry.dest.h - totalHeight;
                    widget->geometry.dest.y = ly + widget->geometry.margin.top + widget->geometry.padding.top;
                    break;
                }

                widget->update();

                const int my = widget->geometry.margin.y();
                const int py = widget->geometry.padding.y();
                currentHeight += widget->geometry.dest.h + my + py;
            }
        }
        else if (direction == horizontal)
        {
            int currentWidth = 0;
            for (auto &widget : widgets)
            {
                switch (horizontalAlign)
                {
                case left:
                    lx = currentWidth + geometry.dest.x;
                    widget->geometry.dest.x = lx + widget->geometry.margin.left + widget->geometry.padding.left;
                    break;
                case center:
                    lx = currentWidth + geometry.dest.x + (geometry.dest.w / 2) - (totalWidth / 2);
                    widget->geometry.dest.x = lx + widget->geometry.padding.left - widget->geometry.padding.right;
                    break;
                case right:
                    lx = currentWidth + geometry.dest.x + geometry.dest.w - totalWidth;
                    widget->geometry.dest.x = lx + widget->geometry.margin.right + widget->geometry.padding.right;
                    break;
                }
                switch (verticalAlign)
                {
                case top:
                    ly = geometry.dest.y;
                    widget->geometry.dest.y = ly + widget->geometry.margin.top + widget->geometry.padding.top;
                    break;
                case middle:
                    ly = geometry.dest.y + (geometry.dest.h / 2) - (widget->geometry.dest.h / 2);
                    widget->geometry.dest.y = ly + widget->geometry.padding.top - widget->geometry.padding.bottom;
                    break;
                case bottom:
                    ly = geometry.dest.y + geometry.dest.h - widget->geometry.dest.h;
                    widget->geometry.dest.y = ly - (widget->geometry.margin.bottom + widget->geometry.padding.bottom);
                    break;
                }

                widget->update();

                const int mx = widget->geometry.margin.x();
                const int px = widget->geometry.padding.x();
                currentWidth += widget->geometry.dest.w + mx + px;
            }
        }
    }

    void Flex::draw()
    {
        for (auto &widget : widgets)
        {
            widget->draw();
        }
        window.renderer.drawRectangle(geometry.dest, Colors::Red);
        window.renderer.drawCross(geometry.dest, Colors::Red);
    }
}