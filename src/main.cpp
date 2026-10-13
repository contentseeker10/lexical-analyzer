#include "analyzer.hpp"

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <limits>
#include <stdexcept>

using std::cout, std::endl;

std::wstring utf8_to_wide(const std::string& src) {
    if (src.empty()) return {};
    if (src.size() > static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
        throw std::length_error("Source file is too large to convert");
    }

    int size_needed = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, src.data(), static_cast<int>(src.size()),
        nullptr, 0
    );
    if (size_needed == 0) {
        throw std::runtime_error("Source file is not valid UTF-8");
    }

    std::wstring result(size_needed, L'\0');
    if (MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, src.data(), static_cast<int>(src.size()),
            result.data(), size_needed
        ) == 0) {
        throw std::runtime_error("Failed to convert source file from UTF-8");
    }
    return result;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
    WORD wOldColorAttrs = csbiInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Stepanov Lexical Analyzer launched" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    if (argc < 2) {
        cout << "Usage: LexicalAnalyzer <filename>" << endl;
        return 1;
    }

    using fs = std::filesystem::path;
    fs path = fs(".") / std::string_view(argv[1]);
    // fs path = fs(".") / std::string_view("pseudocode.txt");

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return 1;
    }

    const std::streamsize size = file.tellg();
    if (size <= 0) {
        cout << "File is empty" << endl;
        return 1;
    }
    file.seekg(0);

    std::string src;
    src.resize(static_cast<std::size_t>(size));
    
    if (!file.read(src.data(), size)) {
        cout << "Error reading file" << endl;
        return 1;
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Source file successfully read" << endl;
    cout << endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << src << endl;
    cout << endl;
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    vs::Analyzer l(utf8_to_wide(src));
    cout << endl;
    
    SetConsoleTextAttribute(hConsole, wOldColorAttrs);
    vs::Token t;
    while (t.token != vs::TokenType::End) {
        t = l.next_token();
        cout << t << endl;
    }

    cout << endl;
    return 0;
}