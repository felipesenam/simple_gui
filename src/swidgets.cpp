#include "swidgets.hpp"

#include "swindow.hpp"
#include "sscheme.hpp"
namespace sgui
{
    void Geometry::normalize()
    {
        switch (behavior)
        {
        case hug:
            dest = abs * aspect;
            src = abs;
            break;

        case normal:
            dest = abs * aspect;
            src = abs;
            break;
        default:
            break;
        }
    }

    Bitmap::Bitmap(ApplicationWindow &window, size_t w, size_t h) : Bitmap(window)
    {
        alloc(w, h);
    }
    Bitmap::Bitmap(ApplicationWindow &window) : Widget(window)
    {
        scheme = UI_TRANSPARENT_COLOR_SCHEME;
    }
    Bitmap::~Bitmap()
    {
    }

    void Bitmap::erase()
    {
        if (surface)
        {
            delete surface;
            surface = nullptr;
        }
        if (m_texture)
        {
            delete m_texture;
            m_texture = nullptr;
        }

        geometry.abs.w = 0;
        geometry.abs.h = 0;
    }

    void Bitmap::alloc(size_t width, size_t height)
    {
        erase();
        geometry.abs.w = width;
        geometry.abs.h = height;

        surface = new Surface(width, height);

        m_render = true;
    }

    void Bitmap::open(const std::string &file)
    {
        this->erase();

        surface = new Surface(file);
        this->geometry.abs.w = surface->width();
        this->geometry.abs.h = surface->height();
    }

    void Bitmap::render()
    {
        if (!surface->mIsDirty)
            return;

        surface->mIsDirty = false;

        if (m_texture != nullptr)
            delete m_texture;

        m_texture = window.renderer->createTextureFromSurface(*surface);

        this->geometry.normalize();
    }

    void Bitmap::draw()
    {
        this->geometry.normalize();
        window.renderer->drawTexture(*m_texture, &geometry.src, &geometry.dest);
    }
    void to_json(json &j, const Bitmap &bitmap)
    {
        j["uid"] = bitmap.uid;
        j["scheme"] = bitmap.scheme;

        j["type"] = demangle(typeid(Bitmap).name());
    }
    void from_json(const json &j, Bitmap &bitmap)
    {
        SETATTR_IF_JSON_CONTAINS(j, bitmap, uid);
        SETATTR_IF_JSON_CONTAINS(j, bitmap, scheme);

        bitmap.alloc(j["width"], j["height"]);
    }

    Flex::Flex(ApplicationWindow &window) : WidgetManager(window)
    {
    }

