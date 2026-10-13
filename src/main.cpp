#include "analyzer.hpp"

#include <Windows.h>

using std::cout, std::endl;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    cout << "Stepanov Lexical Analyzer launched" << endl;

    vs::Analyzer l(L"2228.25*4");

    cout << l.next_token() << endl;

    return 0;
}