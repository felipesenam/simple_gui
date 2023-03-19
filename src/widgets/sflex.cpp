#include "widgets/sflex.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Flex::Flex(Window &window) : WidgetManager(window)
    {
    }

    void Flex::handleEvent(const SDL_Event &e)
    {
        handleGenericEvents(e);

        for (auto &widget : widgets)
        {
            widget->handleEvent(e);
        }
    }

    void Flex::posWidgetHorizontal(int &lx, int &ly, int &currentWidth, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        switch (style.horizontalAlign)
        {
        case left:
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            break;
        case center:
            lx = currentWidth + geometry.dest.x + (geometry.dest.w / 2) - (dimensions.width / 2);
            widget.geometry.dest.x = lx + widget.geometry.padding.left - widget.geometry.padding.right;
            break;
        case right:
            lx = currentWidth + geometry.dest.x + geometry.dest.w - dimensions.width;
            widget.geometry.dest.x = lx + widget.geometry.margin.right + widget.geometry.padding.right;
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case middle:
            ly = geometry.dest.y + (geometry.dest.h / 2) - (widget.geometry.dest.h / 2);
            widget.geometry.dest.y = ly + widget.geometry.padding.top - widget.geometry.padding.bottom;
            break;
        case bottom:
            ly = geometry.dest.y + geometry.dest.h - widget.geometry.dest.h;
            widget.geometry.dest.y = ly - (widget.geometry.margin.bottom + widget.geometry.padding.bottom);
            break;
        }
        switch (style.justifyContent)
        {
        case between:
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            currentWidth += spaceBetween;
            break;
        case around:
            currentWidth += spaceAround;
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            currentWidth += spaceAround;
            break;
        default:
            break;
        }
    }
    void Flex::posWidgetVertical(int &lx, int &ly, int &currentHeight, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        switch (style.horizontalAlign)
        {
        case left:
            lx = geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            break;
        case center:
            lx = geometry.dest.x + (geometry.dest.w / 2) - (widget.geometry.dest.w / 2);
            widget.geometry.dest.x = lx + widget.geometry.padding.left - widget.geometry.padding.right;
            break;
        case right:
            lx = geometry.dest.x + geometry.dest.w - widget.geometry.dest.w;
            widget.geometry.dest.x = lx - (widget.geometry.margin.right + widget.geometry.padding.right);
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case middle:
            ly = currentHeight + geometry.dest.y + (geometry.dest.h / 2) - (dimensions.height / 2);
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case bottom:
            ly = currentHeight + geometry.dest.y + geometry.dest.h - dimensions.height;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        }
        switch (style.justifyContent)
        {
        case between:
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            currentHeight += spaceBetween;
            break;
        case around:
            currentHeight += spaceAround;
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            currentHeight += spaceAround;
            break;
        default:
            break;
        }
    }

    void Flex::getDimensions()
    {
        if (self.style.direction == horizontal)
        {
            dimensions = query_content_horizontal();
        }
        else
        {
            dimensions = query_content_vertical();
        }
    }

    void Flex::update()
    {
        events.perform();

        int lx, ly;
        getDimensions();
        if (style.direction == vertical)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.h - dimensions.height) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.h - dimensions.height) / (widgets.size() - 1) : 0;

            int currentHeight = 0;
            for (auto &widget : widgets)
            {
                posWidgetVertical(lx, ly, currentHeight, *widget, spaceBetween, spaceAround);

                widget->update();

                const int my = widget->geometry.margin.y();
                const int py = widget->geometry.padding.y();
                currentHeight += widget->geometry.dest.h + my + py;
            }
        }
        else if (style.direction == horizontal)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.w - dimensions.width) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.w - dimensions.width) / (widgets.size() - 1) : 0;

            int currentWidth = 0;
            for (auto &widget : widgets)
            {
                posWidgetHorizontal(lx, ly, currentWidth, *widget, spaceBetween, spaceAround);

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

#ifdef DEBUG
        window.renderer.drawRectangle(geometry.dest, Colors::Red);
        window.renderer.drawCross(geometry.dest, {Colors::Red, 122});
#endif
    }
}

namespace PROJECT_NAMESPACE
{
    Column::Column(Window &window) : Flex(window)
    {
    }
    void Column::render()
    {
        for (auto widget : widgets)
            widget->render();

        getDimensions();
        self.geometry.dest.h = dimensions.height;
        self.geometry.dest.w = dimensions.width;
    }
}

namespace PROJECT_NAMESPACE
{
    Row::Row(Window &window) : Flex(window)
    {
        style.direction = horizontal;
    }

    void Row::handleEvent(const SDL_Event &e)
    {
        for (auto widget : widgets)
            widget->handleEvent(e);
    }
    void Row::render()
    {
        for (auto widget : widgets)
        {
            widget->render();

            Column *column = dynamic_cast<Column *>(widget);
            if (column && column->size >= 1)
            {
                column->geometry.dest.w = (column->size / float(self.size)) * self.geometry.dest.w;
            }
        }

        dimensions = query_content_horizontal();
        self.geometry.dest.w = self.parent->geometry.dest.w;
        self.geometry.dest.h = dimensions.height;
    }
    // void Row::update()
    // {
    //     int currentWidth = 0;
    //     int lx, ly;

    //     for (auto widget : widgets)
    //     {
    //         posWidgetHorizontal(lx, ly, currentWidth, *widget, 0, 0);
    //         // widget->geometry.dest.h = self.geometry.dest.h;
    //         widget->update();

    //         const int mx = widget->geometry.margin.x();
    //         const int px = widget->geometry.padding.x();
    //         currentWidth += widget->geometry.dest.w + mx + px;
    //     }
    // }
    void Row::draw()
    {
        for (auto widget : widgets)
            widget->draw();

#ifdef DEBUG
        window.renderer.drawRectangle(self.geometry.dest, Colors::Orange);
        window.renderer.drawCross(geometry.dest, {Colors::Orange, 122});
#endif
    }
}