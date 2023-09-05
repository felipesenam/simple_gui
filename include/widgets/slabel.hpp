#ifndef __LABEL_H__
#define __LABEL_H__

#include "score.hpp"
#include "swidget.hpp"

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

        friend void to_json(json &j, const Label &p)
        {
            j["type"] = type(p);
            j["text"] = p.text;
            j["geometry"] = p.geometry;
            j["scheme"] = p.scheme;
        }
        friend void from_json(const json &j, Label &p)
        {
            SETATTR_IF_JSON_CONTAINS(j, p, text);
            SETATTR_IF_JSON_CONTAINS(j, p, geometry);
            SETATTR_IF_JSON_CONTAINS(j, p, scheme);
        }
    };

}
#endif // __LABEL_H__