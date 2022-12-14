#include "sapplication.hpp"

namespace PROJECT_NAMESPACE
{

    Application::Application(const WindowConfig &windowConfig)
    {
        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

        if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
            const int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
            if (IMG_Init(flags) != flags)
            {
                Warn("IMG_Init: " << IMG_GetError());
            }
            if (TTF_Init() != 0)
            {
                Warn("TTF_Init: " << TTF_GetError());
            }
        }
        else
        {
            SDL_PrintError(Error);
            exit(-1);
        }
        windows.create("main", windowConfig);
    }
    Application::~Application()
    {
    }
    void Application::stop()
    {
        running = false;
    }
    int Application::run()
    {
        running = true;

        Uint32 fStart, frameTime;
        const double frameDelay = 1000 / (double)targetFPS;
        while (running && windows.hasActiveWindows())
        {
            fStart = SDL_GetTicks();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    break;
                }
                windows.handleEvent(event);
            }
            windows.runLogic();

            frameTime = SDL_GetTicks() - fStart;

            const float elapsed = frameTime / (float)SDL_GetPerformanceFrequency() * 1000.0f;

            SDL_Delay(std::floor(frameDelay - elapsed));
        }

        running = false;

        return 0;
    }

    int Application::operator()()
    {
        return run();
    }

    bool Application::isRunning() const
    {
        return running;
    }
}
