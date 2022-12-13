#ifndef __SWINDOW_H__
#define __SWINDOW_H__

#include "score.hpp"
#include "srenderer.hpp"
#include "swidget.hpp"
namespace PROJECT_NAMESPACE
{
    struct WindowConfig
    {
        std::string title = "Window";
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        int width = 600;
        int height = 400;

        struct Options
        {
            // SDL_WINDOW_FULLSCREEN
            bool isFullscreen = false;

            // SDL_WINDOW_SHOWN
            bool isShown = true;

            // SDL_WINDOW_HIDDEN
            bool isHidden = false;

            // SDL_WINDOW_BORDERLESS
            bool isBorderless = false;

            // SDL_WINDOW_RESIZABLE
            bool isResizable = false;

            // SDL_WINDOW_MINIMIZED
            bool isMinimized = false;

            // SDL_WINDOW_MAXIMIZED
            bool isMaximized = false;

            // SDL_WINDOW_FULLSCREEN_DESKTOP
            bool isFullscreenDesktop = false;

            // SDL_WINDOW_ALWAYS_ON_TOP
            bool isAlwaysOnTop = false;

            // SDL_WINDOW_SKIP_TASKBAR
            bool skipsTaskbar = false;

            Uint32 get() const noexcept
            {
                return (
                    (this->isFullscreen ? SDL_WINDOW_FULLSCREEN : 0) |
                    (this->isHidden ? SDL_WINDOW_HIDDEN : 0) |
                    (this->isShown ? SDL_WINDOW_SHOWN : 0) |
                    (this->isBorderless ? SDL_WINDOW_BORDERLESS : 0) |
                    (this->isResizable ? SDL_WINDOW_RESIZABLE : 0) |
                    (this->isMinimized ? SDL_WINDOW_MINIMIZED : 0) |
                    (this->isMaximized ? SDL_WINDOW_MAXIMIZED : 0) |
                    (this->isFullscreenDesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) |
                    (this->isAlwaysOnTop ? SDL_WINDOW_ALWAYS_ON_TOP : 0) |
                    (this->skipsTaskbar ? SDL_WINDOW_SKIP_TASKBAR : 0));
            }
        };
        Options options;
        RendererConfig renderer;
    };
    class Window : public Object<Window>
    {
    private:
        static size_t windowCount;
        SDL_Window *window = nullptr;

        friend Renderer;

        bool active = true;
        bool shown = true;

    public:
        Window(const WindowConfig &config = WindowConfig());
        ~Window();

        Renderer renderer;

        bool isActive() const;
        bool isShown() const;

        void show();
        void hide();
        void destroy();

        WidgetManager widgets;

        struct Behavior
        {
            bool destroyOnClose = true;
        };
        Behavior behavior;

        void handleEvent(const SDL_Event &event);
        void update();
        void draw();
    };
}

#endif // __SWINDOW_H__