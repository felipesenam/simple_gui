#ifndef __SOBJECT_H__
#define __SOBJECT_H__

#include "score.hpp"
#include "type.hpp"

namespace PROJECT_NAMESPACE
{
    template <typename T>
    class Object
    {
    private:
        Object(const Object &) = delete;

    protected:
        static size_t objectCount;
        static std::vector<Object<T> *> objects;
        const size_t id;

    public:
        Object() : id(objectCount++)
        {
            Debug("New object " << self.getName());
            objects.emplace_back(this);
        }
        virtual ~Object()
        {
            Debug("Destroying " << self.getName());
        };
        size_t getId() const noexcept
        {
            return id;
        }
        std::string getName() const noexcept
        {
            return type(self) + "{id=" + std::to_string(id) + "}";
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

    template <typename T>
    size_t Object<T>::objectCount = 0;

    template <typename T>
    std::vector<Object<T> *> Object<T>::objects;
}

#endif // __SOBJECT_H__