#pragma once

#include <string_view>
#include <iostream>
#include <cctype>

enum class TokenType {
    Identifier, Number, Operator, Function, Keyword, Undefined, End
};

struct Token final {
    TokenType token = TokenType::Undefined;
    std::string_view str;
};

std::ostream& operator<<(std::ostream&, const Token&);

class Analyzer final {
public:
    explicit Analyzer(const std::string_view&) noexcept;

    Token next_token() noexcept;
// private:
    std::string_view src;
    size_t it {};

    int peek()    const noexcept { return it < src.size() ? src[it]   : '\0'; };
    int advance()       noexcept { return it < src.size() ? src[it++] : '\0'; };

    bool isalpha(int c) const noexcept {
        c = std::tolower(c); 
        return c == 'с' || c == 'т' || c == 'е' || c == 'п' || c == 'а' || c == 'н' || c == 'о' || c == 'в'; 
    }

    void skip_whitespace() noexcept {
        while (std::isspace(peek())) advance();
    };  
};