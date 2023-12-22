#ifndef __SOBJECT_H__
#define __SOBJECT_H__

#include "score.hpp"
#include "type.hpp"

namespace sgui
{
    template <typename T>
    class Object
    {
    private:
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

        std::string getName() const noexcept
        {
            return type(*this) + "{id=" + std::to_string(id) + "}";
        }

    protected:
        const size_t id;

    public:
        Object() : id(objectCounter++)
        {
            this->uid = std::to_string(id);
            this->init();
        }
        virtual ~Object()
        {
            Debug("Destroying " << *this);
            objectCounter--;
        }

        std::string uid;

        void init()
        {
            if (objectType.empty())
                objectType = type(*this);

            objects.emplace_back(this);
            objectCounter++;

            Debug("New object " << *this);
        }

        size_t getId() const noexcept
        {
            return id;
        }

        std::string getUId() const noexcept
        {
            return uid;
        }

        template <typename _T>
        static _T &get(const std::string &uid)
        {
            auto it = std::find_if(objects.begin(), objects.end(), [&uid](const Object<T> *obj)
                                   { return obj->uid == uid; });

            if (it == objects.end())
                throw std::runtime_error("Object not found '" + uid + "'");

            return *dynamic_cast<_T *>(*it);
        }

        friend std::ostream &operator<<(std::ostream &os, const Object<T> &obj)
        {
            return os << objectType << " " << obj.uid;
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