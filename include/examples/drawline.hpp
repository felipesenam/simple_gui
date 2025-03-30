#ifndef FC2BADD0_E415_43B3_8072_E9A488DDAD25
#define FC2BADD0_E415_43B3_8072_E9A488DDAD25

#include <random>

#include "sapplication.hpp"
#include "swidgets.hpp"

using namespace sgui;

template <typename T>
T randomFrom(const T min, const T max)
{
    static std::random_device rdev;
    static std::default_random_engine re(rdev());
    typedef typename std::conditional<
        std::is_floating_point<T>::value,
        std::uniform_real_distribution<T>,
        std::uniform_int_distribution<T>>::type dist_type;
    dist_type uni(min, max);
    return static_cast<T>(uni(re));
}

void randomInitialize(Widget &widget)
{
    widget.geometry.padding.top = randomFrom<int>(5, 15);
    widget.geometry.padding.left = randomFrom<int>(5, 15);
    widget.geometry.padding.bottom = randomFrom<int>(5, 15);
    widget.geometry.padding.right = randomFrom<int>(5, 15);
    widget.geometry.margin.top = randomFrom<int>(5, 15);
    widget.geometry.margin.left = randomFrom<int>(5, 15);
    widget.geometry.margin.bottom = randomFrom<int>(5, 15);
    widget.geometry.margin.right = randomFrom<int>(5, 15);
}

int drawline()
{
    // std::string config_file("assets/drawline.json");
    // std::ifstream ifstream(config_file);
    // json data = json::parse(ifstream);
    Application app;
    // from_json(data, app);
    ApplicationWindowConfig config;
    config.window.height = 600;
    config.renderer.drawColor = Colors::White;
    config.window.options.isAlwaysOnTop = true;
    config.window.options.isResizable = true;

    auto &window = app.windows.create("main", config);

    auto &row = window.container->create<Row>("row");
    row.gap = 0;

    auto &column01 = row.create<Column>("column01");
    column01.gap = 5;

    randomInitialize(column01.create<Label>("text01", "ABC"));
    randomInitialize(column01.create<Label>("text02", "ABCABC"));
    randomInitialize(column01.create<Label>("text03", "ABCABCABC"));
    randomInitialize(column01.create<Label>("text04", "ABCABCABCABC"));

    auto &label = column01.create<Label>("text05", "ABCABCABCABC123");
    label.scheme.normal.background = Colors::Coral;
    randomInitialize(label);

    auto &column02 = row.create<Column>("column02");
    auto &row01 = column02.create<Row>("row01");
    row01.gap = 5;

    randomInitialize(row01.create<Label>("text11", "ABC"));
    randomInitialize(row01.create<Label>("text21", "ABC"));

    randomInitialize(row);
    randomInitialize(row01);
    randomInitialize(column01);
    randomInitialize(column02);

    row.geometry.behavior = normal;
    row.geometry.overflow = hidden;
    row.geometry.width = 500;
    row.geometry.height = 200;
    row.events["windowSizeChanged"] = [&window](Widget &widget)
    {
        auto windowSize = window.size();
        widget.geometry.width = .8 * windowSize.first;
        widget.geometry.height = .5 * windowSize.second;
    };

    column02.geometry.behavior = normal;
    column02.geometry.width = 200;
    column02.geometry.height = 200;

    app.dump("assets/drawline.dump.json");

    return app();
}

#endif /* FC2BADD0_E415_43B3_8072_E9A488DDAD25 */
