#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include "sconsole.hpp"
#include "sobject.hpp"
#include "ssdl.hpp"

#define RGB(x) x.r, x.g, x.b
#define RGBA(x) x.r, x.g, x.b, x.a

#define SDL_PrintIfError(alert)          \
    if (strcmp(SDL_GetError(), "") != 0) \
        alert(SDL_GetError());           \
    SDL_ClearError();

#define SDL_PrintError(alert) \
    alert(SDL_GetError());    \
    SDL_ClearError();

#define SETATTR_IF_JSON_CONTAINS(j, attr, key) \
    if (j.contains(#key))                      \
        j[#key].get_to(attr.key);
