#pragma once

#include <string>
#include <boost/multi_array.hpp>
#include "../Utilities/Json.hpp"
#include "../Utilities/Color.hpp"
#include "Media.hpp"

// DEBUG
#include <iostream>

class Image : public Media<Image>
{
    friend class Media<Image>;

private:
    struct Info : public Media<Image>::Info
    {
        unsigned int Width, Height;
        double AspectRatio;

        explicit Info(json &option, const std::string &location, double aspectRatio);
    };

    boost::multi_array<Color, 2> _Data;

    void LoadFromFile(Info &&info);

public:
    inline explicit Image(json &option, const std::string &location, double aspectRatio)
    {
        Create(option, location, aspectRatio);
    }

    // DEBUG
    void Show() const
    {
        for (const auto &i : _Data)
        {
            for (const auto &j : i)
            {
                auto r = std::to_string(j.R * j.A / 255);
                auto g = std::to_string(j.G * j.A / 255);
                auto b = std::to_string(j.B * j.A / 255);
                std::cout << "\033[48;2;" << r << ';' << g << ';' << b << "m ";
            }
            std::cout << "\033[m\n";
        }
    }
};
