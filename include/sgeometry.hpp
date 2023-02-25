#ifndef __SGEOMETRY_H__
#define __SGEOMETRY_H__

#include "score.hpp"
namespace PROJECT_NAMESPACE
{
    class Rect : public SDL_Rect
    {
    public:
        Rect() : SDL_Rect({0, 0, 0, 0}) {}

        bool operator==(const Rect &rect)
        {
            return self.x == rect.x && self.y == rect.y && self.w == rect.w && self.h == rect.h;
        }
    };

    class Box
    {
    public:
        int top, left, bottom, right;
        Box() {}
        Box(int top, int left, int bottom, int right) : top(top), left(left), bottom(bottom), right(right) {}

        bool operator==(const Box &box)
        {
            return self.top == box.top && self.left == box.left && self.bottom == box.bottom && self.right == box.right;
        }

        Box &operator=(int value)
        {
            self.top = self.bottom = self.left = self.right = value;
            return self;
        }

        int x() const
        {
            return self.left + self.right;
        }
        void x(int x)
        {
            self.left = x;
            self.right = x;
        }
        int y() const
        {
            return self.top + self.bottom;
        }
        void y(int y)
        {
            self.top = y;
            self.bottom = y;
        }
    };

    enum Behavior
    {
        fixed,
        hug,
        fill
    };

    enum Center
    {
        none,
        top_left,
        top_center,
        top_right,
        middle_left,
        middle_center,
        middle_right,
        bottom_left,
        bottom_center,
        bottom_right
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

        Rect src, dest, abs;
        Behavior behavior = hug;
        Center anchor = middle_center;

        Box margin;
        Box padding;

        void normalize();

        void confine(const SDL_Rect &box)
        {
            confine(src.x, src.w, dest.x, dest.w, box.x, box.w, abs.w);
            confine(src.y, src.h, dest.y, dest.h, box.y, box.h, abs.h);
        }

        void posCenter(Rect ref)
        {
            switch (anchor)
            {
            case none:
                break;
            case top_left:
                dest.x = ref.x;
                dest.y = ref.y;
                break;
            case top_center:
                dest.x = ref.x - (ref.w / 2);
                dest.y = ref.y;
                break;
            case top_right:
                dest.x = ref.x - ref.w;
                dest.y = ref.y;
                break;
            case middle_left:
                dest.x = ref.x;
                dest.y = ref.y - (ref.h / 2);
                break;
            case middle_center:
                dest.x = ref.x - (ref.w / 2);
                dest.y = ref.y - (ref.h / 2);
                break;
            case middle_right:
                dest.x = ref.x - ref.w;
                dest.y = ref.y - (ref.h / 2);
                break;
            case bottom_left:
                dest.x = ref.x;
                dest.y = ref.y - ref.h;
                break;
            case bottom_center:
                dest.x = ref.x - (ref.w / 2);
                dest.y = ref.y - ref.h;
                break;
            case bottom_right:
                dest.x = ref.x - ref.w;
                dest.y = ref.y - ref.h;
                break;
            }
        }
    };
}

#endif // __SGEOMETRY_H__