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
        size_t line;
        size_t column;

        Token(TokenType t, const std::string& v = "", size_t l = 1, size_t c = 1) 
            : type(t), value(v), line(l), column(c) {}
    };

    explicit Tokenizer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t position;
    size_t cur_line;
    size_t cur_column;
    size_t token_start_line;
    size_t token_start_column;

    char peek() const;
    char get();
    void startToken();
    void skipWhitespace();
    Token parseString();
    Token parseNumber();
};