#include "Glyph.hpp"
#include <cassert>
#include <cmath>
#include <locale>
#include "../Utilities/Font.hpp"
#include "../Utilities/Json.hpp"
#include "../Utilities/UTF.hpp"

void Glyph::Load()
{
    FontFace face(_FilePath, _FaceIndex);
    if (FT_Set_Pixel_Sizes(face, _Width, _Height))
        throw std::runtime_error("FreeType2 Error: Fail to set size!");
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
        throw std::runtime_error("FreeType2 Error: Fail to set charmap!");
    constexpr FT_ULong LOAD_FLAGS[2] = {FT_LOAD_RENDER | FT_LOAD_MONOCHROME, FT_LOAD_RENDER};
    if (FT_Load_Char(face, _Text, LOAD_FLAGS[_GrayScale]))
        throw std::runtime_error("FreeType2 Error: Fail to load and render glyph!");
    auto &bitmap = static_cast<FT_Face>(face)->glyph->bitmap;
    _Data.resize(boost::extents[bitmap.rows][bitmap.width]);
    if (_GrayScale)
        for (unsigned int i = 0; i < bitmap.rows; ++i)
            for (unsigned int j = 0; j < bitmap.width; ++j)
                // I hope compiler do the loop-invariant code motion for me.
                _Data[i][j] = *(bitmap.buffer + bitmap.pitch * i + j);
    else
        for (unsigned int i = 0; i < bitmap.rows; ++i)
            for (unsigned int j = 0; j < bitmap.width; ++j)
                // I hope compiler do the loop-invariant code motion for me.
                _Data[i][j] = ((*(bitmap.buffer + bitmap.pitch * i + j / 8) >> (7 - j % 8)) & 1) * 0xFF;
}

Glyph::Glyph(json &option, const std::string &location, double aspectRatio)
{
    using jt = json::value_t;
    static auto validator =
        JsonValidator()
            .AddRequired("id", jt::string)
            .AddRequired("filePath", jt::string)
            .AddOptional("faceIndex", jt::number_unsigned, 0)
            .AddOptional("width", jt::number_unsigned, 0)
            .AddOptional("height", jt::number_unsigned, 0)
            .AddRequired("text", jt::string)
            .AddOptional("grayScale", jt::boolean, false);
    validator.Validate(option, location);
    _ID = option["id"].get<std::string>();
    _FilePath = option["filePath"].get<std::string>();
    _FaceIndex = option["faceIndex"].get<unsigned int>();
    _Width = option["width"].get<unsigned int>();
    _Height = option["height"].get<unsigned int>();
    if (_Width == 0 && _Height != 0)
        _Width = std::round(_Height * aspectRatio);
    else if (_Width != 0 && _Height == 0)
        _Height = std::round(_Width / aspectRatio);
    else if (_Width == 0 && _Height == 0)
        throw std::invalid_argument("Expect at least one non-zero value "
                                    "between \"width\" and \"height\" fields in " +
                                    location);
    auto textStr = Utf8ToUnicode::Convert(option["text"].get<std::string>());
    if (textStr.size() != 1)
        throw std::invalid_argument("Expect one and only one character in " + location + ".text");
    _Text = textStr[0];
    _GrayScale = option["grayScale"].get<bool>();
    Load();
}
