#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class TextBox
{
public:
    TextBox(SDL_Renderer *renderer, TTF_Font *font, int x, int y, int width, int height) : m_renderer(renderer),
                                                                                           m_font(font),
                                                                                           m_x(x),
                                                                                           m_y(y),
                                                                                           m_width(width),
                                                                                           m_height(height),
                                                                                           m_selected(false),
                                                                                           m_cursorPos(0),
                                                                                           m_selectionStart(0),
                                                                                           m_text(""),
                                                                                           m_textColor({0, 0, 0, 255}),
                                                                                           m_selectionColor({192, 192, 192, 255}),
                                                                                           m_cursorColor({0, 0, 0, 255}),
                                                                                           m_textOffset(0),
                                                                                           m_maxTextOffset(0)
    {
        m_rect.x = m_x;
        m_rect.y = m_y;
        m_rect.w = m_width;
        m_rect.h = m_height;

        TTF_SizeText(m_font, "H", nullptr, &m_fontHeight);
        updateMaxTextOffset();
    }

    void handleEvent(SDL_Event &event)
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (mouseX >= m_x + 10 && mouseX <= m_x + m_width - 10 &&
                mouseY >= m_y && mouseY <= m_y + m_height)
            {
                m_selected = true;

                int clickPos = getTextIndexAtPosition(mouseX - (m_x + 10) + m_textOffset);

                m_cursorPos = std::max(0, std::min(static_cast<int>(m_text.length()), clickPos + m_textOffset));

                updateTextOffset();
            }
            else
            {
                m_selected = false;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && m_selected)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (mouseX >= m_x + 10 && mouseX <= m_x + m_width - 10 &&
                mouseY >= m_y && mouseY <= m_y + m_height)
            {
                int clickPos = getTextIndexAtPosition(mouseX - (m_x + 10) + m_textOffset);
                m_cursorPos = std::max(0, std::min(static_cast<int>(m_text.length()), clickPos));
            }
        }
        else if (event.type == SDL_TEXTINPUT && m_selected)
        {
            std::string insertedText = event.text.text;
            int insertedTextWidth = getTextWidth(insertedText);

            if (m_selectionStart != m_cursorPos)
            {
                m_text.erase(std::min(m_selectionStart, m_cursorPos), std::abs(m_selectionStart - m_cursorPos));
                m_cursorPos = m_selectionStart = std::min(m_selectionStart, m_cursorPos);
            }

            m_text.insert(m_cursorPos, insertedText);
            m_selectionStart = m_cursorPos += insertedText.length();

            updateTextOffset();
        }
        else if (event.type == SDL_KEYDOWN && m_selected)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                if (m_selectionStart != m_cursorPos)
                {
                    m_text.erase(std::min(m_selectionStart, m_cursorPos), std::abs(m_selectionStart - m_cursorPos));
                    m_cursorPos = m_selectionStart;
                }
                else if (m_cursorPos > 0)
                {
                    m_text.erase(m_cursorPos - 1, 1);
                    m_cursorPos--;
                }
                break;
            case SDLK_DELETE:
                if (m_selectionStart != m_cursorPos)
                {
                    m_text.erase(std::min(m_selectionStart, m_cursorPos), std::abs(m_selectionStart - m_cursorPos));
                    m_cursorPos = m_selectionStart;
                }
                else if (m_cursorPos < static_cast<int>(m_text.length()))
                {
                    m_text.erase(m_cursorPos, 1);
                }
                break;
            case SDLK_LEFT:
                if (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] && !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT])
                {
                    if (m_cursorPos > 0)
                    {
                        m_cursorPos--;
                        m_selectionStart = m_cursorPos;
                    }
                }
                else
                {
                    if (m_cursorPos > 0)
                    {
                        m_cursorPos--;
                    }
                }

                updateTextOffset();
                break;

            case SDLK_RIGHT:
                if (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] && !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT])
                {
                    if (m_cursorPos < static_cast<int>(m_text.length()))
                    {
                        m_cursorPos++;
                        m_selectionStart = m_cursorPos;
                    }
                }
                else
                {
                    if (m_cursorPos < static_cast<int>(m_text.length()))
                    {
                        m_cursorPos++;
                    }
                }

                updateTextOffset();
                break;
            }
        }
    }

    void render()
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(m_renderer, &m_rect);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(m_renderer, &m_rect);

        if (m_selected)
        {
            int selectionStartX = getTextWidth(m_text.substr(0, m_selectionStart)) + 10 - m_textOffset;
            int selectionEndX = getTextWidth(m_text.substr(0, m_cursorPos)) + 10 - m_textOffset;

            SDL_SetRenderDrawColor(m_renderer, m_selectionColor.r, m_selectionColor.g, m_selectionColor.b, m_selectionColor.a);
            SDL_Rect selectionRect = {m_x + 10 + selectionStartX, m_y + (m_height - m_fontHeight) / 2, selectionEndX - selectionStartX, m_fontHeight};
            SDL_RenderFillRect(m_renderer, &selectionRect);

            int cursorX = getTextWidth(m_text.substr(0, m_cursorPos)) + 10 - m_textOffset;

            SDL_SetRenderDrawColor(m_renderer, m_cursorColor.r, m_cursorColor.g, m_cursorColor.b, m_cursorColor.a);
            SDL_RenderDrawLine(m_renderer, m_x + 10 + cursorX, m_y + (m_height - m_fontHeight) / 2, m_x + 10 + cursorX, m_y + (m_height + m_fontHeight) / 2);
        }

        std::string visibleText = getVisibleText();
        renderText(visibleText, m_x + 10, m_y + (m_height - m_fontHeight) / 2, m_textColor);
    }

