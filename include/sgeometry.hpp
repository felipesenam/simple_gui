#ifndef __SGEOMETRY_H__
#define __SGEOMETRY_H__

#include "score.hpp"
namespace PROJECT_NAMESPACE
{
    class Rect : public SDL_Rect
    {
    public:
        Rect() : SDL_Rect({0, 0, 0, 0}) {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Rect, x, y, w, h);

        bool operator==(const Rect &rect)
        {
            return this->x == rect.x && this->y == rect.y && this->w == rect.w && this->h == rect.h;
        }

        friend std::ostream &operator<<(std::ostream &o, const Rect &rect)
        {
            o << "{" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << "}";
            return o;
        }
    };

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
    };

    enum Center
    {
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
    NLOHMANN_JSON_SERIALIZE_ENUM(Center, {
                                             {Center::top_left, "top_left"},
                                             {Center::top_center, "top_center"},
                                             {Center::top_right, "top_right"},
                                             {Center::middle_left, "middle_left"},
                                             {Center::middle_center, "middle_center"},
                                             {Center::middle_right, "middle_right"},
                                             {Center::bottom_left, "bottom_left"},
                                             {Center::bottom_center, "bottom_center"},
                                             {Center::bottom_right, "bottom_right"},
                                         })

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

        enum Behavior
        {
            normal,
            hug,
        };
        NLOHMANN_JSON_SERIALIZE_ENUM(Behavior, {
                                                   {Behavior::normal, "normal"},
                                                   {Behavior::hug, "hug"},
                                               })
        Behavior behavior = normal;
        Center anchor = middle_center;

        Box margin;
        Box padding;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Geometry, src, dest, abs, behavior, anchor, margin, padding);

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