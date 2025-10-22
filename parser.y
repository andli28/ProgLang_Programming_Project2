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

// Added for new grammar rules
    StmtNode* stmt;
    ExprNode* expr;
    TypeNode* type;
    BlockNode* block;
    ParamNode* param;
    std::vector<ExprNode*>* expr_list;
    std::vector<ParamNode*>* param_list;
}

// If you make changes here, remember to update the corresponding parts in lex file as well (TERMINALS)
%token <floatval> FLOAT_LITERAL
%token <intval> INTEGER_LITERAL 
%token <boolval> BOOL_LITERAL
%token <text> IDENTIFIER
%token FUNC_KEYWORD VAR_KEYWORD LET_KEYWORD IF_KEYWORD ELSE_KEYWORD WHILE_KEYWORD PRINT_KEYWORD RETURN_KEYWORD INT_KEYWORD FLOAT_KEYWORD BOOL_KEYWORD
%token ASSIGN_OP EQUAL_OP NEQ_OP LT_OP GT_OP LEQ_OP GEQ_OP
%token PLUS_OP MINUS_OP MULTIPLY_OP DIVIDE_OP
%token LPAREN_DELIMITER RPAREN_DELIMITER LBRACE_DELIMITER RBRACE_DELIMITER SEMI_DELIMITER COMMA_DELIMITER COLON_DELIMITER
%token END_OF_FILE


// Modify or add more %type declarations as needed (NONTERMINALS)
%type <program> program
%type <code_item> code_item
%type <decl> decl func_decl var_decl // Added func_decl and var_decl
%type <decl_list> decl_list
%type <code_item_list> code_item_list

// Added for new grammar rules
%type <stmt> stmt assignment_stmt if_stmt while_stmt print_stmt return_stmt
%type <expr> expr literal func_call
%type <type> type
%type <block> block
%type <param> param
%type <param_list> param_list opt_param_list
%type <expr_list> arg_list opt_arg_list

%start program

// --- Operator Precedence & Associativity ---
// (Lowest to Highest)
%left EQUAL_OP NEQ_OP
%left LT_OP GT_OP LEQ_OP GEQ_OP
%left PLUS_OP MINUS_OP
%left MULTIPLY_OP DIVIDE_OP
%precedence UMINUS // For unary minus precedence

%%
// --- Top Level Rules ---
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
    : func_decl { $$ = $1; }
    | var_decl { $$ = $1; }
    ;

// --- Declaration Rules ---

func_decl
    : FUNC_KEYWORD IDENTIFIER LPAREN_DELIMITER opt_param_list RPAREN_DELIMITER COLON_DELIMITER type block {
        $$ = new FuncDeclNode(*$2, $4, $7, $8);
        delete $2;
    }
    ;

var_decl
    : VAR_KEYWORD IDENTIFIER COLON_DELIMITER type ASSIGN_OP expr SEMI_DELIMITER {
        $$ = new VarDeclNode(*$2, $4, $6, false); // false indicates 'var' (mutable)
        delete $2;
    }
    | LET_KEYWORD IDENTIFIER COLON_DELIMITER type ASSIGN_OP expr SEMI_DELIMITER {
        $$ = new VarDeclNode(*$2, $4, $6, true); // true indicates 'let' (immutable)
        delete $2;
    }
    ;

// --- Type Rule ---

type
    : INT_KEYWORD   { $$ = new TypeNode(PrimitiveType::INT); }
    | FLOAT_KEYWORD { $$ = new TypeNode(PrimitiveType::FLOAT); }
    | BOOL_KEYWORD  { $$ = new TypeNode(PrimitiveType::BOOL); }
    ;

// --- Block & Statement Rules ---

block
    : LBRACE_DELIMITER code_item_list RBRACE_DELIMITER {
        $$ = new BlockNode($2);
    }
    | LBRACE_DELIMITER RBRACE_DELIMITER {
        $$ = new BlockNode(new std::vector<CodeItemNode*>);
    }
    ;

code_item_list
    : code_item_list code_item {
        $$ = $1;
        $$->push_back($2);
    }
    | code_item {
        $$ = new std::vector<CodeItemNode*>;
        $$->push_back($1);
    }
    ;

code_item
    : stmt     { $$ = $1; }
    | decl     { $$ = $1; }
    ;

stmt
    : assignment_stmt { $$ = $1; }
    | if_stmt         { $$ = $1; }
    | while_stmt      { $$ = $1; }
    | print_stmt      { $$ = $1; }
    | return_stmt     { $$ = $1; }
    ;

