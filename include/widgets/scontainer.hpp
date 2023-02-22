#ifndef D05F5FD6_01E7_4DE5_B36F_00509F8E6E18
#define D05F5FD6_01E7_4DE5_B36F_00509F8E6E18

#include "score.hpp"
#include "swidget.hpp"

namespace PROJECT_NAMESPACE
{
    class Container : public WidgetManager, public Object<Container>
    {
    private:
        void push(Widget &widget)
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

        template <typename Arg>
        void add(Arg &&widget)
        {
            self.push(widget);
        }
    };
}

#endif /* D05F5FD6_01E7_4DE5_B36F_00509F8E6E18 */
