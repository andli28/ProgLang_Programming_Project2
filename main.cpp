#ifndef YYDEBUG
#define YYDEBUG 0  // Default to 0 if not defined by compiler
#endif
#include <iostream>
#include <fstream>
#include <memory>
#include "astnode.hpp"
#include "exception.hpp"
#include "parser.tab.hpp"

extern FILE* yyin;
extern int yydebug;

int main(int argc, char** argv) {
    yydebug = YYDEBUG;  // Set to 1 to enable parser debug output
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source-file>" << std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "Cannot open source file: " << argv[1] << std::endl;
        return 1;
    }

    try {
        ASTNode* ast = nullptr;
        int result = yyparse(&ast);
        std::cout << "Parse result: " << result << std::endl;
    } catch (const LexerException& e) {
        std::cerr << "Lexer error: " << e.what() << std::endl;
    } catch (const ParserException& e) {
        std::cerr << "Parser error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
    }
    fclose(yyin);
    return 0;
}
