#include "parser.h"

Parser::Parser(const std::vector<Tokenizer::Token>& tokens) : tokens(tokens), position(0) {}

JsonValue Parser::parse() {
    if (tokens.empty()) {
        throw JsonException("Input is empty", 1, 1);
    }
    return parseValue();

    if (position < tokens.size()) {
        Tokenizer::Token extra = peek();
        throw JsonException("Unexpected token after end of JSON value: " + extra.value, extra.line, extra.column);
    }
}

Tokenizer::Token Parser::peek() const {
    if (position >= tokens.size()) {
        throw JsonException("Unexpected end of input", tokens.back().line, tokens.back().column);
    }
    return tokens[position];
}

Tokenizer::Token Parser::get() {
    if (position >= tokens.size()) {
        throw JsonException("Unexpected end of input", tokens.back().line, tokens.back().column);
    }
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
            throw JsonException("Unexpected token: " + token.value, token.line, token.column);
    }
}

JsonValue Parser::parseObject() {
    std::map<std::string, JsonValue> object;
    get(); // Skip '{'
    while (peek().type != Tokenizer::TokenType::RightBrace) {
        if (peek().type != Tokenizer::TokenType::String) {
            throw JsonException("Expected string key in object", peek().line, peek().column);
        }
        std::string key = get().value;
        if (get().type != Tokenizer::TokenType::Colon) {
            throw JsonException("Expected ':' after key in object", peek().line, peek().column);
        }
        JsonValue value = parseValue();
        object[key] = value;

        if (peek().type == Tokenizer::TokenType::Comma) {
            get(); // Skip ','
        } else if (peek().type != Tokenizer::TokenType::RightBrace) {
            throw JsonException("Expected ',' or '}' in object", peek().line, peek().column);
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
        const auto& next = peek();
        if (next.type == Tokenizer::TokenType::Comma) {
            get(); // Skip ','
        } else if (next.type != Tokenizer::TokenType::RightBracket) {
            throw JsonException("Expected ',' or ']' in array", next.line, next.column);
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
