#pragma once

#include <stdexcept>
#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H

class FreeType
{
private:
    FT_Library _Library;

public:
    inline explicit FreeType()
    {
        if (FT_Init_FreeType(&_Library))
            throw std::runtime_error("FreeType2 Error: Fail to init FreeType2!");
    }
    inline ~FreeType() noexcept
    {
        FT_Done_FreeType(_Library);
    }
    inline operator FT_Library() const noexcept { return _Library; }
};

class FontFace
{
private:
    inline static FreeType _Library;
    FT_Face _Face;

public:
    inline explicit FontFace(const std::filesystem::path &filePath, unsigned int faceIndex)
    {
        if (FT_New_Face(_Library, filePath.c_str(), faceIndex, &_Face))
            throw std::runtime_error("FreeType2 Error: Fail to load font!");
    }
    inline ~FontFace() noexcept
    {
        FT_Done_Face(_Face);
    }
    inline operator FT_Face() const noexcept { return _Face; }
};
