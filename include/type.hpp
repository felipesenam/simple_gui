#ifndef __TYPE_H__
#define __TYPE_H__

#include <string>
#include <typeinfo>

std::string demangle(const char *name);

template <class T>
std::string type(const T &t)
{
    return demangle(typeid(t).name());
}

#endif // __TYPE_H__