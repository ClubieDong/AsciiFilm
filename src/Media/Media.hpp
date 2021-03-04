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

    inline void Create(json &option, const std::string &location, double aspectRatio)
    {
        typename T::Info info(option, location, aspectRatio);
        static_cast<T*>(this)->LoadFromFile(std::move(info));
    }
};
