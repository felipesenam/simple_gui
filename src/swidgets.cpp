#include "swidgets.hpp"

#include "swindow.hpp"
#include "sscheme.hpp"

namespace PROJECT_NAMESPACE
{
    Bitmap::Bitmap(Window &window) : Widget(window)
    {
    }
    Bitmap::Bitmap(Window &window, size_t w, size_t h) : Widget(window)
    {
        alloc(w, h);
    }

    Bitmap::~Bitmap()
    {
        erase();
    }

    Color &Bitmap::at(size_t x, size_t y)
    {
        m_render = true;
        return data[y][x];
    }

    void Bitmap::alloc(size_t width, size_t height)
    {
        erase();
        geometry.abs.w = this->w = width;
        geometry.abs.h = this->h = height;

        data = new Color *[height];
        for (size_t i = 0; i < height; ++i)
            data[i] = new Color[width];
        surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        if (surface == nullptr)
        {
            SDL_PrintError(Error);
        }
        m_render = true;
    }

    void Bitmap::erase()
    {
        if (w == 0 || h == 0)
            return;

        SDL_FreeSurface(surface);
        if (tex != nullptr)
            SDL_DestroyTexture(tex);
        SDL_PrintIfError(Warn);

        geometry.abs.w = w = 0;
        geometry.abs.h = h = 0;

        data = nullptr;
    }

    void Bitmap::render()
    {
        if (m_render)
        {
            if (tex != NULL)
                SDL_DestroyTexture(tex);
            SDL_PrintIfError(Warn);

            updateSurface();
            tex = window.renderer.createTextureFromSurface(surface);

            m_render = false;
            this->geometry.normalize();
        }
    }

    void Bitmap::draw()
    {
        window.renderer.drawTexture(tex, &geometry.src, &geometry.dest);
    }
    void to_json(json &j, const Bitmap &bitmap)
    {
        j["type"] = demangle(typeid(Bitmap).name());
        j["width"] = bitmap.w;
        j["height"] = bitmap.h;
    }
    void from_json(const json &j, Bitmap &bitmap)
    {
        bitmap.alloc(j["width"], j["height"]);
    }

    Flex::Flex(Window &window) : WidgetManager(window)
    {
    }

    void Flex::posWidgetHorizontal(int &lx, int &ly, int &currentWidth, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        switch (style.horizontalAlign)
        {
        case left:
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            break;
        case center:
            lx = currentWidth + geometry.dest.x + (geometry.dest.w / 2) - (dimensions.width / 2);
            widget.geometry.dest.x = lx + widget.geometry.padding.left - widget.geometry.padding.right;
            break;
        case right:
            lx = currentWidth + geometry.dest.x + geometry.dest.w - dimensions.width;
            widget.geometry.dest.x = lx + widget.geometry.margin.right + widget.geometry.padding.right;
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case middle:
            ly = geometry.dest.y + (geometry.dest.h / 2) - (widget.geometry.dest.h / 2);
            widget.geometry.dest.y = ly + widget.geometry.padding.top - widget.geometry.padding.bottom;
            break;
        case bottom:
            ly = geometry.dest.y + geometry.dest.h - widget.geometry.dest.h;
            widget.geometry.dest.y = ly - (widget.geometry.margin.bottom + widget.geometry.padding.bottom);
            break;
        }
        switch (style.justifyContent)
        {
        case between:
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            currentWidth += spaceBetween;
            break;
        case around:
            currentWidth += spaceAround;
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            currentWidth += spaceAround;
            break;
        default:
            break;
        }
    }
    void Flex::posWidgetVertical(int &lx, int &ly, int &currentHeight, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        switch (style.horizontalAlign)
        {
        case left:
            lx = geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + widget.geometry.padding.left;
            break;
        case center:
            lx = geometry.dest.x + (geometry.dest.w / 2) - (widget.geometry.dest.w / 2);
            widget.geometry.dest.x = lx + widget.geometry.padding.left - widget.geometry.padding.right;
            break;
        case right:
            lx = geometry.dest.x + geometry.dest.w - widget.geometry.dest.w;
            widget.geometry.dest.x = lx - (widget.geometry.margin.right + widget.geometry.padding.right);
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case middle:
            ly = currentHeight + geometry.dest.y + (geometry.dest.h / 2) - (dimensions.height / 2);
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        case bottom:
            ly = currentHeight + geometry.dest.y + geometry.dest.h - dimensions.height;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            break;
        }
        switch (style.justifyContent)
        {
        case between:
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            currentHeight += spaceBetween;
            break;
        case around:
            currentHeight += spaceAround;
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + widget.geometry.padding.top;
            currentHeight += spaceAround;
            break;
        default:
            break;
        }
    }

