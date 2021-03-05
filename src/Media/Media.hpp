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
        unsigned int Width, Height;

    protected:
        inline ~Info() = default;

        explicit Info(json &option, const std::string &location)
        {
            using jt = json::value_t;
            static auto validator =
                JsonValidator()
                    .AddRequired("id", jt::string)
                    .AddRequired("filePath", jt::string)
                    .AddOptional("width", jt::number_unsigned, 0)
                    .AddOptional("height", jt::number_unsigned, 0);
            validator.Validate(option, location);
            ID = option["id"].get<std::string>();
            FilePath = option["filePath"].get<std::string>();
            Width = option["width"].get<unsigned int>();
            Height = option["height"].get<unsigned int>();
            if (ID.empty())
                throw std::invalid_argument("Expect a non-empty id in " + location);
            if (Width == 0 && Height == 0)
                throw std::invalid_argument("Expect at least one non-zero value "
                                            "between \"width\" and \"height\" fields in " +
                                            location);
        }
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
