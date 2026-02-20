#pragma once

#include <string>
#include <vector>
#include <map>

enum class JsonValueType
{
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object
};

class JsonValue
{
    JsonValueType type;
    union
    {
        bool boolean_value;
        double number_value;
        std::string* string_value;
        std::vector<JsonValue>* array_value;
        std::map<std::string, JsonValue>* object_value;
    } data;

public:
    JsonValue();
    JsonValue(bool value);
    JsonValue(double value);
    JsonValue(const std::string& value);
    JsonValue(const std::vector<JsonValue>& value);
    JsonValue(const std::map<std::string, JsonValue>& value);
    JsonValue(const JsonValue& other); // 拷贝构造
    JsonValue& operator=(const JsonValue& other); // 拷贝赋值
    ~JsonValue();

    void print(int indent = 0) const;
};