#pragma once

#include <string>
#include <iostream>
#include <cwctype>
#include <map>

namespace vs {

    enum class TokenType {
        End = -1, Undefined, Identifier, StringLiteral, Number, Operator, Function, Keyword
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

        std::map<std::string, bool> _keywords;
        std::map<std::string, bool> _std_functions;

        void _insert_keywords()     noexcept;
        void _insert_std_function() noexcept;

        bool _is_at_end()  const noexcept { return _curr >= _src.length(); }

        wchar_t _peek()    const noexcept { return _src[_curr]; }
        wchar_t _advance()       noexcept { return _src[_curr++]; }

        void _reset_ptr() noexcept { _begin = _curr; }

        bool _match(wchar_t expected) const noexcept {
            if (_is_at_end()) return false;
            if (_peek() != expected) return false;
            return true;
        }

        bool _is_invalid_id(wchar_t c) const noexcept {
            return !_is_id(c) && !std::iswdigit(c) && !_is_op(c) && !_is_at_end() && !std::iswspace(c);
        }

        bool _is_op(wchar_t c) const noexcept {
            return _match(L'+') || _match(L'-') || _match(L'*') || _match(L'/') || _match(L'%') || _match(L'=');
        }

        bool _is_id(wchar_t c) const noexcept {
            return _match(L'с') || _match(L'т') || _match(L'е') || _match(L'п') 
                || _match(L'а') || _match(L'н') || _match(L'о') || _match(L'в') 
                || _match(L'С') || _match(L'Т') || _match(L'Е') || _match(L'П') 
                || _match(L'А') || _match(L'Н') || _match(L'О') || _match(L'В');
        }

        void _skip_rest() noexcept {
            while (!_is_at_end() && !std::iswspace(_peek())) _advance();
        }

        void _skip_whitespace() noexcept { 
            while (std::iswspace(_peek())) _advance();
            _reset_ptr();
        }

        Token _consume_op(wchar_t c) noexcept {
            _advance();
            _reset_ptr();
            std::wstring wstr = L"";
            wstr.push_back(c);
            std::string str = wide_to_utf8(wstr);
            return { TokenType::Operator, str };
        }

        std::string _get_curr_str() noexcept {
            std::string str = wide_to_utf8(_src.substr(_begin, _curr - _begin));
            _reset_ptr();
            return str;
        }
    };

}