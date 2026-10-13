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
        wchar_t c = advance();

        // Operators
        switch (c) {
            case L'+': return { TokenType::Operator, "+" }; break;
            case L'-': return { TokenType::Operator, "-" }; break;
            case L'*': return { TokenType::Operator, "*" }; break;
            case L'/': return { TokenType::Operator, "/" }; break;
            case L'%': return { TokenType::Operator, "%" }; break;
            default: break;
        }

        // Numbers
        if (std::isdigit(c)) {
            while (!is_at_end() && std::isdigit(advance()));
            if (match(L'.')) {
                if (std::isdigit(advance())) {
                    while (!is_at_end() && std::isdigit(advance()));
                }
            }
            return { TokenType::Number, wide_to_utf8(_src.substr(_begin, _curr - 1)) };
        }

        return { TokenType::Undefined, "" };
    }

}