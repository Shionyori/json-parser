#include "json_value.h"
#include <iostream>

JsonValue::JsonValue() : type(JsonValueType::Null) {}

JsonValue::JsonValue(bool value) : type(JsonValueType::Boolean)
{
    data.boolean_value = value;
}

JsonValue::JsonValue(double value) : type(JsonValueType::Number)
{
    data.number_value = value;
}

JsonValue::JsonValue(const std::string& value) : type(JsonValueType::String)
{
    data.string_value = new std::string(value);
}

JsonValue::JsonValue(const std::vector<JsonValue>& value) : type(JsonValueType::Array)
{
    data.array_value = new std::vector<JsonValue>(value);
}

JsonValue::JsonValue(const std::map<std::string, JsonValue>& value) : type(JsonValueType::Object)
{
    data.object_value = new std::map<std::string, JsonValue>(value);
}

JsonValue::~JsonValue()
{
    switch (type)
    {
    case JsonValueType::String:
        delete data.string_value;
        break;
    case JsonValueType::Array:
        delete data.array_value;
        break;
    case JsonValueType::Object:
        delete data.object_value;
        break;
    default:
        break;
    }
}

JsonValue::JsonValue(const JsonValue& other) : type(other.type)
{
    switch (type)
    {
    case JsonValueType::Boolean:
        data.boolean_value = other.data.boolean_value;
        break;
    case JsonValueType::Number:
        data.number_value = other.data.number_value;
        break;
    case JsonValueType::String:
        data.string_value = new std::string(*other.data.string_value);
        break;
    case JsonValueType::Array:
        data.array_value = new std::vector<JsonValue>(*other.data.array_value);
        break;
    case JsonValueType::Object:
        data.object_value = new std::map<std::string, JsonValue>(*other.data.object_value);
        break;
    default:
        break;
    }
}

JsonValue& JsonValue::operator=(const JsonValue& other)
{
    if (this == &other)
        return *this;

    // Clean up existing data
    this->~JsonValue();

    type = other.type;
    switch (type)
    {
    case JsonValueType::Boolean:
        data.boolean_value = other.data.boolean_value;
        break;
    case JsonValueType::Number:
        data.number_value = other.data.number_value;
        break;
    case JsonValueType::String:
        data.string_value = new std::string(*other.data.string_value);
        break;
    case JsonValueType::Array:
        data.array_value = new std::vector<JsonValue>(*other.data.array_value);
        break;
    case JsonValueType::Object:
        data.object_value = new std::map<std::string, JsonValue>(*other.data.object_value);
        break;
    default:
        break;
    }
    return *this;
}

void JsonValue::print(int indent) const 
{
    std::string pad(indent * 2, ' ');  // 缩进
    switch (type) {
        case JsonValueType::Null:
            std::cout << "null";
            break;
        case JsonValueType::Boolean:
            std::cout << (data.boolean_value ? "true" : "false");
            break;
        case JsonValueType::Number:
            std::cout << data.number_value;
            break;
        case JsonValueType::String:
            std::cout << "\"" << *data.string_value << "\"";
            break;
        case JsonValueType::Array:
            std::cout << "[\n";
            for (size_t i = 0; i < data.array_value->size(); ++i) {
                std::cout << pad << "  ";
                (*data.array_value)[i].print(indent + 1);
                if (i < data.array_value->size() - 1) {
                    std::cout << ",";
                }
                std::cout << "\n";
            }
            std::cout << pad << "]";
            break;
        case JsonValueType::Object:
            std::cout << "{\n";
            size_t count = 0;
            for (const auto& pair : *data.object_value) {
                std::cout << pad << "  \"" << pair.first << "\": ";
                pair.second.print(indent + 1);
                if (count < data.object_value->size() - 1) {
                    std::cout << ",";
                }
                std::cout << "\n";
                ++count;
            }
            std::cout << pad << "}";
            break;
    }
}