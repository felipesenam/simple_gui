#ifndef CCCB18B0_6929_4A1C_9604_57FB6EF1A893
#define CCCB18B0_6929_4A1C_9604_57FB6EF1A893

#include <thread>

#include "sapplication.hpp"
#include "swidgets.hpp"

#define MODIFIER 7
#define THREADS 8

using namespace PROJECT_NAMESPACE;
using namespace PROJECT_NAMESPACE::Colors;

using ld = long double;

ld mapToReal(int x, int w, ld minR, ld maxR)
{
    return x * ((maxR - minR) / w) + minR;
}
ld mapToImaginary(int y, int h, ld minI, ld maxI)
{
    return y * ((maxI - minI) / h) + minI;
}
int findMandelbrot(ld cr, ld ci, int maxN)
{
    int i = 0;
    ld zr = 0., zi = 0.;
    while (i < maxN && zr * zr + zi * zi < 4.)
    {
        ld tmp = zr * zr - zi * zi + cr;
        zi = 2. * zr * zi + ci;
        zr = tmp;
        i++;
    }
    return i;
}
void calc(Bitmap &bitmap, ld minR, ld maxR, ld minI, ld maxI, int maxN, int row, int column, size_t nElements)
{
    size_t elements = 0;
    for (size_t j = row; j < bitmap.height(); ++j)
    {
        for (size_t i = elements == 0 ? column : 0; i < bitmap.width(); ++i)
        {
            ld cr = mapToReal(i, bitmap.width(), minR, maxR);
            ld ci = mapToImaginary(j, bitmap.height(), minI, maxI);
            int n = findMandelbrot(cr, ci, maxN);

            int value = (n * MODIFIER) % 360;
            bitmap.at(i, j).hsl(value, n == 1 ? 0 : 1, n == 360 ? 0 : .5);

            elements++;
            if (elements == nElements)
                return;
        }
    }
}

void updateGrid(sgui::Bitmap &bitmap, ld minR, ld maxR, ld minI, ld maxI, int maxN = 360)
{
    std::vector<std::thread *> threads;
    const size_t tElements = bitmap.width() * bitmap.height();
    const size_t nElements = ceil(tElements / ld(THREADS));
    for (size_t i = 0, j = 0; i < tElements; i += nElements, ++j)
    {
        threads.emplace_back(new std::thread(calc,
                                             std::ref(bitmap),
                                             minR, maxR,
                                             minI, maxI,
                                             maxN,
                                             i / bitmap.width(),
                                             i % bitmap.width(),
                                             nElements));
    }
    for (auto thread : threads)
    {
        thread->join();
        delete thread;
    }
}

ld getMaxI(ld minR, ld maxR, ld minI, size_t width, size_t height)
{
    return minI + (maxR - minR) * height / width;
}

void updateLabels(ld minR, ld maxR, ld minI, Label &minRbox, Label &maxRbox, Label &minIbox)
{
    minRbox.text = std::to_string(minR);
    maxRbox.text = std::to_string(maxR);
    minIbox.text = std::to_string(minI);
}

int mandelbrot()
{
    std::string config_file("assets/mandelbrot.json");
    std::ifstream ifstream(config_file);
    json data = json::parse(ifstream);
    Application app;
    from_json(data, app);

    auto &window = app.windows.get("main");
    auto &container = *window.container;

    auto &content = container.get<Row>("content");

    auto &left_col = content.get<Column>("left_col");
    auto &right_col = content.get<Column>("right_col");

    auto &bitmap = left_col.get<Bitmap>("bitmap");

    auto &values_col = right_col
                           .get<Row>("values_row")
                           .get<Column>("values_col");

    auto &minRbox = values_col.get<Label>("minRbox");
    auto &maxRbox = values_col.get<Label>("maxRbox");
    auto &minIbox = values_col.get<Label>("minIbox");

    /***** Setup values *****/
    const size_t width = bitmap.width();
    const size_t height = bitmap.height();

    ld minR = -2;
    ld maxR = .8;
    ld minI = -1.4;
    ld maxI = getMaxI(minR, maxR, minI, width, height);

    updateGrid(bitmap, minR, maxR, minI, maxI);
    updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    /************************/

    auto &zoom_control_row = right_col.get<Row>("zoom_control_row");

    auto &zoomIn = zoom_control_row.get<Label>("zoomIn");
    auto &zoomOut = zoom_control_row.get<Label>("zoomOut");

    zoomIn.events["clicked"] = [&]()
    {
        ld deltaR = maxR - minR;
        if (deltaR < 0.0000000000001)
        {
            Warn("Zoom limit reached!");
        }
        ld value = (deltaR / 10) / 2;

        minR += value;
        maxR -= value;
        minI += ((maxI - minI) / 10) / 2;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    zoomOut.events["clicked"] = [&]()
    {
        ld deltaR = maxR - minR;
        if (deltaR > 2)
            return;
        ld value = (maxR - minR) / 10;
        minR -= value;
        maxR += value;
        minI -= (maxI - minI) / 10;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &controls_col = right_col
                             .get<Row>("controls_row")
                             .get<Column>("controls_col");

    auto &control_row1 = controls_col.get<Row>("control_row1");
    auto &control_row2 = controls_col.get<Row>("control_row2");

    auto &up = control_row1.get<Label>("up");

    auto &left = control_row2.get<Label>("left");
    auto &down = control_row2.get<Label>("down");
    auto &right = control_row2.get<Label>("right");

    up.events["clicked"] = [&]()
    {
        ld value = ((maxI - minI) / 10) / 2;
        minI -= value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    down.events["clicked"] = [&]()
    {
        ld value = ((maxI - minI) / 10) / 2;
        minI += value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    left.events["clicked"] = [&]()
    {
        ld value = ((maxR - minR) / 10) / 2;
        minR -= value;
        maxR -= value;

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    right.events["clicked"] = [&]()
    {
        ld value = ((maxR - minR) / 10) / 2;
        minR += value;
        maxR += value;

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    app.dump("assets/mandelbrot.dump.json");

    return app();
}

#endif /* CCCB18B0_6929_4A1C_9604_57FB6EF1A893 */
