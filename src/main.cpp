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
    displayRow.scheme.normal.background = Plum;

    auto &displayCol = window.create<Column>();
    displayRow.add(displayCol);
    displayCol.geometry.margin = 4;

    auto &display = window.create<Label>();
    displayCol.add(display);
    display.geometry.padding = 5;
    display.text = "0";
    display.font.open("assets/fonts/arial.ttf", 24);

    auto numericRow = [&window, &display](int begin, int end) -> Widget &
    {
        auto &row = window.create<Row>();
        row.geometry.margin = 3;
        row.style.justifyContent = around;

        for (int i = begin; i <= end; ++i)
        {
            auto &col = window.create<Column>();
            auto &num = window.create<Label>();

            std::string number = std::to_string(i);
            num.text = number;
            num.geometry.padding = 8;
            num.scheme.hover.color = Red;
            num.font.open("assets/fonts/arial.ttf", 24);
            window.keyboard.event_map[{number[0], KMOD_NONE}] = col.events["clicked"] = [&num, &display]()
            {
                display.text += num.text;
            };
            col.add(num);

            row.add(col);
        }

        return row;
    };

    calculator.add(
        displayRow,
        numericRow(7, 9),
        numericRow(4, 6),
        numericRow(1, 3));

    return app();
}
