#include "sapplication.hpp"
#include "scolors.hpp"

#include "widgets/slabel.hpp"

using namespace sgui;

int main(/*int argc, char const *argv[]*/)
{
    WindowConfig config;
    config.renderer.drawColor = Colors::MintCream;

    Application app(config);
    auto &window = app.windows.get("main");
    auto &label = window.widgets.create<Label>();

    return app();
}
