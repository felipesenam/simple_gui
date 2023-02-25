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
    config.window.options.isResizable = true;

    Application app(config);

    auto &window = app.windows.get("main");

    window.container.verticalAlign = bottom;
    window.container.horizontalAlign = right;

    auto &flex = window.container.create<Flex>();
    flex.geometry.padding = 5;
    flex.direction = horizontal;
    flex.verticalAlign = top;
    flex.horizontalAlign = left;

    auto window_size = window.size();
    flex.geometry.dest.x = 0;
    flex.geometry.dest.y = 0;
    flex.geometry.dest.w = window_size.first;
    flex.geometry.dest.h = window_size.second;

    auto &label1 = flex.create<Label>();
    label1.geometry.padding = 5;
    label1.font.color = Red;
    label1.text = "RRR";

    auto &label2 = flex.create<Label>();
    label2.geometry.padding = 10;
    label2.font.color = Blue;
    label2.text = "BBB";

    auto &label3 = flex.create<Label>();
    label3.geometry.padding = 15;
    label3.font.color = Lime;
    label3.text = "GGG";

    return app();
}
