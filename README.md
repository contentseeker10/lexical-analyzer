# lexical-analyzer

A primitive lexical analyzer based on a Deterministic Finite Automaton.

## Technical Specification

- Implemented in C++20
- Built with CMake 4.3
- Uses the Windows API library

## Project Structure

```
lexical-analyzer/
├── bin/
|   └── LexicalAnalyzer.exe # Compiled executable file
├── example/ 
|   ├── example.png         # Example execution screenshot
|   └── pseudocode.txt      # Pseudocode example for lexical analysis
├── include/
|   └── analyzer.hpp        # Analyzer interface
├── src/
|   ├── analyzer.cpp        # Analyzer implementation
|   └── main.cpp            # Program entry point
└── CMakeLists.txt          # CMake configuration
```

## Token Recognition Capabilities

1. **Identifier** - Identifiers composed of characters from the Ukrainian alphabet (from my surname Stepanov) and digits
2. **StringLiteral** - String literal containing any characters
3. **Number** - Integer and floating-point numbers
4. **Operator** - Arithmetic operators
5. **Keyword** - Reserved keywords (if, else, while...)
6. **Function** - Standard built-in functions (sin, cos, pow...)
7. **Undefined** - Malformed token
8. **End** - End of text

The program reads text from a file and retrieves and outputs each token until it reaches the end:
```cpp
vs::Token t;
while (t.token != vs::TokenType::End) {
    t = l.next_token();
    cout << t << endl;
}
```

All token types except Keyword and Function are processed using a DFA. The latter are identified by lookup in a `std::map` populated with a set of keywords and standard functions.

Program execution demonstration using the example from *example/pseudocode.txt*:

<img src="example/example.png" width="50%" alt="Screenshot of example">

The program is implemented specifically for Windows OS - it uses the `wchar_t` type to handle Cyrillic characters and performs bidirectional string conversion.

## Compilation and Execution

To build the project, you can use **CMake 4.3+**:

```ps1
cmake -B out -S .
cmake --build out --config Debug
```

The pseudocode file can be placed in the same directory as the compiled program:

```ps1
cd .\out\Debug\
.\LexicalAnalyzer.exe <filename>
```