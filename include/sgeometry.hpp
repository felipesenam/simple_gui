#ifndef __SGEOMETRY_H__
#define __SGEOMETRY_H__

#include "score.hpp"

namespace PROJECT_NAMESPACE
{
    class Rect : public SDL_Rect
    {
    public:
        Rect() : SDL_Rect({0, 0, 0, 0}) {}
    };

    class Geometry
    {
    private:
    public:
        Rect src, dest, abs;

        Geometry()
        {
        }
    };
}

#endif // __SGEOMETRY_H__