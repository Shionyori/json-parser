#pragma once

#include <string>
#include <vector>
#include <stdexcept>

class Tokenizer {
public:
    enum class TokenType {
        LeftBrace,   // {
        RightBrace,  // }
        LeftBracket, // [
        RightBracket,// ]
        Colon,       // :
        Comma,       // ,
        String,      // "..."
        Number,      // 123, -123.45
        True,        // true
        False,       // false
        Null         // null
    };

    struct Token {
        TokenType type;
        std::string value;

        Token(TokenType t, const std::string& v = "") : type(t), value(v) {}
    };

    explicit Tokenizer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t position;

    char peek() const;
    char get();
    void skipWhitespace();
    Token parseString();
    Token parseNumber();
};