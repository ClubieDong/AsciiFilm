#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonValidator
{
private:
    struct OptionalItem
    {
        std::string Field;
        json::value_t Type;
        json DefaultValue;

        inline explicit OptionalItem(std::string_view field, json::value_t type, const json &defaultValue)
            : Field(field), Type(type), DefaultValue(defaultValue) {}
    };
    struct RequiredItem
    {
        std::string Field;
        json::value_t Type;

        inline explicit RequiredItem(std::string_view field, json::value_t type)
            : Field(field), Type(type) {}
    };

    std::vector<OptionalItem> _OptionalItemList;
    std::vector<RequiredItem> _RequiredItemList;

public:
    inline JsonValidator& AddOptional(std::string_view field, json::value_t type, const json &defaultValue)
    {
        _OptionalItemList.emplace_back(field, type, defaultValue);
        return *this;
    }
    inline JsonValidator& AddRequired(std::string_view field, json::value_t type)
    {
        _RequiredItemList.emplace_back(field, type);
        return *this;
    }

    void Validate(json &data, const std::string &location) const;
};
