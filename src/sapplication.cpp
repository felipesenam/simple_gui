#include "sapplication.hpp"

namespace PROJECT_NAMESPACE
{
    Application::Application()
    {
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
    }

    Application::~Application()
    {
        Debug("Shutting down SDL subsystems...");
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        Debug("Shutdown complete.");
    }
    void Application::stop()
    {
        m_running = false;
    }

    int Application::run()
    {
        m_running = true;

        windows.init();

        Uint32 fStart, frameTime;
        const float frameDelay = 1000.0f / config.targetFPS;
        while (m_running && windows.hasActiveWindows())
        {
            fStart = SDL_GetTicks();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    m_running = false;
                    break;
                }
                windows.handleEvent(event);
            }
            windows.runLogic();

            frameTime = SDL_GetTicks() - fStart;

            const float elapsed = frameTime / (float)SDL_GetPerformanceFrequency() * 1000.0f;

            SDL_Delay(std::floor(frameDelay - elapsed));
        }

        m_running = false;

        return 0;
    }

    int Application::operator()()
    {
        return run();
    }

    bool Application::isRunning() const noexcept
    {
        return m_running;
    }
}
