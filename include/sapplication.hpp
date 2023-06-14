#ifndef __SAPP_H__
#define __SAPP_H__

#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    class ApplicationConfig
    {
    public:
        unsigned targetFPS = 60;
        WindowConfig window;
    };

    class Application : public Object<Application>
    {
    private:
        bool m_running = false;

    public:
        Application(const ApplicationConfig &config = ApplicationConfig());
        ~Application();

        SDL_Event event;
        WindowManager windows;
        ApplicationConfig config;

        int run();
        int operator()();
        void stop();

        bool isRunning() const noexcept;
    };
}

#endif // __SAPP_H__