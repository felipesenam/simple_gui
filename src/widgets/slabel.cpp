#include "widgets/slabel.hpp"
#include "sscheme.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Label::Label(Window &window) : Widget(window)
    {
        self.scheme = UI_LABEL_COLOR_SCHEME;
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

            textTexture = self.window.renderer.renderText(text, font, self.geometry, currentScheme->background);
            renderedText = text;
            self.geometry.normalize();
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
        self.window.renderer.drawTexture(textTexture, &self.geometry.src, &self.geometry.dest);
    }
}