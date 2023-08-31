#ifndef BC9E3D34_49BA_4521_B928_1D231B46998F
#define BC9E3D34_49BA_4521_B928_1D231B46998F

#include "score.hpp"
#include "scolor.hpp"

namespace PROJECT_NAMESPACE
{
    class ColorGrid
    {
    protected:
        SDL_Surface *surface = nullptr;
        Color **data = nullptr;
        size_t w = 0, h = 0;

        void updateSurface();

        ColorGrid();

        void init();

    public:
        ColorGrid(size_t w, size_t h);
        ~ColorGrid();

        virtual Color &at(size_t x, size_t y);
        virtual const Color &get(size_t x, size_t y) const;

        virtual void alloc(size_t width, size_t height);
        virtual void erase();

        size_t width() const noexcept;
        size_t height() const noexcept;

        void savePPMA(const std::string &file);
        void savePPMB(const std::string &file);
        void saveJPG(const std::string &file);
        void savePNG(const std::string &file);
        void saveBMP(const std::string &file);
    };
}

#endif /* BC9E3D34_49BA_4521_B928_1D231B46998F */
