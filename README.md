# lexical-analyzer

Basic Lexical Analyzer for a set of C-like programming language keywords. 

## Input Requirements

- **Identifiers:** [\dстепановСТЕПАНОВ] => *digits and ukrainian letters from my surname (Stepanov)*
- **Numbers:** [+-]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)? => *real numbers*
- **Ariphmetic operators:** [-+*/%]
- **Embedded functions:** (sin|cos|tan|cot|pow|sqrt)
- **Keywords:** (if|else|while|do|void|const)