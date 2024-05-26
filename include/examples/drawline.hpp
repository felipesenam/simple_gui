#ifndef FC2BADD0_E415_43B3_8072_E9A488DDAD25
#define FC2BADD0_E415_43B3_8072_E9A488DDAD25

#include "sapplication.hpp"
#include "swidgets.hpp"

using namespace sgui;

int drawline()
{
    // std::string config_file("assets/drawline.json");
    // std::ifstream ifstream(config_file);
    // json data = json::parse(ifstream);
    Application app;
    // from_json(data, app);
    ApplicationWindowConfig config;
    config.renderer.drawColor = Colors::White;
    config.window.options.isResizable = true;

    auto &window = app.windows.create("main", config);

    auto &row = window.container->create<Row>("row");
    row.gap = 0;
    // row.style.verticalAlign = top;
    row.style.verticalAlign = middle;
    // row.style.verticalAlign = bottom;
    // row.style.horizontalAlign = left;
    row.style.horizontalAlign = center;
    // row.style.horizontalAlign = right;
    row.geometry.padding.top = 23;
    row.geometry.padding.left = 11;
    row.geometry.padding.bottom = 7;
    row.geometry.padding.right = 5;
    row.geometry.margin.top = 5;
    row.geometry.margin.left = 7;
    row.geometry.margin.bottom = 11;
    row.geometry.margin.right = 23;

    auto &column01 = row.create<Column>("column01");
    column01.gap = 0;
    // column01.style.verticalAlign = top;
    column01.style.verticalAlign = middle;
    // column01.style.verticalAlign = bottom;
    // column01.style.horizontalAlign = left;
    column01.style.horizontalAlign = center;
    // column01.style.horizontalAlign = right;
    column01.geometry.padding.top = 23;
    column01.geometry.padding.left = 11;
    column01.geometry.padding.bottom = 7;
    column01.geometry.padding.right = 5;
    column01.geometry.margin.top = 5;
    column01.geometry.margin.left = 7;
    column01.geometry.margin.bottom = 41;
    column01.geometry.margin.right = 23;

    column01.create<Label>("text01", "ABC");
    column01.create<Label>("text02", "ABCABC");
    column01.create<Label>("text03", "ABCABCABC");
    column01.create<Label>("text04", "ABCABCABCABC");
    column01.create<Label>("text05", "ABCABCABCABCABC");

    auto &column02 = row.create<Column>("column02");
    column02.gap = 5;
    column02.geometry.margin = 5;
    column02.geometry.padding = 5;

    column02.create<Label>("text11", "ABC");
    column02.create<Label>("text12", "ABCABC");
    column02.create<Label>("text13", "ABCABCABC");
    column02.create<Label>("text14", "ABCABCABCABC");
    column02.create<Label>("text15", "ABCABCABCABCABC");
    column02.create<Label>("text25", "ABCABCABCABCABC");
    column02.create<Label>("text24", "ABCABCABCABC");
    column02.create<Label>("text23", "ABCABCABC");
    column02.create<Label>("text22", "ABCABC");
    column02.create<Label>("text21", "ABC");

    // auto &image = row.create<Bitmap>("image");

    // image.open("08da096ecf97050bed04e775dfdc3a29.png");
    // // image.geometry.behavior = Geometry::fill;
    // image.geometry.aspect = .3;

    app.dump("assets/drawline.dump.json");

    return app();
}

#endif /* FC2BADD0_E415_43B3_8072_E9A488DDAD25 */
