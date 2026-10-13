#include "analyzer.hpp"

#include <Windows.h>

namespace vs {

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
        std::cout << "Analyzer initialized" << std::endl;
    }

    Token Analyzer::next_token() noexcept {
        skip_whitespace();

        wchar_t c = peek();

        // Operators
        switch (c) {
            case L'+': return consume_op(c); break;
            case L'-': return consume_op(c); break;
            case L'*': return consume_op(c); break;
            case L'/': return consume_op(c); break;
            case L'%': return consume_op(c); break;
            case L'=': return consume_op(c); break;
            default: break;
        }

        // Numbers
        if (std::iswdigit(c)) {
            while (!is_at_end() && std::iswdigit(peek())) advance();
            if (match(L'.')) {
                advance();
                if (std::iswdigit(peek())) {
                    while (!is_at_end() && std::iswdigit(peek())) advance();
                } else {
                    std::string str = wide_to_utf8(_src.substr(_begin, _curr - _begin));
                    reset_ptr();
                    return { TokenType::Undefined, str };
                }
            }
            std::string str = wide_to_utf8(_src.substr(_begin, _curr - _begin));
            reset_ptr();
            return { TokenType::Number, str };
        }

        // Identifiers
        if (is_id(c)) {
            while (!is_at_end() && (is_id(peek()) || std::iswdigit(peek()))) advance();
            std::string str = wide_to_utf8(_src.substr(_begin, _curr - _begin));
            reset_ptr();
            return { TokenType::Identifier, str };
        }

        return { TokenType::Undefined, "" };
    }

}