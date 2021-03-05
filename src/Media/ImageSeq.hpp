#pragma once

#include <string>
#include <vector>
#include "../Utilities/Json.hpp"
#include "Media.hpp"
#include "Image.hpp"

// DEBUG
#include <iostream>

class ImageSeq : public Media<ImageSeq>
{
    friend class Media<ImageSeq>;

private:
    struct Info : public Media<ImageSeq>::Info
    {
        double AspectRatio;

        inline explicit Info(json &option, const std::string &location, double aspectRatio)
            : Media::Info(option, location), AspectRatio(aspectRatio) {}
    };

    std::vector<Image> _Data;

    void LoadFromFile(Info &&info);

public:
    inline explicit ImageSeq(json &option, const std::string &location, double aspectRatio)
    {
        Create(Info(option, location, aspectRatio));
    }

    // DEBUG
    void Show(double fps) const
    {
        for (const auto &i : _Data)
        {
            system("clear");
            i.Show();
            std::cout << std::flush;
            usleep(1000000 / fps);
        }
    }
};
