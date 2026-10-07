#pragma once

#include <string_view>
#include <iostream>
#include <cwctype>

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
private:
    std::string_view src;
    size_t it {};

    wchar_t peek()    const noexcept { return it < src.size() ? src[it]   : '\0'; };
    wchar_t advance()       noexcept { return it < src.size() ? src[it++] : '\0'; };

    bool isalpha(wchar_t c) const noexcept {
        c = std::towlower(c); 
        return c == 'с' || c == 'т' || c == 'е' || c == 'п' 
            || c == 'а' || c == 'н' || c == 'о' || c == 'в'; 
    }

    bool is_delimiter(wchar_t c) noexcept {
        return std::iswspace(c) 
            || c == '+' || c == '-' || c == '*' || c == '/' 
            || c == '%' || c == '(' || c == ')' || c == '\0'; 
    }

    void skip_whitespace() noexcept {
        while (std::iswspace(peek())) advance();
    };  
};