    void Flex::posWidgetHorizontal(int &lx, int &ly, int &currentWidth, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        const int cw = contentWidth();

        switch (style.horizontalAlign)
        {
        case left:
            lx = currentWidth + geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + geometry.padding.left;
            break;
        case center:
            lx = currentWidth + geometry.dest.x + ((geometry.dest.w + geometry.padding.dx() - cw) / 2);
            widget.geometry.dest.x = lx + widget.geometry.margin.left;
            break;
        case right:
            lx = currentWidth + geometry.dest.x + geometry.dest.w - cw;
            widget.geometry.dest.x = lx - geometry.padding.right + widget.geometry.margin.left;
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + geometry.padding.top;
            break;
        case middle:
            ly = geometry.dest.y + ((geometry.dest.h - widget.geometry.dest.h + widget.geometry.margin.dy() + geometry.padding.dy()) / 2);
            widget.geometry.dest.y = ly;
            break;
        case bottom:
            ly = geometry.dest.y + geometry.dest.h - widget.geometry.dest.h;
            widget.geometry.dest.y = ly - widget.geometry.margin.bottom - geometry.padding.bottom;
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
        const int ch = contentHeight();

        switch (style.horizontalAlign)
        {
        case left:
            lx = geometry.dest.x;
            widget.geometry.dest.x = lx + widget.geometry.margin.left + geometry.padding.left;
            break;
        case center:
            lx = geometry.dest.x + (((geometry.dest.w + geometry.padding.dx()) + widget.geometry.margin.dx() - widget.geometry.dest.w) / 2);
            widget.geometry.dest.x = lx;
            break;
        case right:
            lx = geometry.dest.x + geometry.dest.w - widget.geometry.dest.w;
            widget.geometry.dest.x = lx - widget.geometry.margin.right - geometry.padding.right;
            break;
        }
        switch (style.verticalAlign)
        {
        case top:
            ly = currentHeight + geometry.dest.y;
            widget.geometry.dest.y = ly + widget.geometry.margin.top + geometry.padding.top;
            break;
        case middle:
            ly = currentHeight + geometry.dest.y + ((geometry.dest.h + geometry.padding.dy() - ch) / 2);
            widget.geometry.dest.y = ly;
            break;
        case bottom:
            ly = currentHeight + geometry.dest.y + geometry.dest.h - ch;
            widget.geometry.dest.y = ly - widget.geometry.margin.bottom - geometry.padding.bottom;
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

    void Flex::update()
    {
        events.perform();

        int lx = 0, ly = 0;

        const int w = width();
        const int h = height();

        if (style.direction == vertical)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.h - h) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.h - h) / (widgets.size() - 1) : 0;

            int currentHeight = 0;
            for (auto widget : widgets)
            {
                posWidgetVertical(lx, ly, currentHeight, *widget, spaceBetween, spaceAround);

                widget->update();

                currentHeight += widget->height() + gap;
            }
        }
        else if (style.direction == horizontal)
        {
            const int spaceAround = widgets.size() ? (geometry.dest.w - w) / (widgets.size() * 2) : 0;
            const int spaceBetween = widgets.size() > 1 ? (geometry.dest.w - w) / (widgets.size() - 1) : 0;

            int currentWidth = 0;
            for (auto widget : widgets)
            {
                posWidgetHorizontal(lx, ly, currentWidth, *widget, spaceBetween, spaceAround);

                widget->update();

                currentWidth += widget->width() + gap;
            }
        }
    }
    void to_json(json &j, const Flex &p)
    {
        j["uid"] = p.uid;
        j["style"] = p.style;
        j["scheme"] = p.scheme;
        j["widgets"] = p.widgets;
        j["gap"] = p.gap;

        j["type"] = demangle(typeid(Flex).name());
    }
    void from_json(const json &j, Flex &p)
    {
        SETATTR_IF_JSON_CONTAINS(j, p, uid);
        SETATTR_IF_JSON_CONTAINS(j, p, style);
        SETATTR_IF_JSON_CONTAINS(j, p, scheme);
        SETATTR_IF_JSON_CONTAINS(j, p, gap);

        from_json(j["widgets"], &p);
    }

    Column::Column(ApplicationWindow &window) : Flex(window)
    {
        style.direction = vertical;
        style.horizontalAlign = center;
        style.verticalAlign = top;
    }

    void Column::render()
    {
        for (auto widget : widgets)
            widget->render();

        this->geometry.dest.w = contentWidth() + geometry.padding.x();
        this->geometry.dest.h = contentHeight() + geometry.padding.y();
    }
    void to_json(json &j, const Column &col)
    {
        j["uid"] = col.uid;
        j["size"] = col.size;
        j["scheme"] = col.scheme;
        j["widgets"] = col.widgets;

        j["type"] = demangle(typeid(Column).name());
    }
    void from_json(const json &j, Column &col)
    {
        SETATTR_IF_JSON_CONTAINS(j, col, uid);
        SETATTR_IF_JSON_CONTAINS(j, col, scheme);

        from_json(j["widgets"], &col);
    }

    Row::Row(ApplicationWindow &window) : Flex(window)
    {
        style.direction = horizontal;
        style.horizontalAlign = center;
        style.verticalAlign = middle;
    }

    void Row::render()
    {
        for (auto widget : widgets)
        {
            widget->render();

            Column *column = dynamic_cast<Column *>(widget);
            if (column && column->size >= 1)
            {
                column->geometry.dest.w = (column->size / float(this->size)) * this->geometry.dest.w;
            }
        }

        this->geometry.dest.w = contentWidth() + geometry.padding.x();
        this->geometry.dest.h = contentHeight() + geometry.padding.y();
    }
    void to_json(json &j, const Row &row)
    {
        j["uid"] = row.uid;
        j["size"] = row.size;
        j["scheme"] = row.scheme;
        j["widgets"] = row.widgets;

        j["type"] = demangle(typeid(Row).name());
    }
    void from_json(const json &j, Row &row)
    {
        SETATTR_IF_JSON_CONTAINS(j, row, uid);
        SETATTR_IF_JSON_CONTAINS(j, row, scheme);

        from_json(j["widgets"], &row);
    }

