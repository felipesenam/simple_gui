#include "swidgets.hpp"

#include "swindow.hpp"
#include "sscheme.hpp"
namespace sgui
{
    void Geometry::normalize()
    {
        int cw = widget.contentWidth();
        int ch = widget.contentHeight();

        switch (behavior)
        {
        case hug:
        {
            width = cw - padding.x();
            height = ch - padding.y();
            break;
        }
        case fill:
        {
            if (widget.parent)
            {
                width = widget.parent->contentWidth();
                height = widget.parent->contentHeight();
            }
            else
            {
                const auto windowSize = widget.window.size();
                width = windowSize.first;
                height = windowSize.second;
            }
            break;
        }
        default:
            break;
        }

        if (widget.parent)
        {
            if (overflow == hidden)
            {
                clip = dest.intersect(widget.parent->geometry.clip);
            }
            else
            {
                clip = widget.parent->geometry.clip;
            }
        }
        else
        {
            clip = {x, y, width, height};
        }
    }

    void Geometry::confine(Geometry &geometry)
    {
        const int paddedX = x + padding.left;
        const int paddedY = y + padding.top;

        dest.x = std::max(paddedX, geometry.clip.x);
        dest.y = std::max(paddedY, geometry.clip.y);
        dest.w = std::min(width - (dest.x - paddedX), geometry.clip.w - (dest.x - geometry.clip.x));
        dest.h = std::min(height - (dest.y - paddedY), geometry.clip.h - (dest.y - geometry.clip.y));

        src.x = std::max(0, dest.x - x - padding.left);
        src.y = std::max(0, dest.y - y - padding.top);
        src.w = std::min(width - src.x, dest.w);
        src.h = std::min(height - src.y, dest.h);

        widget.show = clip.AABB(paddingRect());
    }

    Rect Geometry::marginRect() const noexcept
    {
        return Rect(
            widget.geometry.x - margin.left,
            widget.geometry.y - margin.top,
            widget.geometry.width + padding.x() + margin.x(),
            widget.geometry.height + padding.y() + margin.y());
    }

    Rect Geometry::paddingRect() const noexcept
    {
        return Rect(
            widget.geometry.x,
            widget.geometry.y,
            widget.geometry.width + padding.x(),
            widget.geometry.height + padding.y());
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

        geometry.width = 0;
        geometry.height = 0;
    }

    void Bitmap::alloc(size_t width, size_t height)
    {
        erase();
        geometry.width = width;
        geometry.height = height;

        surface = new Surface(width, height);

        m_render = true;
    }

    void Bitmap::open(const std::string &file)
    {
        this->erase();

        surface = new Surface(file);
        this->geometry.width = surface->width();
        this->geometry.height = surface->height();
    }

    void Bitmap::render()
    {
        if (!surface->mIsDirty)
            return;

        surface->mIsDirty = false;

        if (m_texture != nullptr)
            delete m_texture;

        m_texture = window.renderer->createTextureFromSurface(*surface);
    }

    void Bitmap::draw()
    {
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
        const int cw = widgetWidth();

        switch (style.horizontalAlign)
        {
        case left:
            lx = geometry.x + currentWidth + geometry.padding.left;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        case center:
            lx = geometry.x + currentWidth + ((geometry.width - cw) / 2) + geometry.padding.left;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        case right:
            lx = geometry.x + currentWidth + geometry.width + geometry.padding.left - cw;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        }

        switch (style.verticalAlign)
        {
        case top:
            ly = geometry.y + geometry.padding.top;
            widget.geometry.y = ly + widget.geometry.margin.top;
            break;
        case middle:
            ly = geometry.y + ((geometry.height - widget.height()) / 2) + geometry.padding.top;
            widget.geometry.y = ly + widget.geometry.margin.top;
            break;
        case bottom:
            ly = geometry.y + geometry.height + geometry.padding.top;
            widget.geometry.y = ly + widget.geometry.margin.top - widget.height();
            break;
        }

        switch (style.justifyContent)
        {
        case between:
            currentWidth += spaceBetween;
            break;
        case around:
            currentWidth += spaceAround;
            lx += spaceAround;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        default:
            break;
        }
    }
    void Flex::posWidgetVertical(int &lx, int &ly, int &currentHeight, Widget &widget, const int spaceBetween, const int spaceAround)
    {
        const int ch = widgetHeight();

        switch (style.horizontalAlign)
        {
        case left:
            lx = geometry.x + geometry.padding.left;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        case center:
            lx = geometry.x + ((geometry.width - widget.width()) / 2) + geometry.padding.left;
            widget.geometry.x = lx + widget.geometry.margin.left;
            break;
        case right:
            lx = geometry.x + geometry.width + geometry.padding.left;
            widget.geometry.x = lx + widget.geometry.margin.left - widget.width();
            break;
        }

        switch (style.verticalAlign)
        {
        case top:
            ly = geometry.y + currentHeight + geometry.padding.top;
            widget.geometry.y = ly + widget.geometry.margin.top;
            break;
        case middle:
            ly = geometry.y + currentHeight + ((geometry.height - ch) / 2) + geometry.padding.top;
            widget.geometry.y = ly + widget.geometry.margin.top;
            break;
        case bottom:
            ly = geometry.y + currentHeight + geometry.height + geometry.padding.top - ch;
            widget.geometry.y = ly + widget.geometry.margin.top;
            break;
        }

        switch (style.justifyContent)
        {
        case between:
            currentHeight += spaceBetween;
            break;
        case around:
            currentHeight += spaceAround;
            ly += spaceAround;
            widget.geometry.y = ly + widget.geometry.margin.top;
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

                widget->preUpdate();
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

                widget->preUpdate();
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

            textTexture = this->window.renderer->renderText(text, font, geometry.width, geometry.height);
            renderedText = text;
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

            textTexture = this->window.renderer->renderText(text, font, geometry.width, geometry.height);
            renderedText = text;
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
            widget->geometry.confine(geometry);
            if (widget->show)
            {
                widget->drawCommonElements();
                widget->draw();
            }
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
        if (parent)
        {
            const auto scheme = this->getCurrentColorScheme();

            const auto paddingRect = geometry.paddingRect();
            const auto backgroundRect = paddingRect.intersect(parent->geometry.clip);

            window.renderer->drawRectangle(backgroundRect, scheme.border);
            window.renderer->drawFillRectangle(backgroundRect, scheme.background);

#ifdef DEBUG
            const auto marginRect = geometry.marginRect();

            window.renderer->drawRectangle(marginRect, Colors::Lime);
            window.renderer->drawRectangle(paddingRect, Colors::Blue);
            window.renderer->drawRectangle(geometry.dest, Colors::Red);
#endif
        }
    }

    void Widget::preUpdate()
    {
        geometry.normalize();
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