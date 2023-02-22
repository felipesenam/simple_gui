#ifndef EE7A3997_4E4B_4A1E_8540_7F921309F34C
#define EE7A3997_4E4B_4A1E_8540_7F921309F34C

#include "scontainer.hpp"

namespace PROJECT_NAMESPACE
{
    class MainContainer : public Container, public Object<MainContainer>
    {
    private:
    public:
        MainContainer(Window &window);
    };
}

#endif /* EE7A3997_4E4B_4A1E_8540_7F921309F34C */
