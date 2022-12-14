#ifndef __SAPP_H__
#define __SAPP_H__

#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    class Application : public Object<Application>
    {
    private:
        unsigned targetFPS = 60;

        bool running = false;

        public:
        Application(const WindowConfig &windowConfig = WindowConfig());
        ~Application();

        SDL_Event event;
        WindowManager windows;

        int run();
        int operator()();
        void stop();

        bool isRunning() const;
    };
}

#endif // __SAPP_H__