private:
    SDL_Renderer *m_renderer;
    TTF_Font *m_font;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    SDL_Rect m_rect;
    bool m_selected;
    int m_cursorPos;
    int m_selectionStart;
    std::string m_text;
    SDL_Color m_textColor;
    SDL_Color m_selectionColor;
    SDL_Color m_cursorColor;
    int m_textOffset;
    int m_maxTextOffset;
    int m_fontHeight;

    int getTextWidth(const std::string &text)
    {
        int textWidth = 0;
        TTF_SizeText(m_font, text.c_str(), &textWidth, nullptr);
        return textWidth;
    }

    void updateMaxTextOffset()
    {
        int textWidth = getTextWidth(m_text);
        m_maxTextOffset = std::max(0, textWidth - (m_width - 20));
    }

    int getTextIndexAtPosition(int positionX)
    {
        int textIndex = 0;
        int textWidth = 0;
        int charWidth = 0;

        for (char character : m_text)
        {
            TTF_SizeText(m_font, std::string(1, character).c_str(), &charWidth, nullptr);

            if (textWidth + charWidth / 2 >= positionX)
            {
                break;
            }

            textWidth += charWidth;
            textIndex++;
        }

        return textIndex;
    }

    void renderText(const std::string &text, int x, int y, SDL_Color color)
    {
        if (!text.empty())
        {
            int textWidth = getTextWidth(text);
            int renderWidth = std::min(textWidth - m_textOffset, m_width - 20);

            SDL_Surface *surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            SDL_Rect rect = {x, y, textWidth, surface->h};
            SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }

    void updateTextOffset()
    {
        updateMaxTextOffset();

        int cursorX = getTextWidth(m_text.substr(0, m_cursorPos)) + 10 - m_textOffset;

        if (cursorX < 0)
        {
            m_textOffset = std::max(0, getTextWidth(m_text.substr(0, m_cursorPos)) - (m_width - 20) + 10);
        }
        else if (cursorX > m_width - 20)
        {
            int offsetChange = cursorX - (m_width - 20);
            int maxOffsetChange = std::max(0, m_maxTextOffset - m_textOffset);
            m_textOffset += std::min(offsetChange, maxOffsetChange);
        }
        else
        {
            int textWidth = getTextWidth(m_text);
            int renderWidth = std::min(textWidth, m_width - 20);

            if (textWidth > renderWidth)
            {
                int cursorEndX = getTextWidth(m_text.substr(0, m_cursorPos + 1)) + 10 - m_textOffset;

                if (cursorEndX > m_width - 20)
                {
                    int offsetChange = cursorEndX - (m_width - 20);
                    int maxOffsetChange = std::max(0, m_maxTextOffset - m_textOffset);
                    m_textOffset += std::min(offsetChange, maxOffsetChange);
                }
            }
            else
            {
                m_textOffset = 0;
            }
        }
    }

    std::string getVisibleText()
    {
        int availableWidth = m_width - 20;
        std::string visibleText = "";

        int textWidth = getTextWidth(m_text);

        if (textWidth <= availableWidth)
        {
            // O texto inteiro cabe na caixa de texto
            visibleText = m_text;
        }
        else
        {
            int startIndex = std::max(0, m_textOffset);
            int endIndex = static_cast<int>(m_text.length()) - 1;

            int visibleWidth = availableWidth;

            // Determinar o índice de início do texto visível
            int textOffset = std::min(m_textOffset, textWidth - availableWidth);
            int accumulatedWidth = 0;
            int currentIndex = 0;

            while (currentIndex < static_cast<int>(m_text.length()) && accumulatedWidth < textOffset)
            {
                int charWidth = getTextWidth(std::string(1, m_text[currentIndex]));
                accumulatedWidth += charWidth;
                currentIndex++;
            }

            startIndex = currentIndex;

            // Determinar o índice de fim do texto visível
            accumulatedWidth = 0;
            currentIndex = startIndex;

            while (currentIndex < static_cast<int>(m_text.length()) && accumulatedWidth < visibleWidth)
            {
                int charWidth = getTextWidth(std::string(1, m_text[currentIndex]));
                accumulatedWidth += charWidth;

                if (accumulatedWidth <= visibleWidth)
                {
                    endIndex = currentIndex;
                }

                currentIndex++;
            }

            visibleText = m_text.substr(startIndex, endIndex - startIndex + 1);
        }

        return visibleText;
    }
};

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Text Box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font *font = TTF_OpenFont("assets/fonts/arial.ttf", 24);

    TextBox textBox(renderer, font, 100, 100, 400, 50);

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            textBox.handleEvent(event);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        textBox.render();

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
