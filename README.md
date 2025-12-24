# Kaleidoscope

A toy programming language compiler built with LLVM, following the official [LLVM Kaleidoscope Tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html).

## Overview

Kaleidoscope is a simple procedural language that supports:
- Function definitions and declarations
- Basic arithmetic operations
- Function calls
- Variables
- Comments

The compiler generates LLVM IR, which can then be compiled to native machine code.

## Prerequisites

- **LLVM** (version 14 or later recommended)
- **Clang** or **GCC** with C++17 support
- **CMake** (optional, for build automation)

### Installing LLVM

**Ubuntu/Debian:**
```bash
sudo apt-get install llvm llvm-dev clang
```

**macOS (Homebrew):**
```bash
brew install llvm
```

**Fedora:**
```bash
sudo dnf install llvm llvm-devel clang
```

## Building

### Using Clang (Recommended)

```bash
clang++ -g -O3 one.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o kaleidoscope
```

### Using GCC

```bash
g++ -g -O3 one.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o kaleidoscope
```

## Usage

Run the compiler to start the interactive REPL:

```bash
./kaleidoscope
```

You'll see the `ready>` prompt where you can enter Kaleidoscope code.

### Language Syntax

#### Defining Functions

```
ready> def add(x y) x + y;
```

Output:
```
Parsed a function definition.
define double @add(double %x, double %y) {
entry:
  %addtmp = fadd double %x, %y
  ret double %addtmp
}
```

#### Declaring External Functions

```
ready> extern sin(x);
```

#### Expressions

```
ready> 2 + 3;
ready> 4 * (2 + 3);
ready> add(1, 2);
```

#### Binary Operators

| Operator | Description |
|----------|-------------|
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `<` | Less than comparison |

#### Comments

Lines starting with `#` are treated as comments:

```
# This is a comment
def foo(x) x * 2;
```

### Example Session

```
ready> def multiply(a b) a * b;
Parsed a function definition.
define double @multiply(double %a, double %b) {
entry:
  %multmp = fmul double %a, %b
  ret double %multmp
}

ready> def square(x) multiply(x, x);
Parsed a function definition.
define double @square(double %x) {
entry:
  %calltmp = call double @multiply(double %x, double %x)
  ret double %calltmp
}

ready> extern cos(x);
Parsed an extern.
declare double @cos(double)

ready> cos(1.0);
Parsed a top-level expr
define double @__anon_expr() {
entry:
  %calltmp = call double @cos(double 1.000000e+00)
  ret double %calltmp
}
```

### Exiting

Press `Ctrl+D` (EOF) to exit the REPL and print the complete LLVM IR module.

## Project Structure

```
Kaleidoscope/
├── one.cpp           # Main compiler source code
├── tests/            # Unit tests
│   ├── lexer_test.cpp
│   └── README.md
├── LICENSE           # MIT License
└── README.md         # This file
```

## Architecture

The compiler consists of several components:

1. **Lexer** - Tokenizes input into tokens (numbers, identifiers, keywords)
2. **Parser** - Builds an Abstract Syntax Tree (AST) using recursive descent parsing
3. **AST Nodes** - Represent expressions, function definitions, and prototypes
4. **Code Generator** - Traverses the AST and generates LLVM IR

### AST Node Types

- `NumberExprAST` - Numeric literals
- `VariableExprAST` - Variable references
- `BinaryExprAST` - Binary operations (+, -, *, <)
- `CallExprAST` - Function calls
- `PrototypeAST` - Function prototypes
- `FunctionAST` - Function definitions

## Resources

- [LLVM Kaleidoscope Tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
- [LLVM Language Reference](https://llvm.org/docs/LangRef.html)
- [LLVM Programmer's Manual](https://llvm.org/docs/ProgrammersManual.html)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
