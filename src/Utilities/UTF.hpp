#pragma once

#include <string>
#include <stdexcept>

class Utf8ToUnicode
{
    // Sadly `std::wstring_convert` is depreciated in C++17,
    // so I need to provide this function on my own.
public:
    static std::wstring Convert(const std::string &input)
    {
        std::wstring output;
        int state = 0;
        for (auto iter = input.cbegin(); iter != input.cend(); ++iter)
        {
            unsigned char ch = *iter;
            if (state == 0)
                if ((ch & 0b1000'0000) == 0b0000'0000)
                    output.push_back(ch & 0b0111'1111), state = 0;
                else if ((ch & 0b1110'0000) == 0b1100'0000)
                    output.push_back(ch & 0b0001'1111), state = 1;
                else if ((ch & 0b1111'0000) == 0b1110'0000)
                    output.push_back(ch & 0b0000'1111), state = 2;
                else if ((ch & 0b1111'1000) == 0b1111'0000)
                    output.push_back(ch & 0b0000'0111), state = 3;
                else if ((ch & 0b1111'1100) == 0b1111'1000)
                    output.push_back(ch & 0b0000'0011), state = 4;
                else if ((ch & 0b1111'1110) == 0b1111'1100)
                    output.push_back(ch & 0b0000'0001), state = 5;
                else
                    throw std::invalid_argument("Illegal UTF-8 encoding!");
            else
            {
                if ((ch & 0b1100'0000) != 0b1000'0000)
                    throw std::invalid_argument("Illegal UTF-8 encoding!");
                output.back() = (output.back() << 6) + (ch & 0b0011'1111);
                --state;
            }
        }
        if (state != 0)
            throw std::invalid_argument("Illegal UTF-8 encoding!");
        return output;
    }
};
