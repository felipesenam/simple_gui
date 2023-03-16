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

        static size_t objectCounter;
        static size_t objectCount;
        static std::string objectType;
        static std::vector<Object<T> *> objects;

        const static int err;
        static void atexit_handler() // cppcheck-suppress unusedPrivateFunction
        {
            if (objectCount)
            {
                Debug("There's " << objectCount << " lost " << objectType);
            }
            else
            {
                Debug("All " << objectType << " has been freed");
            }
        }

    protected:
        const size_t id;

    public:
        Object() : id(objectCounter++)
        {
            if (objectType.empty())
                objectType = type(self);

            Debug("New object " << self.getName());
            objects.emplace_back(this);
            objectCounter++;
        }
        virtual ~Object()
        {
            Debug("Destroying " << self.getName());
            objectCounter--;
        }

        size_t getId() const noexcept
        {
            return id;
        }
        std::string getName() const noexcept
        {
            return type(self) + "{id=" + std::to_string(id) + "}";
        }
        static T &get(size_t id, LINE_INFO)
        {
            try
            {
                return *dynamic_cast<T *>(objects.at(id));
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
    size_t Object<T>::objectCounter = 0;

    template <typename T>
    std::string Object<T>::objectType = "";

    template <typename T>
    std::vector<Object<T> *> Object<T>::objects;
}

#endif // __SOBJECT_H__