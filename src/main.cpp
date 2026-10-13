#include "analyzer.hpp"

#include <Windows.h>

using std::cout, std::endl;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    cout << "Stepanov Lexical Analyzer launched" << endl;

    vs::Analyzer l(L"степ123 = sin 2 if pow do пнв + 14 / 2 степіанов = \"Вовчик\"");

    vs::Token t;
    while (t.token != vs::TokenType::End) {
        t = l.next_token();
        cout << t << endl;
    }

    return 0;
}