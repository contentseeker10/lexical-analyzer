#include "analyzer.hpp"

#include <Windows.h>

namespace vs {

    std::ostream& operator<<(std::ostream& os, const Token& val) {
        std::string_view type;
        switch (val.token) {
            case TokenType::End:
                type = "End"; break;
            case TokenType::Undefined:
                type = "Undefined"; break;
            case TokenType::Identifier:
                type = "Identifier"; break;
            case TokenType::StringLiteral:
                type = "String literal"; break;
            case TokenType::Number:
                type = "Number"; break;
            case TokenType::Operator:
                type = "Operator"; break;
            case TokenType::Function:
                type = "Std function"; break;
            case TokenType::Keyword:
                type = "Keyword"; break;
            default: break;
        }
        os << "<" << type << ", " << '"' << val.str << '"' << '>';
        return os;
    }

    std::string wide_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return {};

        int size_needed = WideCharToMultiByte(
            CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
            nullptr, 0, nullptr, nullptr
        );
        if (size_needed <= 0) return {};
        std::string result(size_needed, 0);
        WideCharToMultiByte(
            CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
            result.data(), size_needed, nullptr, nullptr
        );
        return result;
    }

    Analyzer::Analyzer(const std::wstring& source) noexcept : _src(source) {
        _insert_keywords();
        _insert_std_function();
        std::cout << "Analyzer initialized" << std::endl;
    }

    void Analyzer::_insert_keywords() noexcept {
        _keywords.insert({"if", true});
        _keywords.insert({"else", true});
        _keywords.insert({"while", true});
        _keywords.insert({"do", true});
        _keywords.insert({"void", true});
        _keywords.insert({"const", true});
    }

    void Analyzer::_insert_std_function() noexcept {
        _std_functions.insert({"sin", true});
        _std_functions.insert({"cos", true});
        _std_functions.insert({"tan", true});
        _std_functions.insert({"cot", true});
        _std_functions.insert({"pow", true});
        _std_functions.insert({"sqrt", true});
    }

    Token Analyzer::next_token() noexcept {
        _skip_whitespace();
        wchar_t c = _peek();

        // End token
        if (_is_at_end()) {
            return { TokenType::End, "" };
        }

        // Operator token
        if (_is_op(c)) {
            return _consume_op(c);
        }

        // String literal token
        if (_match(L'\"')) {
            TokenType type = TokenType::StringLiteral;
            _advance();
            while (!_is_at_end() || _match(L'\"')) _advance();
            if (_peek() != L'\"') type = TokenType::Undefined;
            return { type, _get_curr_str() };
        }

        // Number token
        if (std::iswdigit(c)) {
            TokenType type = TokenType::Number;
            while (!_is_at_end() && std::iswdigit(_peek())) _advance();
            if (_match(L'.')) {
                _advance();
                if (!_is_at_end() && std::iswdigit(_peek())) {
                    while (!_is_at_end() && std::iswdigit(_peek())) _advance();
                } else type = TokenType::Undefined;
            }
            if (!std::iswspace(_peek()) && !_is_at_end() && !_is_op(_peek())) {
                type = TokenType::Undefined;
                _skip_rest();
            }
            return { type, _get_curr_str() };
        }

        // Identifier token
        if (_is_id(c)) {
            TokenType type = TokenType::Identifier;
            while (!_is_at_end() && (_is_id(_peek()) || std::iswdigit(_peek()))) _advance();
            if (_is_invalid_id(_peek())) {
                type = TokenType::Undefined;
                _skip_rest();
            }
            return { type, _get_curr_str() };
        }

        _skip_rest();
        std::string str = _get_curr_str();

        // Keyword token
        if (_keywords.contains(str)) return { TokenType::Keyword, str };
        
        // Std function token
        if (_std_functions.contains(str)) return { TokenType::Function, str };

        // Undefined token
        return { TokenType::Undefined, str };
    }

}