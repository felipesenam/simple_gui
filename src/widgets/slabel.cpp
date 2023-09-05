#include "widgets/slabel.hpp"
#include "sscheme.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
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
}