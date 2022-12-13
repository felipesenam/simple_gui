#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "sconsole.hpp"
#include "sobject.hpp"

#define PROJECT_NAMESPACE sgui

#define PRINT_TRACE std::cerr << "From: " << __file << ":" << __line << std::endl
#define LINE_INFO int __line = __LINE__, const char *__file = __FILE__
#define self (*this)

#define RGB(x) x.r, x.g, x.b
#define RGBA(x) x.r, x.g, x.b, x.a

#define SDL_PrintIfError(alert)          \
    if (strcmp(SDL_GetError(), "") != 0) \
        alert(SDL_GetError());           \
    SDL_ClearError();

#define SDL_PrintError(alert) \
    alert(SDL_GetError());    \
    SDL_ClearError();
