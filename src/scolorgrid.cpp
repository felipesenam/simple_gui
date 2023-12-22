
#include "scolorgrid.hpp"

#include <fstream>

namespace sgui
{
    void ColorGrid::updateSurface()
    {
        size_t i;
        unsigned char *pixels = (unsigned char *)surface->pixels;
        for (size_t y = 0; y < h; ++y)
        {
            for (size_t x = 0; x < w; ++x)
            {
                i = 4 * (y * surface->w + x);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixels[i + 0] = 255;
                pixels[i + 1] = data[y][x].r;
                pixels[i + 2] = data[y][x].g;
                pixels[i + 3] = data[y][x].b;
#else
                pixels[i + 3] = 255;
                pixels[i + 2] = data[y][x].r;
                pixels[i + 1] = data[y][x].g;
                pixels[i + 0] = data[y][x].b;
#endif
            }
        }
    }

    ColorGrid::ColorGrid()
    {
    }

    void ColorGrid::init()
    {
    }
    ColorGrid::ColorGrid(size_t w, size_t h)
    {
        alloc(w, h);
    }
    ColorGrid::~ColorGrid()
    {
        erase();
    }
    Color &ColorGrid::at(size_t x, size_t y)
    {
        return data[y][x];
    }
    const Color &ColorGrid::get(size_t x, size_t y) const
    {
        return data[y][x];
    }

    void ColorGrid::alloc(size_t width, size_t height)
    {
        erase();
        w = width;
        h = height;

        data = new Color *[height];
        for (size_t i = 0; i < height; ++i)
            data[i] = new Color[width];
        surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
    }

    void ColorGrid::erase()
    {
        if (w == 0 || h == 0)
            return;

        for (size_t i = 0; i < h; ++i)
            delete[] data[i];
        delete[] data;
        SDL_FreeSurface(surface);

        w = 0;
        h = 0;

        data = nullptr;
    }

    size_t ColorGrid::width() const noexcept
    {
        return w;
    }
    size_t ColorGrid::height() const noexcept
    {
        return h;
    }

    void ColorGrid::savePPMA(const std::string &file)
    {
        std::ofstream save;
        save.open(file);

        save << "P3\r";
        save << w << " " << h << "\r";
        save << "255\r";

        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < w; ++j)
                save << data[i][j].r << " " << data[i][j].g << " " << data[i][j].b << "\r";

        save.close();
    }
    void ColorGrid::savePPMB(const std::string &file)
    {
        std::ofstream save;
        save.open(file, std::ios_base::binary);

        save << "P6\r";
        save << w << " " << h << "\r";
        save << "255\r";

        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < w; ++j)
                save << (unsigned char)data[i][j].r << (unsigned char)data[i][j].g << (unsigned char)data[i][j].b;

        save.close();
    }
    void ColorGrid::saveJPG(const std::string &file)
    {
        updateSurface();
        if (IMG_SaveJPG(surface, file.c_str(), 100) < 0)
        {
            SDL_PrintError(Error);
        }
    }
    void ColorGrid::savePNG(const std::string &file)
    {
        updateSurface();
        if (IMG_SavePNG(surface, file.c_str()) < 0)
        {
            SDL_PrintError(Error);
        }
    }
    void ColorGrid::saveBMP(const std::string &file)
    {
        updateSurface();
        if (SDL_SaveBMP(surface, file.c_str()) < 0)
        {
            SDL_PrintError(Error);
        }
    }
}