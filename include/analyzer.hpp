#pragma once

#include <string>
#include <iostream>
#include <cwctype>

namespace vs {

    enum class TokenType {
        Undefined, Identifier, Number, Operator, Function, Keyword, End
    };

    struct Token final {
        TokenType token = TokenType::Undefined;
        std::string str;
    };

    std::ostream& operator<<(std::ostream&, const Token&);
    std::string wide_to_utf8(const std::wstring_view&);

    class Analyzer final {
    public:
        explicit Analyzer(const std::wstring&) noexcept;

        Token next_token() noexcept;
    private:
        std::wstring _src;

        size_t _begin {};
        size_t _curr {};

        bool is_at_end() const noexcept {
            return _curr >= _src.length();
        }

        wchar_t advance() noexcept {
            return _src[_curr++];
        }

        bool match(wchar_t expected) noexcept {
            if (is_at_end()) return false;
            if (_src[_curr-1] != expected) return false;
            _curr++;
            return true;
        }

        bool isalpha(wchar_t c) const noexcept;

        bool isdelim(wchar_t c) const noexcept;

        void skip_whitespace() noexcept;
    };

}