#include "sgeometry.hpp"

namespace sgui
{
    void Geometry::normalize()
    {
        switch (behavior)
        {
        case hug:
            src = dest = abs;
            break;

        case normal:
            src = dest = abs;
            break;

        default:
            break;
        }
    }
}