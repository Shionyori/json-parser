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
    JsonValue(const JsonValue& other); // copy construct
    JsonValue& operator=(const JsonValue& other); // copy assign
    ~JsonValue();

    JsonValue& operator[] (const std::string& key);
    JsonValue& operator[] (const std::string& key) const;
    JsonValue& operator[] (size_t index);
    JsonValue& operator[] (size_t index) const;

    bool is_null() const { return type == JsonValueType::Null; }
    bool is_boolean() const { return type == JsonValueType::Boolean; }
    bool is_number() const { return type == JsonValueType::Number; }
    bool is_string() const { return type == JsonValueType::String; }
    bool is_array() const { return type == JsonValueType::Array; }
    bool is_object() const { return type == JsonValueType::Object; }

    bool as_boolean() const { return data.boolean_value; }
    double as_double() const { return data.number_value; }
    int as_int() const { return static_cast<int>(data.number_value); }
    const std::string& as_string() const { return *data.string_value; }
    const std::vector<JsonValue>& as_array() const { return *data.array_value; }
    const std::map<std::string, JsonValue>& as_object() const { return *data.object_value; }

    void print(int indent = 0) const;
    std::string dump(int indent = 0) const; // -1 compact mode, 0 line break no indent mode, >=1 indent mode
    
private:
    std::string dump_impl(int indent, int depth) const;
};