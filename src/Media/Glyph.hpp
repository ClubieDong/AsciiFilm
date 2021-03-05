#pragma once

#include <string>
#include <vector>
#include <boost/multi_array.hpp>
#include "../Utilities/Json.hpp"
#include "Media.hpp"

// DEBUG
#include <iostream>

class Glyph : public Media<Glyph>
{
    friend class Media<Glyph>;

private:
    struct Info : public Media<Glyph>::Info
    {
        unsigned int FaceIndex;
        std::wstring Text;
        bool GrayScale;

        explicit Info(json &option, const std::string &location, double aspectRatio);
    };

    std::vector<boost::multi_array<unsigned char, 2>> _Data;

    void LoadFromFile(Info &&info);

public:
    inline explicit Glyph(json &option, const std::string &location, double aspectRatio)
    {
        Create(Info(option, location, aspectRatio));
    }

    // DEBUG
    void Show() const
    {
        for (const auto &i : _Data.front())
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
