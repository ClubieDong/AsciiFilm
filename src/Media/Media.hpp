#pragma once

#include <string>
#include <filesystem>
#include "../Utilities/Json.hpp"

// CRTP to avoid virtual functions
template <typename T>
class Media
{
protected:
    class Info
    {
    public:
        std::string ID;
        std::filesystem::path FilePath;

    protected:
        inline ~Info() = default;
    };

    inline ~Media() = default;

    template <typename Info>
    inline void Create(Info &&info)
    {
        static_cast<T *>(this)->LoadFromFile(std::move(info));
    }

    inline static void FitAspectRatio(unsigned int &width, unsigned int &height,
                               double pixelAspectRatio, double imageAspectRatio)
    {
        if (width == 0)
            width = std::round(pixelAspectRatio / imageAspectRatio * height);
        else if (height == 0)
            height = std::round(imageAspectRatio / pixelAspectRatio * width);
    }
};
