#include "sapplication.hpp"
#include "scolors.hpp"
using namespace sgui;

int main(/*int argc, char const *argv[]*/)
{
    WindowConfig config;
    config.renderer.drawColor = Colors::MintCream;

    Application app(config);

    return app();
}
