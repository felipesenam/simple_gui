#ifndef CCCB18B0_6929_4A1C_9604_57FB6EF1A893
#define CCCB18B0_6929_4A1C_9604_57FB6EF1A893

#include <thread>

#include "sapplication.hpp"
#include "swidgets.hpp"

#define MODIFIER 7
#define THREADS 16

using namespace sgui;
using namespace sgui::Colors;

using ld = long double;

int findMandelbrot(ld cr, ld ci, int maxN)
{
    int i = 0;
    ld zr = 0., zi = 0., tmp;
    while (i < maxN && zr * zr + zi * zi < 4.)
    {
        tmp = zr * zr - zi * zi + cr;
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
            const ld cr = i * ((maxR - minR) / bitmap.width()) + minR;
            const ld ci = j * ((maxI - minI) / bitmap.height()) + minI;
            const int n = findMandelbrot(cr, ci, maxN);

            int value = (int)((sin(n) + cos(n)) * 361) % 361;
            bitmap.at(i, j).hsl(value, n == 1 ? 0 : 1, n == 255 ? 0 : .5);

            elements++;
            if (elements == nElements)
                return;
        }
    }
}

void updateGrid(sgui::Bitmap &bitmap, ld minR, ld maxR, ld minI, ld maxI, int maxN = 255)
{
    std::vector<std::thread *> threads;
    const size_t tElements = bitmap.width() * bitmap.height();
    const size_t nElements = ceil(tElements / ld(THREADS));
    for (size_t i = 0; i < tElements; i += nElements)
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

    auto &bitmap = Object<Widget>::get<Bitmap>("bitmap");

    auto &minRbox = Object<Widget>::get<Label>("minRbox");
    auto &maxRbox = Object<Widget>::get<Label>("maxRbox");
    auto &minIbox = Object<Widget>::get<Label>("minIbox");

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

    auto &zoomIn = Object<Widget>::get<Label>("zoomIn");
    zoomIn.events["clicked"] = [&](Widget &)
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

    auto &zoomOut = Object<Widget>::get<Label>("zoomOut");
    zoomOut.events["clicked"] = [&](Widget &)
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

    auto &up = Object<Widget>::get<Label>("up");
    up.events["clicked"] = [&](Widget &)
    {
        ld value = ((maxI - minI) / 10) / 2;
        minI -= value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &down = Object<Widget>::get<Label>("down");
    down.events["clicked"] = [&](Widget &)
    {
        ld value = ((maxI - minI) / 10) / 2;
        minI += value;
        maxI = getMaxI(minR, maxR, minI, width, height);

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &left = Object<Widget>::get<Label>("left");
    left.events["clicked"] = [&](Widget &)
    {
        ld value = ((maxR - minR) / 10) / 2;
        minR -= value;
        maxR -= value;

        updateGrid(bitmap, minR, maxR, minI, maxI);
        updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
    };

    auto &right = Object<Widget>::get<Label>("right");
    right.events["clicked"] = [&](Widget &)
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
