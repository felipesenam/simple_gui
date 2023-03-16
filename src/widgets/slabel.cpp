#include "widgets/slabel.hpp"
#include "swindow.hpp"

namespace PROJECT_NAMESPACE
{
    Label::Label(Window &window) : Widget(window)
    {
    }
    Label::~Label()
    {
        if (textTexture)
        {
            SDL_DestroyTexture(textTexture);
        }
    }

    void Label::handleEvent(const SDL_Event &e)
    {
        handleWindowEvents(e);
    }

    void Label::render()
    {
        if (text != renderedText)
        {
            if (textTexture != nullptr)
            {
                SDL_DestroyTexture(textTexture);
            }
            SDL_PrintIfError(Warn);

            textTexture = self.window.renderer.renderText(text, font, self.geometry);
            renderedText = text;
            self.geometry.normalize();
        }
    }

    void Label::update()
    {
        if (textTexture != nullptr)
        {
            if (SDL_SetTextureColorMod(textTexture, RGB(font.color)) != 0)
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