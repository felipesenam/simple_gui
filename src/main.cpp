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

    window.container.geometry.padding = 5;
    window.container.style.direction = horizontal;
    window.container.style.verticalAlign = middle;
    window.container.style.justifyContent = between;

    auto &row = window.container.create<Row>();
    row.style.horizontalAlign = center;

    auto &column1 = row.create<Column>();
    column1.size = 2;

    auto &label1 = column1.create<Label>();
    label1.geometry.padding = 5;
    label1.font.color = Red;
    label1.text = "RRR";

    auto &column2 = row.create<Column>();
    auto &label2 = column2.create<Label>();
    label2.geometry.padding = 10;
    label2.font.color = Blue;
    label2.text = "BBB";

    auto &column3 = row.create<Column>();
    auto &label3 = column3.create<Label>();
    label3.geometry.padding = 15;
    label3.font.color = Lime;
    label3.text = "GGG";

    auto &column4 = row.create<Column>();
    auto &label4 = column4.create<Label>();
    label4.geometry.padding = 20;
    label4.font.color = White;
    label4.text = "WWW";

    return app();
}
