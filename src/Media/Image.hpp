#pragma once

#include "Media.hpp"
#include <filesystem>
#include <string>
#include <boost/multi_array.hpp>
#include "../Utilities/Json.hpp"
#include "../Utilities/Color.hpp"

// DEBUG
#include <iostream>

class Image : public Media
{
private:
    std::filesystem::path _FilePath;
    unsigned int _Width, _Height;

    boost::multi_array<Color, 2> _Data;

public:
    explicit Image(json &option, const std::string &location, double aspectRatio);

    // DEBUG
    void Show() const
    {
        for (const auto &i : _Data)
        {
            for (const auto &j : i)
            {
                auto r = std::to_string(j.R);
                auto g = std::to_string(j.G);
                auto b = std::to_string(j.B);
                std::cout << "\033[48;2;" << r << ';' << g << ';' << b << "m ";
            }
            std::cout << "\033[m\n";
        }
    }
};
