#include <fstream>
#include "Utilities/Json.hpp"
#include "Media/Glyph.hpp"
#include "Media/Image.hpp"

int main()
{
    std::ifstream f("script.json");
    json j;
    f >> j;
    Glyph(j["media"]["glyphs"][0], "media.glyphs.0", 18 / 8.).Show();
    Image(j["media"]["images"][0], "media.images.0", 18 / 8.).Show();
    return 0;
}
