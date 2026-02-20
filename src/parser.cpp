#include "parser.h"

Parser::Parser(const std::vector<Tokenizer::Token>& tokens) : tokens(tokens), position(0) {}

JsonValue Parser::parse() {
    return parseValue();
}

Tokenizer::Token Parser::peek() const {
    return tokens[position];
}

Tokenizer::Token Parser::get() {
    return tokens[position++];
}

JsonValue Parser::parseValue() {
    Tokenizer::Token token = peek();
    switch (token.type) {
        case Tokenizer::TokenType::LeftBrace:
            return parseObject();
        case Tokenizer::TokenType::LeftBracket:
            return parseArray();
        case Tokenizer::TokenType::String:
            return parseString();
        case Tokenizer::TokenType::Number:
            return parseNumber();
        case Tokenizer::TokenType::True:
            return parseTrue();
        case Tokenizer::TokenType::False:
            return parseFalse();
        case Tokenizer::TokenType::Null:
            return parseNull();
        default:
            throw std::runtime_error("Unexpected token: " + token.value);
    }
}

JsonValue Parser::parseObject() {
    std::map<std::string, JsonValue> object;
    get(); // Skip '{'
    while (peek().type != Tokenizer::TokenType::RightBrace) {
        if (peek().type != Tokenizer::TokenType::String) {
            throw std::runtime_error("Expected string key in object");
        }
        std::string key = get().value;
        if (get().type != Tokenizer::TokenType::Colon) {
            throw std::runtime_error("Expected ':' after key in object");
        }
        JsonValue value = parseValue();
        object[key] = value;

        if (peek().type == Tokenizer::TokenType::Comma) {
            get(); // Skip ','
        } else if (peek().type != Tokenizer::TokenType::RightBrace) {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }
    get(); // Skip '}'
    return JsonValue(object);
}

JsonValue Parser::parseArray() {
    std::vector<JsonValue> array;
    get(); // Skip '['
    while (peek().type != Tokenizer::TokenType::RightBracket) {
        array.push_back(parseValue());
        if (peek().type == Tokenizer::TokenType::Comma) {
            get(); // Skip ','
        } else if (peek().type != Tokenizer::TokenType::RightBracket) {
            throw std::runtime_error("Expected ',' or ']' in array");
        }
    }
    get(); // Skip ']'
    return JsonValue(array);
}

JsonValue Parser::parseString() {
    return JsonValue(get().value);
}

JsonValue Parser::parseNumber() {
    return JsonValue(std::stod(get().value));
}

JsonValue Parser::parseTrue() {
    get(); // Skip 'true'
    return JsonValue(true);
}

JsonValue Parser::parseFalse() {
    get(); // Skip 'false'
    return JsonValue(false);
}

JsonValue Parser::parseNull() {
    get(); // Skip 'null'
    return JsonValue();
}
