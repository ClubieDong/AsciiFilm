#include <fstream>
#include "Utilities/Json.hpp"
#include "Media/Glyph.hpp"
#include "Media/Image.hpp"
#include "Media/Video.hpp"

int main()
{
    std::ifstream f("script.json");
    json j;
    f >> j;
    // Glyph(j["media"]["glyphs"][0], "media.glyphs.0", 18 / 8.).Show();
    // Image(j["media"]["images"][0], "media.images.0", 18 / 8.).Show();
    Video(j["media"]["videos"][0], "media.videos.0", 18 / 8., 10).Show(10);
    return 0;
}
