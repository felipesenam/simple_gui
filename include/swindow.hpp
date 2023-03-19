#ifndef __SWINDOW_H__
#define __SWINDOW_H__

#include "score.hpp"
#include "sfont.hpp"
#include "srenderer.hpp"
#include "swidget.hpp"
#include "widgets/sflex.hpp"
namespace PROJECT_NAMESPACE
{
    struct WindowConfig
    {
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

        struct Behavior
        {
            bool destroyOnClose = true;
        };

        std::string title = "Window";
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        int width = 600;
        int height = 400;

        std::string defaultFontPath = "assets/fonts/segoeui.ttf";
        unsigned defaultFontSize = 12;

        /**
         * @brief Window options. Note this alter this options after after window initialized has no effect.
         *
         */
        Options options;
        Behavior behavior;
        RendererConfig renderer;
    };

    class WindowManager
    {
    private:
        std::unordered_map<std::string, Window *> windows;

        friend class Application;

    public:
        WindowManager();
        ~WindowManager();

        bool hasActiveWindows() const;
        Window &get(const std::string &id);
        Window &create(const std::string &id, const WindowConfig &windowConfig = WindowConfig());
        void handleEvent(const SDL_Event &);
        void runLogic();
    };

    class Window : public Object<Window>
    {
    private:
        static size_t windowCount;
        SDL_Window *window = nullptr;

        bool active = true;
        bool shown = true;

        friend Renderer;

    public:
        Window(const WindowConfig &config = WindowConfig());
        ~Window();

        Renderer renderer;

        WindowConfig config;
        Flex container;

        bool isActive() const;
        bool isShown() const;

        /**
         * @brief
         *
         * @return std::pair<int, int> Which represents respectively window width and height.
         */
        std::pair<int, int> size() const
        {
            std::pair<int, int> _size;
            SDL_GetWindowSize(window, &_size.first, &_size.second);
            return _size;
        }

        template <typename T, typename... Args>
        T &create(Args &&...args)
        {
            return *new T(self, args...);
        }

        void show();
        void hide();
        void destroy();

        void handleEvent(const SDL_Event &event);
        void update();
        void draw();
    };
}

#endif // __SWINDOW_H__