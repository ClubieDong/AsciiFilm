#include "Glyph.hpp"
#include "../Utilities/UTF.hpp"
#include "../Utilities/Font.hpp"

Glyph::Info::Info(json &option, const std::string &location, double aspectRatio)
{
    using jt = json::value_t;
    static auto validator =
        JsonValidator()
            .AddOptional("id", jt::string, "")
            .AddRequired("filePath", jt::string)
            .AddOptional("faceIndex", jt::number_unsigned, 0)
            .AddOptional("width", jt::number_unsigned, 0)
            .AddOptional("height", jt::number_unsigned, 0)
            .AddRequired("text", jt::string)
            .AddOptional("grayScale", jt::boolean, false);
    validator.Validate(option, location);
    ID = option["id"].get<std::string>();
    FilePath = option["filePath"].get<std::string>();
    FaceIndex = option["faceIndex"].get<unsigned int>();
    Width = option["width"].get<unsigned int>();
    Height = option["height"].get<unsigned int>();
    Text = Utf8ToUnicode::Convert(option["text"].get<std::string>());
    GrayScale = option["grayScale"].get<bool>();
    if (ID.empty())
        ID = option["text"].get<std::string>();
    if (Width != 0 && Height == 0)
        Height = Width / aspectRatio;
    else if (Width == 0 && Height != 0)
        Width = Height * aspectRatio;
    else if (Width == 0 && Height == 0)
        throw std::invalid_argument("Expect at least one non-zero value "
                                    "between \"width\" and \"height\" fields in " +
                                    location);
}

void Glyph::LoadFromFile(Info&& info)
{
    FontFace face(info.FilePath, info.FaceIndex);
    if (FT_Set_Pixel_Sizes(face, info.Width, info.Height))
        throw std::runtime_error("FreeType2 Error: Fail to set size!");
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
        throw std::runtime_error("FreeType2 Error: Fail to set charmap!");
    constexpr FT_ULong LOAD_FLAGS[2] = {FT_LOAD_RENDER | FT_LOAD_MONOCHROME, FT_LOAD_RENDER};
    auto loadFlags = LOAD_FLAGS[info.GrayScale];
    const auto &bitmap = static_cast<FT_Face>(face)->glyph->bitmap;
    _Data.reserve(info.Text.size());
    for (auto wch : info.Text)
    {
        if (FT_Load_Char(face, wch, loadFlags))
            throw std::runtime_error("FreeType2 Error: Fail to load and render glyph!");
        _Data.emplace_back(boost::extents[bitmap.rows][bitmap.width]);
        for (unsigned int i = 0; i < bitmap.rows; ++i)
            for (unsigned int j = 0; j < bitmap.width; ++j)
                // I hope compiler do the loop-invariant code motion for me.
                // TODO: Check if compiler actually do the code motion, probably need to optimize.
                if (info.GrayScale)
                    _Data.back()[i][j] = *(bitmap.buffer + bitmap.pitch * i + j);
                else
                    _Data.back()[i][j] = ((*(bitmap.buffer + bitmap.pitch * i + j / 8) >> (7 - j % 8)) & 1) * 0xFF;
    }
}
