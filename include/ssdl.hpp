#ifndef CDCA1879_BAE4_44C1_A123_791BDAB0826F
#define CDCA1879_BAE4_44C1_A123_791BDAB0826F

#include "score.hpp"

namespace sgui
{
    class Surface;
    class Texture;

    class Color : public SDL_Color
    {
    public:
        Color();
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        Color(unsigned hex, Uint8 a = 255);
        Color(const Color &color);

        Color &hsl(float h, float s, float l, Uint8 a = 255);
        Color &hsv(float h, float s, float v, Uint8 a = 255);
        Color &rgb(float r, float g, float b, Uint8 a = 255);
        Color &hex(unsigned hex);

        Color &operator=(unsigned hex);
        Color &operator=(const Color &color);

        friend std::ostream &operator<<(std::ostream &o, const Color &color)
        {
            o << "RGBA(" << static_cast<int>(color.r) << ", "
              << static_cast<int>(color.g) << ", "
              << static_cast<int>(color.b) << ", "
              << static_cast<int>(color.a) << ")";
            return o;
        }

        friend void to_json(json &j, const Color &color)
        {
            j = json::array({color.r, color.g, color.b, color.a});
        }
        friend void from_json(const json &j, Color &color)
        {
            if (j.is_array())
            {
                if (j.size() >= 3)
                {
                    color.r = j[0];
                    color.g = j[1];
                    color.b = j[2];
                }
                if (j.size() >= 4)
                {
                    color.a = j[3];
                }
            }
        }
    };

    class Rect : public SDL_Rect
    {
    public:
        Rect() : SDL_Rect({0, 0, 0, 0}) {}
        Rect(int x, int y, int w, int h) : SDL_Rect({x, y, w, h}) {}

        bool mouseIn() const
        {
            SDL_Point mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            return SDL_PointInRect(&mousePos, this);
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Rect, x, y, w, h);

        bool operator==(const Rect &rect)
        {
            return this->x == rect.x && this->y == rect.y && this->w == rect.w && this->h == rect.h;
        }

        friend std::ostream &operator<<(std::ostream &o, const Rect &rect)
        {
            o << "{" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << "}";
            return o;
        }

        friend Rect operator*(const Rect &rect, float scale)
        {
            return {static_cast<int>(rect.x * scale),
                    static_cast<int>(rect.y * scale),
                    static_cast<int>(rect.w * scale),
                    static_cast<int>(rect.h * scale)};
        }
    };

    class Font
    {
    private:
        TTF_Font *ttf = nullptr;

        friend class Renderer;

    public:
        Font(const std::string &path, unsigned size = 12);
        ~Font();

        enum RenderType
        {
            blended,
            solid,
            shaded
        };
        RenderType renderType = blended;
        Color background;

        void open(const std::string &path, unsigned size);

        Surface *render(const std::string &text, unsigned wrapLenght = 0);
    };

    struct WindowConfig
    {
        struct Options
        {
            // SDL_WINDOW_FULLSCREEN
            bool isFullscreen = false;

            // SDL_WINDOW_SHOWN
            bool isShown = true;

            // SDL_WINDOW_HIDDEN
            bool isHidden = false;

            // SDL_WINDOW_BORDERLESS
            bool isBorderless = false;

            // SDL_WINDOW_RESIZABLE
            bool isResizable = false;

            // SDL_WINDOW_MINIMIZED
            bool isMinimized = false;

            // SDL_WINDOW_MAXIMIZED
            bool isMaximized = false;

            // SDL_WINDOW_FULLSCREEN_DESKTOP
            bool isFullscreenDesktop = false;

            // SDL_WINDOW_ALWAYS_ON_TOP
            bool isAlwaysOnTop = false;

            // SDL_WINDOW_SKIP_TASKBAR
            bool skipsTaskbar = false;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Options,
                                                        isFullscreen,
                                                        isShown,
                                                        isHidden,
                                                        isBorderless,
                                                        isResizable,
                                                        isMinimized,
                                                        isMaximized,
                                                        isFullscreenDesktop,
                                                        isAlwaysOnTop,
                                                        skipsTaskbar);

            Uint32 get() const noexcept
            {
                return (
                    (this->isFullscreen ? SDL_WINDOW_FULLSCREEN : 0) |
                    (this->isHidden ? SDL_WINDOW_HIDDEN : 0) |
                    (this->isShown ? SDL_WINDOW_SHOWN : 0) |
                    (this->isBorderless ? SDL_WINDOW_BORDERLESS : 0) |
                    (this->isResizable ? SDL_WINDOW_RESIZABLE : 0) |
                    (this->isMinimized ? SDL_WINDOW_MINIMIZED : 0) |
                    (this->isMaximized ? SDL_WINDOW_MAXIMIZED : 0) |
                    (this->isFullscreenDesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) |
                    (this->isAlwaysOnTop ? SDL_WINDOW_ALWAYS_ON_TOP : 0) |
                    (this->skipsTaskbar ? SDL_WINDOW_SKIP_TASKBAR : 0));
            }
        };

        std::string title = "Window";
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        int width = 600;
        int height = 400;

