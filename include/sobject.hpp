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
        static size_t objectCounter;
        static size_t objectCount;
        static std::vector<Object<T> *> objects;
        const size_t id;

    public:
        Object() : id(objectCounter++)
        {
            Debug("New object " << self.getName());
            objects.emplace_back(this);
            objectCounter++;
        }
        virtual ~Object()
        {
            Debug("Destroying " << self.getName());
            objectCounter--;
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
        static void atexit_handler()
        {
            if (objectCounter)
            {
                Debug("There's " << objectCounter << " lost objects");
            }
            else
            {
                Debug("All objects has been freed");
            }
        }
    };

    template <typename T>
    size_t Object<T>::objectCount = 0;

    template <typename T>
    size_t Object<T>::objectCounter = 0;

    template <typename T>
    std::vector<Object<T> *> Object<T>::objects;

    // template <typename T>
    // std::atexit(Object<T>::atexit_handler);
}

#endif // __SOBJECT_H__