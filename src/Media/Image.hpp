#pragma once

#include <string>
#include <boost/multi_array.hpp>
#include <opencv2/imgcodecs.hpp>
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
        double AspectRatio;

        inline explicit Info(json &option, const std::string &location, double aspectRatio)
            : Media::Info(option, location), AspectRatio(aspectRatio) {}
    };

    boost::multi_array<Color, 2> _Data;

    void LoadFromMat(cv::Mat &&img, unsigned int width, unsigned int height);
    inline void LoadFromFile(Info &&info)
    {
        auto img = cv::imread(info.FilePath, cv::IMREAD_UNCHANGED);
        if (img.empty())
            throw std::invalid_argument("OpenCV Error: Fail to load image!");
        FitAspectRatio(info.Width, info.Height, info.AspectRatio,
                       static_cast<double>(img.size[0]) / img.size[1]);
        LoadFromMat(std::move(img), info.Width, info.Height);
    }

public:
    inline explicit Image(json &option, const std::string &location, double aspectRatio)
    {
        Create(Info(option, location, aspectRatio));
    }
    inline explicit Image(cv::Mat &&img, unsigned int width, unsigned int height)
    {
        LoadFromMat(std::move(img), width, height);
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
