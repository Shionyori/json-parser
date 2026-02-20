# pragma once

#include <stdexcept>
#include <string>

class JsonException : public std::runtime_error {
public:
    JsonException(const std::string& message, size_t line, size_t column)
        : std::runtime_error(formatMessage(message, line, column)), line(line), column(column) {}

    size_t getLine() const { return line; }
    size_t getColumn() const { return column; }

private:
    size_t line;
    size_t column;    

    static std::string formatMessage(const std::string& message, size_t line, size_t column) {
        return "Error at line " + std::to_string(line) + ", column " + std::to_string(column) + ": " + message;
    }
};