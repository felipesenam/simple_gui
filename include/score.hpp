#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>

#include "sobject.hpp"

#define PROJECT_NAMESPACE sgui

#define PRINT_TRACE std::cerr << "From: " << __file << ":" << __line << std::endl
#define LINE_INFO int __line = __LINE__, const char *__file = __FILE__
#define self (*this)