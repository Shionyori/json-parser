#include "tokenizer.h"

Tokenizer::Tokenizer(const std::string& input) : input(input), position(0) {}

std::vector<Tokenizer::Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while (position < input.size()) {
        skipWhitespace();
        if (position >= input.size()) break;

        startToken();

        char current = peek();
        switch (current) {
            case '{':
                tokens.emplace_back(TokenType::LeftBrace);
                get();
                break;
            case '}':
                tokens.emplace_back(TokenType::RightBrace);
                get();
                break;
            case '[':
                tokens.emplace_back(TokenType::LeftBracket);
                get();
                break;
            case ']':
                tokens.emplace_back(TokenType::RightBracket);
                get();
                break;
            case ':':
                tokens.emplace_back(TokenType::Colon);
                get();
                break;
            case ',':
                tokens.emplace_back(TokenType::Comma);
                get();
                break;
            case '"':
                tokens.push_back(parseString());
                break;
            default:
                if (isdigit(current) || current == '-') {
                    tokens.push_back(parseNumber());
                } else if (input.compare(position, 4, "true") == 0) {
                    tokens.emplace_back(TokenType::True);
                    position += 4;
                } else if (input.compare(position, 5, "false") == 0) {
                    tokens.emplace_back(TokenType::False);
                    position += 5;
                } else if (input.compare(position, 4, "null") == 0) {
                    tokens.emplace_back(TokenType::Null);
                    position += 4;
                } else {
                    throw std::runtime_error(
                        "Error at line " + std::to_string(cur_line) + 
                        ", column " + std::to_string(cur_column) +
                        ": Unexpected character '" + std::string(1, current) + "'"
                    );
                }
        }
    }
    return tokens;
}

char Tokenizer::peek() const {
    return input[position];
}

char Tokenizer::get() {
    if (position >= input.size()) return '\0';
    char current = input[position++];
    if (current == '\n') {
        cur_line++;
        cur_column = 1;
    } else {
        cur_column++;
    }
    return current;
}

void Tokenizer::skipWhitespace() {
    while (position < input.size() && isspace(peek())) {
        get();
    }
}

Tokenizer::Token Tokenizer::parseString() {
    std::string result;
    get(); // Skip opening quote
    while (position < input.size()) {
        char current = get();
        if (current == '"') {
            break; // End of string
        } else if (current == '\\') {
            if (position >= input.size()) {
                throw std::runtime_error(
                    "Error at line " + std::to_string(cur_line) + 
                    ", column " + std::to_string(cur_column) +
                    ": Unexpected end of input in string literal"
                );
            }
            char escape = get();
            switch (escape) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default:
                    throw std::runtime_error(
                        "Error at line " + std::to_string(cur_line) + 
                        ", column " + std::to_string(cur_column) +
                        ": Invalid escape sequence '\\" + std::string(1, escape) + "'"
                    );
            }
        } else {
            result += current;
        }
    }
    return Token(TokenType::String, result);
}

Tokenizer::Token Tokenizer::parseNumber() {
    size_t start = position;
    if (peek() == '-') {
        get();
    }
    if (position >= input.size() || !isdigit(peek())) {
        throw std::runtime_error(
            "Error at line " + std::to_string(cur_line) + 
            ", column " + std::to_string(cur_column) +
            ": Invalid number format"
        );
    }
    while (position < input.size() && isdigit(peek())) {
        get();
    }
    if (position < input.size() && peek() == '.') {
        get();
        if (position >= input.size() || !isdigit(peek())) {
            throw std::runtime_error(
                "Error at line " + std::to_string(cur_line) + 
                ", column " + std::to_string(cur_column) +
                ": Invalid number - missing digits after decimal point"
            );
        }
        while (position < input.size() && isdigit(peek())) {
            get();
        }
    }
    if (position < input.size() && (peek() == 'e' || peek() == 'E')) {
        get();
        if (position < input.size() && (peek() == '+' || peek() == '-')) {
            get();
        }
        if (position >= input.size() || !isdigit(peek())) {
            throw std::runtime_error(
                "Error at line " + std::to_string(cur_line) + 
                ", column " + std::to_string(cur_column) +
                ": Invalid number - missing digits in exponent"
            );
        }
        while (position < input.size() && isdigit(peek())) {
            get();
        }
    }
    return Token(TokenType::Number, input.substr(start, position - start));
}

void Tokenizer::startToken() {
    token_start_line = cur_line;
    token_start_column = cur_column;
}