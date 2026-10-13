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
        insert_keywords();
        insert_std_function();
        std::cout << "Analyzer initialized" << std::endl;
    }

    void Analyzer::insert_keywords() noexcept {
        _keywords.insert({"if", true});
        _keywords.insert({"else", true});
        _keywords.insert({"while", true});
        _keywords.insert({"do", true});
        _keywords.insert({"void", true});
        _keywords.insert({"const", true});
    }

    void Analyzer::insert_std_function() noexcept {
        _std_functions.insert({"sin", true});
        _std_functions.insert({"cos", true});
        _std_functions.insert({"tan", true});
        _std_functions.insert({"cot", true});
        _std_functions.insert({"pow", true});
        _std_functions.insert({"sqrt", true});
    }

    Token Analyzer::next_token() noexcept {
        skip_whitespace();
        wchar_t c = peek();

        // End token
        if (is_at_end()) {
            return { TokenType::End, "" };
        }

        // Operator token
        switch (c) {
            case L'+': return consume_op(c); break;
            case L'-': return consume_op(c); break;
            case L'*': return consume_op(c); break;
            case L'/': return consume_op(c); break;
            case L'%': return consume_op(c); break;
            case L'=': return consume_op(c); break;
            default: break;
        }

        // String literal token
        if (match(L'\"')) {
            advance();
            while (!is_at_end()) {
                if (match(L'\"')) {
                    advance();
                    break;
                }
                advance();
            }
            return { TokenType::StringLiteral, get_curr_str() };
        }

        // Number token
        if (std::iswdigit(c)) {
            while (!is_at_end() && std::iswdigit(peek())) advance();
            if (match(L'.')) {
                advance();
                if (!is_at_end() && std::iswdigit(peek())) {
                    while (!is_at_end() && std::iswdigit(peek())) advance();
                } else return { TokenType::Undefined, get_curr_str() };
            }
            return { TokenType::Number, get_curr_str() };
        }

        // Identifier token
        if (is_id(c)) {
            while (!is_at_end() && (is_id(peek()) || std::iswdigit(peek()))) advance();
            return { TokenType::Identifier, get_curr_str() };
        }

        while (!is_at_end() && !std::iswspace(peek())) advance();
        std::string str = get_curr_str();

        // Keyword token
        if (_keywords.contains(str)) return { TokenType::Keyword, str };
        
        // Std function token
        if (_std_functions.contains(str)) return { TokenType::Function, str };

        // Undefined token
        return { TokenType::Undefined, get_curr_str() };
    }

}