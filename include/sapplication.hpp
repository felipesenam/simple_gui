#ifndef __SAPP_H__
#define __SAPP_H__

#include "score.hpp"
#include "swindow.hpp"

using json = nlohmann::json;

namespace sgui
{
    class ApplicationConfig
    {
    public:
        unsigned targetFPS = 60;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ApplicationConfig, targetFPS);
    };

    class Application : public Object<Application>
    {
    private:
        bool m_running = false;

    public:
        Application();
        Application(const std::string &config_file);
        ~Application();

        void dump(const std::string &config_file)
        {
            std::ofstream ofstream(config_file);
            json data = *this;
            ofstream << data.dump(4) << std::endl;
            ofstream.close();
        }

        SDL_Event event;
        WindowManager windows;
        ApplicationConfig config;

        // NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Application, config, windows);

        int run();
        int operator()();
        void stop();

        bool isRunning() const noexcept;

        friend void to_json(nlohmann::json &j, const Application &app)
        {
            j["config"] = app.config;
            j["windows"] = app.windows;
        }
        friend void from_json(const nlohmann::json &j, Application &app)
        {
            SETATTR_IF_JSON_CONTAINS(j, app, config);
            SETATTR_IF_JSON_CONTAINS(j, app, windows);
        }
    };
}

#endif // __SAPP_H__