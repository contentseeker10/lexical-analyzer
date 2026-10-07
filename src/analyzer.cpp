#include "analyzer.hpp"

#include <string>

std::ostream& operator<<(std::ostream& os, const Token& val) {
    std::string_view type;
    switch (val.token) {
        case TokenType::Identifier:
            type = "Identifier"; break;
        case TokenType::Number:
            type = "Number"; break;
        case TokenType::Operator:
            type = "Operator"; break;
        case TokenType::Function:
            type = "Function (embedded)"; break;
        case TokenType::Keyword:
            type = "Keyword (reserved word)"; break;
        case TokenType::Undefined:
            type = "Undefined"; break;
        default: break;
    }
    os << "<" << type << ", " << '"' << val.str << '"' << '>';
    return os;
}

Analyzer::Analyzer(const std::string_view& source) noexcept : src(source) {
    std::cout << "Analyzer initialized with source:\n" << source << std::endl;
}

Token Analyzer::next_token() noexcept {
    skip_whitespace();
    if (it >= src.size()) return { TokenType::End, "" };

    size_t begin = it;
    wchar_t c = advance();

    switch (c) {
        case '+': case '-': case '*': case '/': case '%': case '(': case ')':
            return { TokenType::Operator, src.substr(begin, 1) };
    }
    
    if (std::isdigit(c)) {
        while (std::isdigit(peek())) advance();
        if (peek() == '.') {
            advance();
            while (std::isdigit(peek())) advance();
        }
        return { TokenType::Number, src.substr(begin, it - begin) };
    }

    if (isalpha(c)) {
        while (isalpha(peek()) || std::isdigit(peek())) advance();
        return { TokenType::Identifier, src.substr(begin, it - begin) };
    }

    while (!is_delimiter(peek())) advance();

    return { TokenType::Undefined, src.substr(begin, it - begin) };
}