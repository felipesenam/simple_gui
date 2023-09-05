
// #include "examples/mandelbrot.hpp"

#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "sapplication.hpp"
#include "widgets/slabel.hpp"
#include "widgets/sflex.hpp"

using namespace sgui;

int example()
{
    std::string config_file("assets/example.json");
    std::ifstream ifstream(config_file);
    json data = json::parse(ifstream);
    Application app;
    from_json(data, app);

    auto &window = app.windows.create("main");

    auto &label = window.container->get<Label>("mylabel");
    label.events["clicked"] = [&label]()
    {
        Debug("Clicked!");
        label.text = "Clicked!";
    };

    return app();
}

int main()
{
    return example();
}
