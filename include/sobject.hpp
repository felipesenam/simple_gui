#ifndef __SOBJECT_H__
#define __SOBJECT_H__

#include "score.hpp"
#include "type.hpp"

namespace PROJECT_NAMESPACE
{
    class Object
    {
    protected:
        static size_t objectCount;
        static std::vector<Object *> objects;
        const size_t id;

    public:
        Object();
        virtual ~Object(){};
        size_t getId() const noexcept
        {
            return id;
        }
        std::string getName() const noexcept
        {
            return type(self) + std::to_string(id);
        }
        static Object &get(size_t id, LINE_INFO)
        {
            try
            {
                return *objects.at(id);
            }
            catch (const std::out_of_range &e)
            {
                PRINT_TRACE;
                __throw_exception_again;
            }
        }
    };
}

#endif // __SOBJECT_H__