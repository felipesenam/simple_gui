#include "sapplication.hpp"
#include "scolors.hpp"

#include "widgets/sflex.hpp"
#include "widgets/slabel.hpp"

using namespace sgui;
using namespace sgui::Colors;

int main(/*int argc, char const *argv[]*/)
{
    ApplicationConfig config;
    config.window.renderer.drawColor = Gray;

    Application app(config);

    auto &window = app.windows.get("main");

    auto &flex = window.container.create<Flex>();

    auto &label1 = flex.create<Label>();
    label1.font.color = Blue;
    label1.text = "Hello world";

    auto &label2 = flex.create<Label>();
    label2.font.color = Lime;
    label2.text = "World hello";

    return app();
}