        /**
         * @brief Window options. Note this alter this options after after window initialized has no effect.
         *
         */
        Options options;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WindowConfig, title, x, y, width, height, options);
    };

    class Window
    {
    private:
        int id;

    public:
        Window(const WindowConfig &config)
        {
            window = SDL_CreateWindow(
                config.title.c_str(),
                config.x,
                config.y,
                config.width,
                config.height,
                config.options.get());
        }
        ~Window()
        {
            SDL_DestroyWindow(window);
        }

        SDL_Window *window = nullptr;

        /**
         * @brief
         *
         * @return std::pair<int, int> Which represents respectively window width and height.
         */
        std::pair<int, int> size() const
        {
            std::pair<int, int> _size;
            SDL_GetWindowSize(window, &_size.first, &_size.second);
            return _size;
        }

        unsigned getId() const
        {
            return SDL_GetWindowID(window);
        }

        void hide()
        {
            SDL_HideWindow(window);
        }
        void show()
        {
            SDL_ShowWindow(window);
        }
    };

    struct RendererConfig
    {
        struct Options
        {
            // SDL_RENDERER_ACCELERATED
            bool usesHardwareAcceleration = false;

            // SDL_RENDERER_PRESENTVSYNC
            bool usesVSync = true;

            Uint32 get() const noexcept
            {
                return (
                    (this->usesHardwareAcceleration ? SDL_RENDERER_ACCELERATED : 0) |
                    (this->usesVSync ? SDL_RENDERER_PRESENTVSYNC : 0));
            }

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Options, usesHardwareAcceleration, usesVSync);
        };

        int index = -1;
        Color drawColor;

        Options options;

        enum BlendMode
        {
            none = SDL_BLENDMODE_NONE,
            alpha = SDL_BLENDMODE_BLEND,
            additive = SDL_BLENDMODE_ADD,
            modulate = SDL_BLENDMODE_MOD,
            multiply = SDL_BLENDMODE_MUL
        };
        BlendMode blendMode = alpha;
        NLOHMANN_JSON_SERIALIZE_ENUM(BlendMode, {
                                                    {BlendMode::none, "none"},
                                                    {BlendMode::alpha, "alpha"},
                                                    {BlendMode::additive, "additive"},
                                                    {BlendMode::modulate, "modulate"},
                                                    {BlendMode::multiply, "multiply"},
                                                })

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(RendererConfig, index, drawColor, options, blendMode);
    };

    class Renderer
    {
    private:
    public:
        Renderer(Window &, const RendererConfig &);
        ~Renderer();

        SDL_Renderer *renderer = nullptr;

        Color drawColor;

        bool isCreated() const noexcept;

        void create(Window &, const RendererConfig &);
        void destroy();

        void clear();
        void present();

        Texture *createTextureFromSurface(Surface &surface);
        void drawTexture(Texture &texture, Rect *src, Rect *dest);
        void drawRectangle(const Rect &dest, const Color &color);
        void drawCross(const Rect &dest, const Color &color);
        void drawFillRectangle(const Rect &dest, const Color &color);
        Texture *renderText(const std::string &text, Font &font, int &width, int &height, Uint32 wrapLenght = 0);
    };

    class Texture
    {
    private:
        SDL_Texture *texture = nullptr;
        int m_width, m_height;

        friend Renderer;

    public:
        Texture(Renderer &renderer, Surface &surface);
        ~Texture();

        int width() const noexcept
        {
            return m_width;
        }

        int height() const noexcept
        {
            return m_height;
        }

        void modColor(const Color &color);
        void query(Uint32 *format, int *access, int *w, int *h);
    };

    class Surface
    {
    private:
        size_t m_width, m_height;

        bool mIsDirty = true;

        friend class Bitmap;

    public:
        Surface(const std::string &file);
        Surface(SDL_Surface *surface);
        Surface(int width, int height);
        ~Surface();

        SDL_Surface *surface = nullptr;

        void toJPG(const std::string &file, int quality = 95) const;
        void toPNG(const std::string &file) const;
        void toBMP(const std::string &file) const;
        void toPPMA(const std::string &file) const;
        void toPPMB(const std::string &file) const;

        struct Pixel
        {
            Uint8 r;
            Uint8 g;
            Uint8 b;
            Uint8 a;
        };

        Pixel get(size_t x, size_t y) const
        {
            const int bpp = surface->format->BytesPerPixel;
            unsigned char *pixels = (unsigned char *)surface->pixels;
            Uint8 *p = (Uint8 *)pixels + (y * surface->pitch) + (x * bpp);

            switch (bpp)
            {
            case 1:
                return {p[0], p[0], p[0], 255};
            case 2:
                return {p[0], p[1], p[1], 255};
            case 3:
                return {p[0], p[1], p[2], 255};
            case 4:
                return {p[0], p[1], p[2], p[3]};
            default:
                return {0, 0, 0, 0};
            }
        }

        void set(int x, int y, const Pixel &pixel)
        {
            Uint32 *pixels = (Uint32 *)surface->pixels;
            const int index = y * (surface->pitch / surface->format->BytesPerPixel) + x;

            pixels[index] = SDL_MapRGBA(surface->format, RGBA(pixel));

            mIsDirty = true;
        }

        size_t width() const noexcept
        {
            return m_width;
        }

        size_t height() const noexcept
        {
            return m_height;
        }
    };
}

#endif /* CDCA1879_BAE4_44C1_A123_791BDAB0826F */
