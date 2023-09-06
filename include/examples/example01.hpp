#ifndef E9E8C19E_3881_43A3_863B_6EB6E3A33FA6
#define E9E8C19E_3881_43A3_863B_6EB6E3A33FA6

#include "sapplication.hpp"
#include "swidgets.hpp"

using namespace sgui;

int example()
{
    std::string config_file("assets/example.json");
    std::ifstream ifstream(config_file);
    json data = json::parse(ifstream);
    Application app;
    from_json(data, app);

    auto &window = app.windows.get("main");
    auto &main_container = *window.container;

    auto &label = main_container
                      .get<Row>("row")
                      .get<Column>("col")
                      .get<Label>("label");

    label.events["clicked"] = [&label]()
    {
        Debug("Clicked!");
        label.text = "Clicked!";
    };

    app.dump("assets/example.dump.json");

    return app();
}

#endif /* E9E8C19E_3881_43A3_863B_6EB6E3A33FA6 */
