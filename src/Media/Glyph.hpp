#pragma once

#include "Media.hpp"
#include <filesystem>
#include <variant>
#include <string>
#include <string_view>
#include <boost/multi_array.hpp>
#include "../Utilities/Json.hpp"

// DEBUG
#include <iostream>

class Glyph : public Media
{
private:
    std::filesystem::path _FilePath;
    unsigned int _FaceIndex;
    unsigned int _Width, _Height;
    wchar_t _Text;
    bool _GrayScale;

    boost::multi_array<unsigned char, 2> _Data;

    void Load();

public:
    inline explicit Glyph(std::string_view id, const std::filesystem::path &filePath,
                          unsigned int faceIndex, unsigned int width, unsigned int height,
                          wchar_t text, bool grayScale)
        : Media(id), _FilePath(filePath), _FaceIndex(faceIndex), _Width(width),
          _Height(height), _Text(text), _GrayScale(grayScale) { Load(); }

    explicit Glyph(json &option, const std::string &location, double aspectRatio);

    // DEBUG
    void Show() const
    {
        for (const auto &i : _Data)
        {
            for (auto j : i)
            {
                auto c = std::to_string(255 - j);
                std::cout << "\033[48;2;" << c << ';' << c << ';' << c << "m ";
            }
            std::cout << '\n';
        }
        std::cout << "\033[m";
    }
};
