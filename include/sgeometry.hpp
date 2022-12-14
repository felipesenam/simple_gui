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

    enum Behavior
    {
        fixed,
        hug,
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

    class Geometry
    {
    private:
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
        Geometry()
        {
        }

        Rect src, dest, abs;
        Behavior behavior = hug;
        Center anchor = middle_center;

        void normalize()
        {
            switch (behavior)
            {
            case fixed:
                break;

            case hug:
                if (src == dest)
                {
                    src = dest = abs;
                }
                else
                {
                    dest = abs;
                }
                break;
            }
        }

        void confine(const SDL_Rect &box)
        {
            confine(src.x, src.w, dest.x, dest.w, box.x, box.w, abs.w);
            confine(src.y, src.h, dest.y, dest.h, box.y, box.h, abs.h);
        }

        void posCenter()
        {
            switch (anchor)
            {
            case none:
                break;
            case top_left:
                dest.x = abs.x;
                dest.y = abs.y;
                break;
            case top_center:
                dest.x = abs.x - (abs.w / 2);
                dest.y = abs.y;
                break;
            case top_right:
                dest.x = abs.x - abs.w;
                dest.y = abs.y;
                break;
            case middle_left:
                dest.x = abs.x;
                dest.y = abs.y - (abs.h / 2);
                break;
            case middle_center:
                dest.x = abs.x - (abs.w / 2);
                dest.y = abs.y - (abs.h / 2);
                break;
            case middle_right:
                dest.x = abs.x - abs.w;
                dest.y = abs.y - (abs.h / 2);
                break;
            case bottom_left:
                dest.x = abs.x;
                dest.y = abs.y - abs.h;
                break;
            case bottom_center:
                dest.x = abs.x - (abs.w / 2);
                dest.y = abs.y - abs.h;
                break;
            case bottom_right:
                dest.x = abs.x - abs.w;
                dest.y = abs.y - abs.h;
                break;
            }
        }
    };
}

#endif // __SGEOMETRY_H__