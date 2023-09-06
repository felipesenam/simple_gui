#include "sgeometry.hpp"

namespace PROJECT_NAMESPACE
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