    Label::Label(ApplicationWindow &window) : Widget(window)
    {
        this->scheme = UI_LABEL_COLOR_SCHEME;
    }
    Label::Label(ApplicationWindow &window, const std::string &text) : Label(window)
    {
        this->text = text;
    }

    Label::~Label()
    {
        if (textTexture)
            delete textTexture;
    }

    void Label::render()
    {
        if (text != renderedText)
        {
            if (textTexture != nullptr)
                delete textTexture;

            textTexture = this->window.renderer->renderText(text, font, this->geometry.abs);
            renderedText = text;
            this->geometry.normalize();
        }
    }

    void Label::update()
    {
        events.perform();

        auto &scheme = getCurrentColorScheme();
        textTexture->modColor(scheme.color);
    }

    void Label::draw()
    {
        if (this->texture != nullptr)
        {
            Rect destR = this->geometry.dest;
            texture->query(NULL, NULL, &destR.w, &destR.h);
            this->window.renderer->drawTexture(*this->texture, NULL, &this->geometry.dest);
        }
        this->window.renderer->drawTexture(*textTexture, &this->geometry.src, &this->geometry.dest);
    }

    void TextBox::render()
    {
        if (text != renderedText)
        {
            if (textTexture != nullptr)
                delete textTexture;

            textTexture = this->window.renderer->renderText(text, font, this->geometry.abs);
            renderedText = text;
            this->geometry.normalize();
        }
    }
    void TextBox::update()
    {
        events.perform();

        auto &scheme = getCurrentColorScheme();
        textTexture->modColor(scheme.color);
    }
    void TextBox::draw()
    {
        window.renderer->drawTexture(*textTexture, &geometry.src, &geometry.dest);
    }

    WidgetManager::WidgetManager(ApplicationWindow &window) : Widget(window)
    {
        scheme = UI_TRANSPARENT_COLOR_SCHEME;
    }

    WidgetManager::~WidgetManager()
    {
        for (auto widget : widgets)
            delete widget;
    }

    void WidgetManager::handleEvent(const SDL_Event &e)
    {
        for (auto widget : widgets)
        {
            widget->handleGenericEvents(e);
            widget->handleEvent(e);
        }
    }
    void WidgetManager::render()
    {
        for (auto widget : widgets)
            widget->render();
    }
    void WidgetManager::update()
    {
        for (auto widget : widgets)
            widget->update();
    }
    void WidgetManager::draw()
    {
        for (auto widget : widgets)
        {
            widget->drawCommonElements();
            widget->draw();
        }
    }

    template <>
    const int Object<Widget>::err = std::atexit(Object<Widget>::atexit_handler);

    Widget::Widget(ApplicationWindow &window) : window(window), geometry(*this), font(window.config.defaultFontPath, window.config.defaultFontSize), events(*this)
    {
        window.widgets[this->uid] = this;
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
            bool isInside = geometry.dest.mouseIn();
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
        window.renderer->drawFillRectangle(geometry.dest, scheme.background);
        window.renderer->drawRectangle(geometry.dest, scheme.border);

#ifdef DEBUG
        // if (this->m_isHovered)
        // {
        auto marginbox = geometry.dest;
        marginbox.x -= geometry.margin.left;
        marginbox.y -= geometry.margin.top;
        marginbox.w += geometry.margin.x();
        marginbox.h += geometry.margin.y();
        window.renderer->drawRectangle(marginbox, Colors::Lime);

        auto paddingbox = geometry.dest;
        paddingbox.x += geometry.padding.left;
        paddingbox.y += geometry.padding.top;
        paddingbox.w -= geometry.padding.x();
        paddingbox.h -= geometry.padding.y();

        window.renderer->drawRectangle(paddingbox, Colors::Blue);
        // }
        window.renderer->drawRectangle(geometry.dest, Colors::Red);
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

    void to_json(json &j, const std::vector<Widget *> &p)
    {
        for (auto widget : p)
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