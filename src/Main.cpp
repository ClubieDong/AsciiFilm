#include <fstream>
#include "Utilities/Json.hpp"
#include "Media/Glyph.hpp"
int main()
{
    std::ifstream f("script.json");
    json j;
    f >> j;
    Glyph g(j["media"]["glyphs"][0], "media.glyph.0", 18 / 8.);
    g.Show();
    return 0;
}
