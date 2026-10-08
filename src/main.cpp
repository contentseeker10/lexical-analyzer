#include "analyzer.hpp"

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif

    std::setlocale(LC_ALL, "ua_UA.UTF-8");

    std::cout << "Stepanov Lexical Analyzer launched" << std::endl;

    Analyzer l("*");

    std::cout << l.next_token() << std::endl;

    return 0;
}