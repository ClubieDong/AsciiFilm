#pragma once

#include "Media.hpp"
#include <filesystem>
#include <variant>
#include <string>
#include <boost/multi_array.hpp>
#include "../Utilities/Json.hpp"

// DEBUG
#include <iostream>

class Glyph : public Media
{
private:
    std::filesystem::path _FilePath;
    unsigned int _FaceIndex;
    unsigned int _Height;
    wchar_t _Text;
    bool _GrayScale;

    boost::multi_array<unsigned char, 2> _Data;

public:
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
            std::cout << "\033[m\n";
        }
    }
};
