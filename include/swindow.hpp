#ifndef __SWINDOW_H__
#define __SWINDOW_H__

#include "score.hpp"
#include "sgeometry.hpp"
#include "scolor.hpp"
#include "scolors.hpp"
#include "skeyboard.hpp"

namespace sgui
{
    class Flex;
    class Font;
    class Window;

    using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
    using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

    struct RendererConfig
    {
        struct Options
        {
            // SDL_RENDERER_ACCELERATED
            bool usesHardwareAcceleration = false;

            // SDL_RENDERER_PRESENTVSYNC
            bool usesVSync = true;

            Uint32 get() const noexcept
            {
                return (
                    (this->usesHardwareAcceleration ? SDL_RENDERER_ACCELERATED : 0) |
                    (this->usesVSync ? SDL_RENDERER_PRESENTVSYNC : 0));
            }

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Options, usesHardwareAcceleration, usesVSync);
        };

        int index = -1;
        Color drawColor;

        Options options;

        enum BlendMode
        {
            none = SDL_BLENDMODE_NONE,
            alpha = SDL_BLENDMODE_BLEND,
            additive = SDL_BLENDMODE_ADD,
            modulate = SDL_BLENDMODE_MOD,
            multiply = SDL_BLENDMODE_MUL
        };
        BlendMode blendMode = alpha;
        NLOHMANN_JSON_SERIALIZE_ENUM(BlendMode, {
                                                    {BlendMode::none, "none"},
                                                    {BlendMode::alpha, "alpha"},
                                                    {BlendMode::additive, "additive"},
                                                    {BlendMode::modulate, "modulate"},
                                                    {BlendMode::multiply, "multiply"},
                                                })

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(RendererConfig, index, drawColor, options, blendMode);
    };

    class Renderer
    {
    private:
        SDL_Renderer *renderer = nullptr;

    public:
        Renderer();
        ~Renderer();

        Color drawColor;

        bool isCreated() const noexcept;

        void create(Window &, const RendererConfig &);
        void destroy();

        void clear();
        void present();

        SDL_Texture *createTextureFromSurface(SDL_Surface *surface);
        void drawTexture(SDL_Texture *texture, Rect *src, Rect *dest);
        void drawRectangle(const Rect &dest, const Color &color);
        void drawCross(const Rect &dest, const Color &color);
        void drawFillRectangle(const Rect &dest, const Color &color);
        SDL_Texture *renderText(const std::string &text, Font &font, Geometry &geometry, const Color &background, Uint32 wrapLenght = 0);
    };

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

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Options,
                                                        isFullscreen,
                                                        isShown,
                                                        isHidden,
                                                        isBorderless,
                                                        isResizable,
                                                        isMinimized,
                                                        isMaximized,
                                                        isFullscreenDesktop,
                                                        isAlwaysOnTop,
                                                        skipsTaskbar);

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

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Behavior, destroyOnClose);
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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WindowConfig, title, x, y, width, height, defaultFontPath, defaultFontSize, options, behavior, renderer);
    };

    class Window : public Object<Window>
    {
    private:
        static size_t windowCount;
        SDL_Window *window = nullptr;

        bool active = true;
        bool shown = true;

        friend Renderer;
        friend Widget;

        std::unordered_map<std::string, Widget *> widgets;

    public:
        Window(const WindowConfig &config = WindowConfig());
        ~Window();

        Renderer renderer;

        WindowConfig config;
        KeyboardController keyboard;

        Flex *container = nullptr;

        template <typename T>
        T &get(const std::string &uid)
        {
            auto *widget = dynamic_cast<T *>(this->widgets[uid]);
            if (!widget)
            {
                throw std::runtime_error("Widget with uid '" + uid + "' not found.");
            }

            return *widget;
        }

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

        void show();
        void hide();
        void destroy();

        void init();
        void handleEvent(const SDL_Event &event);
        void update();
        void draw();

        friend void to_json(json &j, const Window &p);
        friend void from_json(const json &j, Window &p);
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
        void init();
        void handleEvent(const SDL_Event &);
        void runLogic();

        friend void to_json(json &j, const WindowManager &p)
        {
            for (auto pair : p.windows)
                j["windows"][pair.first] = *pair.second;
        }
        friend void from_json(const json &j, WindowManager &p)
        {
            for (auto window : j["windows"].items())
            {
                from_json(window.value(), p.create(window.key()));
            }
        }
    };

}

#endif // __SWINDOW_H__