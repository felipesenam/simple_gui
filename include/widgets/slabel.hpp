#ifndef __LABEL_H__
#define __LABEL_H__

#include "../score.hpp"
#include "../swidget.hpp"

namespace PROJECT_NAMESPACE
{
    class Label : public Widget, public Object<Label>
    {
    private:
        SDL_Texture *text_texture;

    public:
        Label(Window &window) : Widget(window)
        {
        }

        void handleEvent(const SDL_Event &e) override
        {
        }
        void update() override
        {
        }
        void draw() override
        {
            // self.window.renderer
        }
    };

}
#endif // __LABEL_H__