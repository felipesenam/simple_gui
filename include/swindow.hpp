#ifndef __SWINDOW_H__
#define __SWINDOW_H__

#include "score.hpp"
#include "scolors.hpp"
#include "skeyboard.hpp"

namespace sgui
{
    class Flex;
    class Font;
    class ApplicationWindow;
    class Widget;

    struct ApplicationWindowConfig
    {
        struct Behavior
        {
            bool destroyOnClose = true;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Behavior, destroyOnClose);
        };
        Behavior behavior;

        std::string defaultFontPath = "assets/fonts/segoeui.ttf";
        unsigned defaultFontSize = 12;

        WindowConfig window;
        RendererConfig renderer;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ApplicationWindowConfig, window, renderer, behavior, defaultFontPath, defaultFontSize);
    };

    class ApplicationWindow : public Object<ApplicationWindow>
    {
    private:
        static size_t windowCount;
        Window *window = nullptr;

        bool active = true;
        bool shown = true;

        friend Renderer;
        friend Widget;

        std::unordered_map<std::string, Widget *> widgets;

    public:
        ApplicationWindow(const ApplicationWindowConfig &config = ApplicationWindowConfig());
        ~ApplicationWindow();

        Renderer *renderer = nullptr;

        ApplicationWindowConfig config;
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

        void show();
        void hide();
        void destroy();

        void init();
        void handleEvent(const SDL_Event &event);
        void update();
        void draw();

        auto size() const{
            return window->size(); 
        }

        friend void to_json(json &j, const ApplicationWindow &p);
        friend void from_json(const json &j, ApplicationWindow &p);
    };

    class WindowManager
    {
    private:
        std::unordered_map<std::string, ApplicationWindow *> windows;

        friend class Application;

    public:
        WindowManager();
        ~WindowManager();

        bool hasActiveWindows() const;
        ApplicationWindow &get(const std::string &id);
        ApplicationWindow &create(const std::string &id, const ApplicationWindowConfig &windowConfig = ApplicationWindowConfig());
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