#include "Glyph.hpp"
#include <stdexcept>
#include "../Utilities/Font.hpp"
#include "../Utilities/UTF.hpp"

Glyph::Glyph(json &option, const std::string &location, double aspectRatio)
{
    // Get property from JSON
    using jt = json::value_t;
    static auto validator =
        JsonValidator()
            .AddRequired("id", jt::string)
            .AddRequired("filePath", jt::string)
            .AddOptional("faceIndex", jt::number_unsigned, 0)
            .AddRequired("height", jt::number_unsigned)
            .AddRequired("text", jt::string)
            .AddOptional("grayScale", jt::boolean, false);
    validator.Validate(option, location);
    _ID = option["id"].get<std::string>();
    _FilePath = option["filePath"].get<std::string>();
    _FaceIndex = option["faceIndex"].get<unsigned int>();
    _Height = option["height"].get<unsigned int>();
    auto textStr = Utf8ToUnicode::Convert(option["text"].get<std::string>());
    if (textStr.size() != 1)
        throw std::invalid_argument("Expect one and only one character in " + location + ".text");
    _Text = textStr[0];
    _GrayScale = option["grayScale"].get<bool>();
    // Load glyph
    FontFace face(_FilePath, _FaceIndex);
    if (FT_Set_Pixel_Sizes(face, _Height * aspectRatio, _Height))
        throw std::runtime_error("FreeType2 Error: Fail to set size!");
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
        throw std::runtime_error("FreeType2 Error: Fail to set charmap!");
    constexpr FT_ULong LOAD_FLAGS[2] = {FT_LOAD_RENDER | FT_LOAD_MONOCHROME, FT_LOAD_RENDER};
    if (FT_Load_Char(face, _Text, LOAD_FLAGS[_GrayScale]))
        throw std::runtime_error("FreeType2 Error: Fail to load and render glyph!");
    auto &bitmap = static_cast<FT_Face>(face)->glyph->bitmap;
    _Data.resize(boost::extents[bitmap.rows][bitmap.width]);
    for (unsigned int i = 0; i < bitmap.rows; ++i)
        for (unsigned int j = 0; j < bitmap.width; ++j)
            // I hope compiler do the loop-invariant code motion for me.
            if (_GrayScale)
                _Data[i][j] = *(bitmap.buffer + bitmap.pitch * i + j);
            else
                _Data[i][j] = ((*(bitmap.buffer + bitmap.pitch * i + j / 8) >> (7 - j % 8)) & 1) * 0xFF;
}
