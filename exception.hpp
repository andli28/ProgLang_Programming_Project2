#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>
#include <string>

class LexerException : public std::runtime_error {
 public:
    LexerException(int line, int col) : std::runtime_error(
        "Lexer Error at line " +
        std::to_string(line) +
        ", column " + std::to_string(col)) {}
};

class ParserException : public std::runtime_error {
 public:
    ParserException(int line, int col) : std::runtime_error(
        "Parser Error at line " +
        std::to_string(line) +
        ", column " + std::to_string(col)) {}
};

#endif /* EXCEPTION_HPP */
