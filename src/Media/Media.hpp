#pragma once

#include <string>
#include <string_view>

class Media
{
protected:
    std::string _ID;

    inline explicit Media() = default;
    inline virtual ~Media() = default;
    inline Media(std::string_view id) : _ID(id) {}
};
