%code requires {
    #include "astnode.hpp"
} 
%{
#include <vector>
#include <string>
#include "astnode.hpp"
#include "exception.hpp"

#define YYDEBUG 1

void yyerror(ASTNode** root, const char* s);
extern int yylex();
%}
%parse-param { ASTNode** root }
%locations

%union {
    // Modify or add more types as needed
    ProgramNode* program;
    CodeItemNode* code_item;
    DeclNode* decl;
   
    std::vector<CodeItemNode*>* code_item_list;
    std::vector<DeclNode*>* decl_list;
    
    std::string* text;
    int intval;
    double floatval;
    bool boolval;
}

// If you make changes here, remember to update the corresponding parts in lex file as well
%token <floatval> FLOAT_LITERAL
%token <intval> INTEGER_LITERAL 
%token <boolval> BOOL_LITERAL
%token <text> IDENTIFIER
%token FUNC_KEYWORD VAR_KEYWORD LET_KEYWORD IF_KEYWORD ELSE_KEYWORD WHILE_KEYWORD PRINT_KEYWORD RETURN_KEYWORD INT_KEYWORD FLOAT_KEYWORD BOOL_KEYWORD
%token ASSIGN_OP EQUAL_OP NEQ_OP LT_OP GT_OP LEQ_OP GEQ_OP
%token PLUS_OP MINUS_OP MULTIPLY_OP DIVIDE_OP
%token LPAREN_DELIMITER RPAREN_DELIMITER LBRACE_DELIMITER RBRACE_DELIMITER SEMI_DELIMITER COMMA_DELIMITER COLON_DELIMITER
%token END_OF_FILE


// Modify or add more %type declarations as needed
%type <program> program
%type <code_item> code_item
%type <decl> decl
%type <decl_list> decl_list
%type <code_item_list> code_item_list

%start program

%%
// Modify or add other grammar rules here...
program
    : decl_list END_OF_FILE {
        ProgramNode* ast = new ProgramNode();
        for (auto& decl : *$1)
            ast->addDecl(decl);
        *root = ast;
        $$ = ast;
    }
    ;
decl_list
    : decl_list decl {
        $$ = $1;
        $$->push_back($2);
    }
    | decl {
        $$ = new std::vector<DeclNode*>;
        $$->push_back($1);
    }
    ;
decl
    : {
        // Add semantic actions here
    }
%%
