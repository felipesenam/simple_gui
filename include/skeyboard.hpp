#ifndef __SKEYBOARD_H__
#define __SKEYBOARD_H__

#include "score.hpp"

namespace sgui
{

    class KeyboardController
    {
    private:
    public:
        std::map<std::pair<SDL_Keycode, Uint16>, std::function<void()>> event_map;

        void handleKeyboardEvent(const SDL_Event &e)
        {
            if (e.type == SDL_KEYDOWN)
            {
                auto it = event_map.find(std::make_pair(e.key.keysym.sym, e.key.keysym.mod));
                if (it != event_map.end())
                {
                    it->second();
                }
            }
        }
    };
}

#endif // __SKEYBOARD_H__