#pragma once

#include <string>
#include <vector>
#include "../Utilities/Json.hpp"
#include "Media.hpp"
#include "Image.hpp"

// DEBUG
#include <iostream>

class Video : public Media<Video>
{
    friend class Media<Video>;

private:
    struct Info : public Media<Video>::Info
    {
        unsigned int Width, Height;
        double AspectRatio;
        double FPS;

        explicit Info(json &option, const std::string &location,
                      double aspectRatio, double fps);
    };

    std::vector<Image> _Data;

    void LoadFromFile(Info &&info);

public:
    inline explicit Video(json &option, const std::string &location,
                          double aspectRatio, double fps)
    {
        Create(Info(option, location, aspectRatio, fps));
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
