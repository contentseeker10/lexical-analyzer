#pragma once

#include <string_view>
#include <iostream>

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
    std::string_view _src;

    size_t _begin {};
    size_t _curr {};

    using char_utf = uint16_t;

    bool is_at_end() const noexcept {
        return _curr >= _src.length();
    }

    char_utf advance() noexcept {
        return _src.at(_curr++);
    }

    bool match(char_utf expected) noexcept {
        if (is_at_end()) return false;
        if (_src.at(_curr) != expected) return false;
        _curr++;
        return true;
    }

    bool isalpha(char_utf c) const noexcept;

    bool isdelim(char_utf c) const noexcept;

    void skip_whitespace() noexcept;
};