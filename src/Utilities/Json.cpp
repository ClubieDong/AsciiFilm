#include "Json.hpp"
#include "stdexcept"

using jt = json::value_t;

inline bool CheckType(jt expect, jt actual)
{
    return actual == expect ||
           (expect == jt::number_float && (actual == jt::number_integer ||
                                           actual == jt::number_unsigned)) ||
           (expect == jt::number_integer && actual == jt::number_unsigned);
}

inline const char *GetTypeName(jt type)
{
    constexpr const char *TYPE_NAME[] =
        {"null", "object", "array", "string", "boolean",
         "integer", "unsigned integer", "float"};
    auto idx = static_cast<std::uint8_t>(type);
    return idx < 8 ? TYPE_NAME[idx] : "unknown";
}

void JsonValidator::Validate(json &data, const std::string &location) const
{
    // `data` itself must be an object
    if (data.type() != jt::object)
        throw std::invalid_argument("Expect " + location + " to be object" +
                                    ", but got " + GetTypeName(data.type()));
    std::string errMsg;
    for (const auto &[field, type] : _RequiredItemList)
    {
        auto iter = data.find(field);
        if (iter == data.end())
            errMsg += "Expect \"" + field + "\" field in " + location + '\n';
        else if (!CheckType(type, iter->type()))
            errMsg += "Expect " + location + '.' + field +
                      " to be " + GetTypeName(type) +
                      ", but got " + GetTypeName(iter->type()) + '\n';
    }
    for (const auto &[field, type, defaultValue] : _OptionalItemList)
    {
        auto iter = data.find(field);
        if (iter == data.end())
            data[field] = defaultValue;
        else if (!CheckType(type, iter->type()))
            errMsg += "Expect " + location + '.' + field +
                      " to be " + GetTypeName(type) +
                      ", but got " + GetTypeName(iter->type()) + '\n';
    }
    if (!errMsg.empty())
    {
        // remove trailing '\n'
        errMsg.pop_back();
        throw std::invalid_argument(errMsg);
    }
}
