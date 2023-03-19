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

    auto &row = window.create<Row>();
    row.style.horizontalAlign = center;
    auto &calculator = window.create<Column>();
    calculator.size = 8;
    row.add(calculator);

    auto &displayRow = window.create<Row>();

    displayRow.style.horizontalAlign = right;
    auto &displayCol = window.create<Column>();
    displayRow.add(displayCol);
    displayCol.geometry.margin = 4;

    auto &display = window.create<Label>();
    displayCol.add(display);
    display.geometry.padding = 5;
    display.text = "0";

    auto &keyboard1 = window.create<Row>();
    keyboard1.geometry.margin = 3;
    keyboard1.style.justifyContent = around;

    auto &colnum7 = window.create<Column>();
    auto &num7 = window.create<Label>();
    num7.text = "7";
    num7.geometry.padding = 4;
    colnum7.add(num7);

    auto &colnum8 = window.create<Column>();
    auto &num8 = window.create<Label>();
    num8.text = "8";
    num8.geometry.padding = 4;
    colnum8.add(num8);

    auto &colnum9 = window.create<Column>();
    auto &num9 = window.create<Label>();
    num9.text = "9";
    num9.geometry.padding = 4;
    colnum9.add(num9);
    keyboard1.add(colnum7, colnum8, colnum9);

    auto &keyboard2 = window.create<Row>();
    keyboard2.geometry.margin = 3;
    keyboard2.style.justifyContent = around;

    auto &colnum4 = window.create<Column>();
    auto &num4 = window.create<Label>();
    num4.text = "4";
    num4.geometry.padding = 4;
    colnum4.add(num4);

    auto &colnum5 = window.create<Column>();
    auto &num5 = window.create<Label>();
    num5.text = "5";
    num5.geometry.padding = 4;
    colnum5.add(num5);

    auto &colnum6 = window.create<Column>();
    auto &num6 = window.create<Label>();
    num6.text = "6";
    num6.geometry.padding = 4;
    colnum6.add(num6);
    keyboard2.add(colnum4, colnum5, colnum6);

    auto &keyboard3 = window.create<Row>();
    keyboard3.geometry.margin = 3;
    keyboard3.style.justifyContent = around;

    auto &colnum1 = window.create<Column>();
    auto &num1 = window.create<Label>();
    num1.text = "1";
    num1.geometry.padding = 4;
    colnum1.add(num1);

    auto &colnum2 = window.create<Column>();
    auto &num2 = window.create<Label>();
    num2.text = "2";
    num2.geometry.padding = 4;
    colnum2.add(num2);

    auto &colnum3 = window.create<Column>();
    auto &num3 = window.create<Label>();
    num3.text = "3";
    num3.scheme.hover.color = Red;
    num3.geometry.padding = 4;
    num3.events["hover"] = []()
    {
        Warn("kk");
    };
    colnum3.add(num3);

    keyboard3.add(colnum1, colnum2, colnum3);

    calculator.add(displayRow, keyboard1, keyboard2, keyboard3);

    return app();
}
