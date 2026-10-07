#include "analyzer.hpp"

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

// Token Analyzer::next_token() noexcept {

// }