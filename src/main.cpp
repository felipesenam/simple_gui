#include "sapplication.hpp"
#include "scolors.hpp"

#include "widgets/slabel.hpp"

using namespace sgui;
using namespace sgui::Colors;

int main(/*int argc, char const *argv[]*/)
{
    WindowConfig config;
    config.renderer.drawColor = Colors::MintCream;

    Application app(config);
    auto &window = app.windows.get("main");
    auto &label1 = window.widgets.create<Label>();
    label1.color.hex(Red);
    label1.text = "Hello world";

    return app();
}
