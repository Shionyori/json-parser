#pragma once

#include "tokenizer.h"
#include "json_value.h"
#include "json_exception.h"
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Tokenizer::Token>& tokens);
    JsonValue parse();

private:
    std::vector<Tokenizer::Token> tokens;
    size_t position;

    Tokenizer::Token peek() const;
    Tokenizer::Token get();

    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseString();
    JsonValue parseNumber();
    JsonValue parseTrue();
    JsonValue parseFalse();
    JsonValue parseNull();
};