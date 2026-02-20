#include "json_value.h"
#include <iostream>
#include <sstream>

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
    std::string pad(indent * 2, ' ');
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

static std::string escape_string(const std::string& s) {
    std::string result;
    for (char c : s) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '/': result += "\\/"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[7];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    result += buf;
                } else {
                    result += c;
                }
        }
    }
    return result;
}

std::string JsonValue::dump(int indent) const {
    return dump_impl(indent, 0);  // Second parameter is current depth, initially 0
}

std::string JsonValue::dump_impl(int indent, int depth) const {
    std::string pad(depth * indent, ' ');
    switch (type) {
        case JsonValueType::Null:
            return "null";
        case JsonValueType::Boolean:
            return data.boolean_value ? "true" : "false";
        case JsonValueType::Number:
            // return std::to_string(data.number_value);
            // to_string may add trailing zeros, better use snprintf
            {
                char buf[32];
                snprintf(buf, sizeof(buf), "%g", data.number_value);
                return std::string(buf);
            }
        case JsonValueType::String:
            return "\"" + escape_string(*data.string_value) + "\"";
        case JsonValueType::Array: {
            if (data.array_value->empty()) return "[]";
            
            std::string newline = (indent >= 0) ? "\n" : "";
            std::string child_pad = (indent >= 0) ? std::string(indent, ' ') : "";
            
            std::string result = "[" + newline;
            for (size_t i = 0; i < data.array_value->size(); ++i) {
                if (indent >= 0) result += pad + child_pad;
                result += (*data.array_value)[i].dump_impl(indent, depth + 1);
                if (i < data.array_value->size() - 1) result += "," + newline;
                else result += newline;
            }
            if (indent >= 0) result += pad;
            result += "]";
            return result;
        }
        case JsonValueType::Object: {
            if (data.object_value->empty()) return "{}";
            
            std::string newline = (indent >= 0) ? "\n" : "";
            std::string child_pad = (indent >= 0) ? std::string(indent, ' ') : "";
            
            std::string result = "{" + newline;
            size_t count = 0;
            for (const auto& pair : *data.object_value) {
                if (indent >= 0) result += pad + child_pad;
                result += "\"" + escape_string(pair.first) + "\": " + pair.second.dump_impl(indent, depth + 1);
                if (count < data.object_value->size() - 1) result += "," + newline;
                else result += newline;
                ++count;
            }
            if (indent >= 0) result += pad;
            result += "}";
            return result;
        }
    }
    return "null";
}