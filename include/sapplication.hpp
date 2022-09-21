#ifndef __SAPP_H__
#define __SAPP_H__

#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    class Application
    {
    private:
        unsigned targetFPS = 60;

        bool running = false;

        class WindowManager
        {
        private:
            std::unordered_map<std::string, Window *> windows;

        public:
            WindowManager();
            ~WindowManager();
            bool hasActiveWindows() const;
            Window &get(const std::string &id);
            Window &create(const std::string &id, const WindowConfig &windowConfig = WindowConfig());
            void handleEvent(const SDL_Event &);
            void runLogic();
        };

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