    void Flex::getDimensions()
    {
        switch (this->style.direction)
        {
        case horizontal:
            this->dimensions = query_content_horizontal();
            break;
        case vertical:
            this->dimensions = query_content_vertical();
            break;
        }
    }

    void Flex::update()
    {
        events.perform();

        int lx = 0, ly = 0;
        getDimensions();
        if (style.direction == vertical)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.h - dimensions.height) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.h - dimensions.height) / (widgets.size() - 1) : 0;

            int currentHeight = 0;
            for (auto &pair : widgets)
            {
                auto widget = pair.second;

                posWidgetVertical(lx, ly, currentHeight, *widget, spaceBetween, spaceAround);

                widget->update();

                const int my = widget->geometry.margin.y();
                const int py = widget->geometry.padding.y();
                currentHeight += widget->geometry.dest.h + my + py;
            }
        }
        else if (style.direction == horizontal)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.w - dimensions.width) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.w - dimensions.width) / (widgets.size() - 1) : 0;

            int currentWidth = 0;
            for (auto &pair : widgets)
            {
                auto widget = pair.second;

                posWidgetHorizontal(lx, ly, currentWidth, *widget, spaceBetween, spaceAround);

                widget->update();

                const int mx = widget->geometry.margin.x();
                const int px = widget->geometry.padding.x();
                currentWidth += widget->geometry.dest.w + mx + px;
            }
        }
    }
    void to_json(json &j, const Flex &p)
    {
        j["type"] = demangle(typeid(Flex).name());
        j["style"] = p.style;
        j["widgets"] = p.widgets;
    }
    void from_json(const json &j, Flex &p)
    {
        SETATTR_IF_JSON_CONTAINS(j, p, style);
        from_json(j["widgets"], &p);
    }

    Column::Column(Window &window) : Flex(window)
    {
        style.direction = vertical;
        style.horizontalAlign = center;
        style.verticalAlign = top;
    }

    void Column::render()
    {
        for (auto pair : widgets)
            pair.second->render();

        this->dimensions = query_content_vertical();
        this->geometry.dest.w = dimensions.width + this->geometry.padding.x();
        this->geometry.dest.h = dimensions.height + this->geometry.padding.y();
    }
    void to_json(json &j, const Column &col)
    {
        j["type"] = demangle(typeid(Column).name());
        j["size"] = col.size;
        j["widgets"] = col.widgets;
    }
    void from_json(const json &j, Column &col)
    {
        from_json(j["widgets"], &col);
    }

    Row::Row(Window &window) : Flex(window)
    {
        style.direction = horizontal;
        style.horizontalAlign = center;
        style.verticalAlign = middle;
    }

    void Row::render()
    {
        for (auto pair : widgets)
        {
            auto widget = pair.second;

            widget->render();

            Column *column = dynamic_cast<Column *>(widget);
            if (column && column->size >= 1)
            {
                column->geometry.dest.w = (column->size / float(this->size)) * this->geometry.dest.w;
            }
        }

        dimensions = query_content_horizontal();
        this->geometry.dest.w = dimensions.width + this->geometry.padding.x();
        this->geometry.dest.h = dimensions.height + this->geometry.padding.y();
    }
    void to_json(json &j, const Row &row)
    {
        j["type"] = demangle(typeid(Row).name());
        j["size"] = row.size;
        j["widgets"] = row.widgets;
    }
    void from_json(const json &j, Row &row)
    {
        from_json(j["widgets"], &row);
    }

    Label::Label(Window &window) : Widget(window)
    {
        this->scheme = UI_LABEL_COLOR_SCHEME;
    }
    Label::~Label()
    {
        if (textTexture)
        {
            SDL_DestroyTexture(textTexture);
        }
    }

    void Label::render()
    {
        if (text != renderedText)
        {
            const WidgetColorScheme::ColorScheme *currentScheme = &scheme.normal;
            if (textTexture != nullptr)
            {
                SDL_DestroyTexture(textTexture);
            }
            SDL_PrintIfError(Warn);

            textTexture = this->window.renderer.renderText(text, font, this->geometry, currentScheme->background);
            renderedText = text;
            this->geometry.normalize();
        }
    }

    void Label::update()
    {
        events.perform();

        auto &scheme = getCurrentColorScheme();
        if (textTexture != nullptr)
        {
            if (SDL_SetTextureColorMod(textTexture, RGB(scheme.color)) != 0)
            {
                Warn(SDL_GetError());
            }
        }
    }

    void Label::draw()
    {
        this->window.renderer.drawTexture(textTexture, &this->geometry.src, &this->geometry.dest);
    }

    WidgetManager::WidgetManager(Window &window) : Widget(window)
    {
        scheme = UI_TRANSPARENT_COLOR_SCHEME;
    }

    WidgetManager::~WidgetManager()
    {
        for (auto pair : widgets)
            delete pair.second;
    }

    void WidgetManager::handleEvent(const SDL_Event &e)
    {
        for (auto pair : widgets)
        {
            auto widget = pair.second;

            widget->handleGenericEvents(e);
            widget->handleEvent(e);
        }
    }
    void WidgetManager::render()
    {
        for (auto pair : widgets)
        {
            auto widget = pair.second;

            widget->render();
        }
    }
    void WidgetManager::update()
    {
        for (auto pair : widgets)
        {
            auto widget = pair.second;

            widget->update();
        }
    }
    void WidgetManager::draw()
    {
        for (auto pair : widgets)
        {
            auto widget = pair.second;

            widget->drawCommonElements();
            widget->draw();
        }
    }

    template <>
    const int Object<Widget>::err = std::atexit(Object<Widget>::atexit_handler);

    Widget::Widget(Window &window) : window(window), geometry(*this), font(window.config.defaultFontPath, window.config.defaultFontSize)
    {
        // if (window.container && window.container != this)
        // {
        //     window.container->add(*this);
        // }
    }

    void Widget::handleGenericEvents(const SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_WINDOWEVENT:
        {
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                events["windowSizeChanged"].triggered = true;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                events["windowResized"].triggered = true;
                break;
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            SDL_Point mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            bool isInside = SDL_PointInRect(&mousePos, &geometry.dest);
            if (isInside)
            {
                if (!m_isHovered)
                {
                    Debug("Hovering " << *this);
                    events["hover"].triggered = m_isHovered = true;
                }
            }
            else
            {
                m_isHovered = false;
            }

            events["hovering"].triggered = isInside;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            events["mousedown"].triggered = m_isPressed = m_isHovered;
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            events["mouseup"].triggered;
            if (m_isPressed && m_isHovered)
            {
                events["clicked"].triggered = true;
            }
            break;
        }
        }
    }

    void Widget::drawCommonElements()
    {
        auto scheme = this->getCurrentColorScheme();
        window.renderer.drawFillRectangle(geometry.dest, scheme.background);
        window.renderer.drawRectangle(geometry.dest, scheme.border);

#ifdef DEBUG
        if (this->m_isHovered)
        {
            auto marginbox = geometry.dest;
            marginbox.x -= geometry.margin.left;
            marginbox.y -= geometry.margin.top;
            marginbox.w += geometry.margin.x();
            marginbox.h += geometry.margin.y();

            window.renderer.drawRectangle(marginbox, Colors::Yellow);

            auto paddingbox = geometry.dest;
            paddingbox.x += geometry.padding.left;
            paddingbox.y += geometry.padding.top;
            paddingbox.w -= geometry.padding.x();
            paddingbox.h -= geometry.padding.y();

            window.renderer.drawRectangle(paddingbox, Colors::Blue);
        }
        window.renderer.drawRectangle(geometry.dest, Colors::Red);
#endif
    }

    void Widget::handleEvent(const SDL_Event &)
    {
    }
    void Widget::render()
    {
    }
    void Widget::update()
    {
        events.perform();
    }
    void Widget::draw()
    {
    }

    void to_json(json &j, const std::map<std::string, Widget *> &p)
    {
        for (auto &widget : p)
        {
            if (emplace<Label>(j, widget))
                continue;
            if (emplace<Column>(j, widget))
                continue;
            if (emplace<Row>(j, widget))
                continue;
            if (emplace<Flex>(j, widget))
                continue;
            if (emplace<Bitmap>(j, widget))
                continue;
        }
    }
    void from_json(const json &j, WidgetManager *man)
    {
        for (auto widget : j.items())
        {
            const std::string uid(widget.key());
            json value = widget.value();

            if (get_from<Label>(value, uid, man))
                continue;
            if (get_from<Column>(value, uid, man))
                continue;
            if (get_from<Row>(value, uid, man))
                continue;
            if (get_from<Flex>(value, uid, man))
                continue;
            if (get_from<Bitmap>(value, uid, man))
                continue;
        }
    }
}