assignment_stmt
    : IDENTIFIER ASSIGN_OP expr SEMI_DELIMITER {
        $$ = new AssignmentStmtNode(*$1, $3);
        delete $1;
    }
    ;

if_stmt
    : IF_KEYWORD LPAREN_DELIMITER expr RPAREN_DELIMITER block {
        $$ = new IfStmtNode($3, $5, nullptr);
    }
    | IF_KEYWORD LPAREN_DELIMITER expr RPAREN_DELIMITER block ELSE_KEYWORD block {
        $$ = new IfStmtNode($3, $5, $7);
    }
    ;

while_stmt
    : WHILE_KEYWORD LPAREN_DELIMITER expr RPAREN_DELIMITER block {
        $$ = new WhileStmtNode($3, $5);
    }
    ;

print_stmt
    : PRINT_KEYWORD LPAREN_DELIMITER expr RPAREN_DELIMITER SEMI_DELIMITER {
        $$ = new PrintStmtNode($3);
    }
    ;

return_stmt
    : RETURN_KEYWORD expr SEMI_DELIMITER {
        $$ = new ReturnStmtNode($2);
    }
    ;

// --- Expression Rules ---

expr
    : literal                   { $$ = $1; }
    | IDENTIFIER                { $$ = new IdentifierNode(*$1); delete $1; }
    | func_call                 { $$ = $1; }
    | LPAREN_DELIMITER expr RPAREN_DELIMITER { $$ = $2; }
    | MINUS_OP expr %prec UMINUS { $$ = new UnaryOpNode($2); }
    | expr MULTIPLY_OP expr     { $$ = new BinaryOpNode($1, BinaryOperator::MULTIPLY, $3); }
    | expr DIVIDE_OP expr       { $$ = new BinaryOpNode($1, BinaryOperator::DIVIDE, $3); }
    | expr PLUS_OP expr         { $$ = new BinaryOpNode($1, BinaryOperator::PLUS, $3); }
    | expr MINUS_OP expr        { $$ = new BinaryOpNode($1, BinaryOperator::MINUS, $3); }
    | expr LT_OP expr           { $$ = new BinaryOpNode($1, BinaryOperator::LT, $3); }
    | expr GT_OP expr           { $$ = new BinaryOpNode($1, BinaryOperator::GT, $3); }
    | expr LEQ_OP expr          { $$ = new BinaryOpNode($1, BinaryOperator::LEQ, $3); }
    | expr GEQ_OP expr          { $$ = new BinaryOpNode($1, BinaryOperator::GEQ, $3); }
    | expr EQUAL_OP expr        { $$ = new BinaryOpNode($1, BinaryOperator::EQUAL, $3); }
    | expr NEQ_OP expr          { $$ = new BinaryOpNode($1, BinaryOperator::NEQ, $3); }
    ;

literal
    : INTEGER_LITERAL           { $$ = new LiteralNode($1); }
    | FLOAT_LITERAL             { $$ = new LiteralNode($1); }
    | BOOL_LITERAL              { $$ = new LiteralNode($1); }
    ;

func_call
    : IDENTIFIER LPAREN_DELIMITER opt_arg_list RPAREN_DELIMITER {
        $$ = new FuncCallNode(*$1, $3);
        delete $1;
    }
    ;

// --- Parameter & Argument List Rules ---

opt_param_list
    : param_list                { $$ = $1; }
    | %empty                    { $$ = new std::vector<ParamNode*>; }
    ;

param_list
    : param_list COMMA_DELIMITER param {
        $$ = $1;
        $$->push_back($3);
    }
    | param {
        $$ = new std::vector<ParamNode*>;
        $$->push_back($1);
    }
    ;

param
    : IDENTIFIER COLON_DELIMITER type {
        $$ = new ParamNode(*$1, $3);
        delete $1;
    }
    ;

opt_arg_list
    : arg_list                  { $$ = $1; }
    | %empty                    { $$ = new std::vector<ExprNode*>; }
    ;

arg_list
    : arg_list COMMA_DELIMITER expr {
        $$ = $1;
        $$->push_back($3);
    }
    | expr {
        $$ = new std::vector<ExprNode*>;
        $$->push_back($1);
    }
    ;

%%

void yyerror(ASTNode** root, const char* s) {
    // root parameter is required by parse-param but not used here
    (void)root;  // Suppress unused parameter warning
    (void)s;     // suppress unused param warning

    throw ParserException(yylloc.first_line, yylloc.first_column);
}