# Kaleidoscope Unit Tests

This folder contains unit tests for the Kaleidoscope compiler.

## Building Tests

```bash
g++ -std=c++17 -o lexer_test lexer_test.cpp
./lexer_test
```

## Test Structure

- `lexer_test.cpp` - Tests for the lexer/tokenizer
- More tests to be added for parser, AST, and codegen

## TODO

- [ ] Add Google Test or Catch2 framework
- [ ] Implement lexer tests
- [ ] Add parser tests
- [ ] Add AST tests
- [ ] Add code generation tests
