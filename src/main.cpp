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

    auto numericRow = [&window, &display](int begin, int end) -> Widget &
    {
        auto &row = window.create<Row>();
        row.geometry.margin = 3;
        row.style.justifyContent = around;

        for (int i = begin; i <= end; ++i)
        {
            auto &col = window.create<Column>();
            auto &num = window.create<Label>();
            num.text = std::to_string(i);
            num.geometry.padding = 4;
            num.scheme.hover.color = Red;
            col.events["hover"] = [&num, &display]()
            {
                Warn("clicked");
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
