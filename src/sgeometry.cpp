#include "sgeometry.hpp"
#include "swidget.hpp"
#include "widgets/scontainer.hpp"

namespace PROJECT_NAMESPACE
{
    void Geometry::normalize()
    {
        switch (behavior)
        {
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

        default:
            break;
        }
    }
}