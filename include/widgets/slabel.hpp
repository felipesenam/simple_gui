#ifndef __LABEL_H__
#define __LABEL_H__

#include "../score.hpp"
#include "../swidget.hpp"

namespace PROJECT_NAMESPACE
{
    class Label : public Widget, public Object<Label>
    {
    private:
        SDL_Texture *textTexture = nullptr;

        Color renderedColor;
        std::string renderedText;
        // TTF* renderedFont = nullptr;

    public:
        Label(Window &window);
        ~Label();

        std::string text;

        void render() override;
        void update() override;
        void draw() override;
    };

}
#endif // __LABEL_H__