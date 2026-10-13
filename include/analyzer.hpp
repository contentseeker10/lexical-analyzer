#pragma once

#include <string>
#include <iostream>
#include <cwctype>

namespace vs {

    enum class TokenType {
        End = -1, Undefined, Identifier, Number, Operator, Function, Keyword
    };

    struct Token final {
        TokenType token = TokenType::Undefined;
        std::string str;
    };

    std::ostream& operator<<(std::ostream&, const Token&);
    std::string wide_to_utf8(const std::wstring&);

    class Analyzer final {
    public:
        explicit Analyzer(const std::wstring&) noexcept;

        Token next_token() noexcept;
    private:
        std::wstring _src;

        size_t _begin {};
        size_t _curr {};

        bool is_at_end()  const noexcept { return _curr >= _src.length(); }

        wchar_t peek()    const noexcept { return _src[_curr]; }
        wchar_t advance()       noexcept { return _src[_curr++]; }

        void reset_ptr() noexcept { _begin = _curr; }

        bool match(wchar_t expected) const noexcept {
            if (is_at_end()) return false;
            if (peek() != expected) return false;
            return true;
        }

        bool is_id(wchar_t c) const noexcept {
            return match(L'с') || match(L'т') || match(L'е') || match(L'п') || match(L'а') || match(L'н') || match(L'о') || match(L'в') 
                || match(L'С') || match(L'Т') || match(L'Е') || match(L'П') || match(L'А') || match(L'Н') || match(L'О') || match(L'В');
        }

        void skip_whitespace() noexcept { 
            while (std::iswspace(peek())) advance();
            reset_ptr();
        }

        Token consume_op(wchar_t c) noexcept {
            advance();
            reset_ptr();
            std::wstring wstr = L"";
            wstr.push_back(c);
            std::string str = wide_to_utf8(wstr);
            return { TokenType::Operator, str };
        }

        std::string get_curr_str() noexcept {
            std::string str = wide_to_utf8(_src.substr(_begin, _curr - _begin));
            reset_ptr();
            return str;
        }
    };

}