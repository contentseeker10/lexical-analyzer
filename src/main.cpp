#include "analyzer.hpp"

#include <Windows.h>

using std::cout, std::endl;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    cout << "Stepanov Lexical Analyzer launched" << endl;

    vs::Analyzer l(L"бебра");

    cout << l.next_token() << endl;
    cout << l.next_token() << endl;
    // cout << l.next_token() << endl;
    // cout << l.next_token() << endl;
    // cout << l.next_token() << endl;

    return 0;
}