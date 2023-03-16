#ifndef D05F5FD6_01E7_4DE5_B36F_00509F8E6E18
#define D05F5FD6_01E7_4DE5_B36F_00509F8E6E18

#include "score.hpp"
#include "swidget.hpp"

namespace PROJECT_NAMESPACE
{
    template <typename T>
    class Container : public WidgetManager, public Object<Container<T>>
    {
    private:
        void push(T &widget)
        {
            auto &pwidgets = widget.parent->widgets;
            pwidgets.erase(std::remove_if(
                pwidgets.begin(), pwidgets.end(),
                [this, &widget](const T *ptr)
                {
                    if (&widget == ptr)
                    {
                        Debug("Moving object " << widget.getName());
                        widget.parent = this;
                        self.widgets.emplace_back(&widget);
                        return true;
                    }
                    return false;
                }));
        }

    public:
        Container(Window &window) : WidgetManager(window)
        {
        }
        virtual ~Container()
        {
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

        template <typename Arg>
        void add(Arg &&widget)
        {
            self.push(widget);
        }
    };
}

#endif /* D05F5FD6_01E7_4DE5_B36F_00509F8E6E18 */
