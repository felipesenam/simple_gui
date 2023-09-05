#ifndef CCCB18B0_6929_4A1C_9604_57FB6EF1A893
#define CCCB18B0_6929_4A1C_9604_57FB6EF1A893

#include <thread>

#include "score.hpp"
#include "scolors.hpp"
#include "sapplication.hpp"
#include "../widgets/sbitmap.hpp"
#include "../widgets/slabel.hpp"

#define MODIFIER 7
#define THREADS 8

using namespace PROJECT_NAMESPACE;
using namespace PROJECT_NAMESPACE::Colors;

double mapToReal(int x, int w, double minR, double maxR)
{
    return x * ((maxR - minR) / w) + minR;
}
double mapToImaginary(int y, int h, double minI, double maxI)
{
    return y * ((maxI - minI) / h) + minI;
}
int findMandelbrot(double cr, double ci, int maxN)
{
    int i = 0;
    double zr = 0., zi = 0.;
    while (i < maxN && zr * zr + zi * zi < 4.)
    {
        double tmp = zr * zr - zi * zi + cr;
        zi = 2. * zr * zi + ci;
        zr = tmp;
        i++;
    }
    return i;
}
void calc(Bitmap &bitmap, double minR, double maxR, double minI, double maxI, int maxN, int row, int column, size_t nElements)
{
    size_t elements = 0;
    for (size_t j = row; j < bitmap.height(); ++j)
    {
        for (size_t i = elements == 0 ? column : 0; i < bitmap.width(); ++i)
        {
            double cr = mapToReal(i, bitmap.width(), minR, maxR);
            double ci = mapToImaginary(j, bitmap.height(), minI, maxI);
            int n = findMandelbrot(cr, ci, maxN);

            int value = (n * MODIFIER) % 360;
            bitmap.at(i, j).hsl(value, n == 1 ? 0 : 1, n == 360 ? 0 : .5);

            elements++;
            if (elements == nElements)
                return;
        }
    }
}
void updateGrid(sgui::Bitmap &bitmap, double minR, double maxR, double minI, double maxI, int maxN = 360)
{
    std::vector<std::thread *> threads;
    const size_t tElements = bitmap.width() * bitmap.height();
    const size_t nElements = ceil(tElements / double(THREADS));
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

double getMaxI(double minR, double maxR, double minI, size_t width, size_t height)
{
    return minI + (maxR - minR) * height / width;
}

void updateLabels(double minR, double maxR, double minI, Label &minRbox, Label &maxRbox, Label &minIbox)
{
    minRbox.text = std::to_string(minR);
    maxRbox.text = std::to_string(maxR);
    minIbox.text = std::to_string(minI);
}

int mandelbrot()
{
    const std::string config_file("assets/mandelbrot.json");
    std::ifstream ifstream(config_file);
    json data = json::parse(ifstream);
    Application app = data.template get<Application>();

    app.dump(config_file);

    auto &window = app.windows.get("main");
    window.container.style.direction = vertical;
    window.container.style.verticalAlign = middle;
    window.container.style.horizontalAlign = center;

    // app.window.setIcon("assets/icon.png");

    const int width = 512;
    const int height = 512;

    auto &content = window.create<Row>();

    auto &right_col = window.create<Column>();
    auto &left_col = window.create<Column>();
    content.add(left_col, right_col);

    auto &bitmap = window.create<Bitmap>(width, height);
    bitmap.scheme.normal.background = Black;
    left_col.add(bitmap);

    double minR, maxR, minI;
    minR = -2;
    maxR = .8;
    minI = -1.4;

    double maxI = getMaxI(minR, maxR, minI, width, height);

    auto &values_row = window.create<Row>();
    auto &values_col = window.create<Column>();

    auto &minRbox = window.create<Label>();
    auto &maxRbox = window.create<Label>();
    auto &minIbox = window.create<Label>();

    values_col.add(minRbox, maxRbox, minIbox);
    values_row.add(values_col);
    right_col.add(values_row);

    updateGrid(bitmap, minR, maxR, minI, maxI);
    updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);

    auto &zoom_control_row = window.create<Row>();

    auto &zoomIn = window.create<Label>();
    zoomIn.text = "IN";
    zoomIn.events["clicked"] = [&]()
    {
        double deltaR = maxR - minR;
        if (deltaR < 0.0000000000001)
        {
            std::cout << "cannot be more acurate" << std::endl;
            // return;
        }
        double value = (deltaR / 10) / 2;

        minR += value;
        maxR -= value;
        minI += ((maxI - minI) / 10) / 2;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &zoomOut = window.create<Label>();
    zoomOut.text = "OUT";
    zoomOut.events["clicked"] = [&]()
    {
        double deltaR = maxR - minR;
        if (deltaR > 2)
            return;
        double value = (maxR - minR) / 10;
        minR -= value;
        maxR += value;
        minI -= (maxI - minI) / 10;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    zoom_control_row.add(zoomIn, zoomOut);
    right_col.add(zoom_control_row);

    auto &controls_row = window.create<Row>();
    controls_row.style.horizontalAlign = center;
    right_col.add(controls_row);

    auto &controls_col = window.create<Column>();
    controls_row.add(controls_col);

    auto &control_row1 = window.create<Row>();
    control_row1.style.horizontalAlign = center;
    controls_col.add(control_row1);

    auto &up = window.create<Label>();
    up.geometry.anchor = middle_center;
    up.text = "UP";
    up.events["clicked"] = [&]()
    {
        double value = ((maxI - minI) / 10) / 2;
        minI -= value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };
    control_row1.add(up);

    auto &control_row2 = window.create<Row>();
    control_row2.style.horizontalAlign = center;
    controls_col.add(control_row2);

    auto &down = window.create<Label>();
    down.geometry.anchor = middle_center;
    down.text = "DOWN";
    down.events["clicked"] = [&]()
    {
        double value = ((maxI - minI) / 10) / 2;
        minI += value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &left = window.create<Label>();
    left.geometry.anchor = middle_center;
    left.text = "LEFT";
    left.events["clicked"] = [&]()
    {
        double value = ((maxR - minR) / 10) / 2;
        minR -= value;
        maxR -= value;

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &right = window.create<Label>();
    right.geometry.anchor = middle_center;
    right.text = "RIGHT";
    right.events["clicked"] = [&]()
    {
        double value = ((maxR - minR) / 10) / 2;
        minR += value;
        maxR += value;

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    control_row2.add(left, down, right);

    return app();
}

#endif /* CCCB18B0_6929_4A1C_9604_57FB6EF1A893 */
