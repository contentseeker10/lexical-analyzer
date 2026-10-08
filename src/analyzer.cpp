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

Analyzer::Analyzer(const std::string_view& source) noexcept : _src(source) {
    std::cout << "Analyzer initialized with source:\n" << source << std::endl;
}

Token Analyzer::next_token() noexcept {
    char_utf c = advance();

    // Operators
    switch (c) {
        case '+': return { TokenType::Operator, "+" }; break;
        case '-': return { TokenType::Operator, "-" }; break;
        case '*': return { TokenType::Operator, "*" }; break;
        case '/': return { TokenType::Operator, "/" }; break;
        case '%': return { TokenType::Operator, "%" }; break;
        default: break;
    }

    return { TokenType::Undefined, "" };
}