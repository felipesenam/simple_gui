#include "widgets/slabel.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Label::Label(Window &window) : Widget(window)
    {
    }

    void Label::handleEvent(const SDL_Event &e)
    {
    }
    void Label::update()
    {
        if (text != renderedText)
        {
            if (textTexture != nullptr)
                SDL_DestroyTexture(textTexture);
            SDL_PrintIfError(Warn);

            textTexture = self.window.renderer.renderText(text, font, self.geometry);
            renderedText = text;
            self.geometry.normalize();
        }
        if (textTexture != nullptr)
        {
            SDL_SetTextureColorMod(textTexture, RGB(font.color));
            SDL_PrintIfError(Warn);
        }
    }
    void Label::draw()
    {
        self.window.renderer.drawTexture(textTexture, &self.geometry.src, &self.geometry.dest);
    }
}