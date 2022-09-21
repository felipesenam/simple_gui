#include "sobject.hpp"

namespace PROJECT_NAMESPACE
{
    size_t Object::objectCount = 0;
    std::vector<Object *> Object::objects;

    Object::Object() : id(objectCount++)
    {
        objects.emplace_back(this);
    }
}