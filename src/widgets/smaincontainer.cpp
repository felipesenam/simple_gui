
#include "widgets/smaincontainer.hpp"
#include "swindow.hpp"

PROJECT_NAMESPACE::MainContainer::MainContainer(Window &window) : Container(window)
{
    auto size = window.size();
    geometry.abs.w = size.first;
    geometry.abs.h = size.second;
}
