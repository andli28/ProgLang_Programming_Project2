#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <vector>
#include <string>
#include <variant>

//////////////////////////////// Forward Declarations for all Node Types ////////////////////////////////
class ASTNode;
class CodeItemNode;
class DeclNode;
class ProgramNode;
class StmtNode;
class ExprNode;
class TypeNode;
class BlockNode;
class ParamNode;
class FuncDeclNode;
class VarDeclNode;
class AssignmentStmtNode;
class IfStmtNode;
class WhileStmtNode;
class PrintStmtNode;
class ReturnStmtNode;
class IdentifierNode;
class LiteralNode;
class FuncCallNode;
class UnaryOpNode;
class BinaryOpNode;

// Enums
enum class PrimitiveType { INT, FLOAT, BOOL };
enum class BinaryOperator { 
    PLUS, MINUS, MULTIPLY, DIVIDE, 
    EQUAL, NEQ, LT, GT, LEQ, GEQ 
};

// Base ASTNode class
class ASTNode {
 public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

class CodeItemNode : public ASTNode {};
class DeclNode : public CodeItemNode {};
class StmtNode : public CodeItemNode {};
class ExprNode : public ASTNode {};

// Top-level ProgramNode
class ProgramNode : public ASTNode {
 public:
    std::vector<DeclNode*> declarations;
    ~ProgramNode();
    void addDecl(DeclNode* decl);
    void print(int indent = 0) const override;
};

// Declarations
class FuncDeclNode : public DeclNode {
public:
    std::string id;
    std::vector<ParamNode*>* params;
    TypeNode* returnType;
    BlockNode* body;

    FuncDeclNode(const std::string& id, std::vector<ParamNode*>* params, TypeNode* returnType, BlockNode* body);
    ~FuncDeclNode();
    void print(int indent = 0) const override;
};

class VarDeclNode : public DeclNode {
public:
    std::string id;
    TypeNode* type;
    ExprNode* initExpr;
    bool isImmutable; // true for 'let', false for 'var'

    VarDeclNode(const std::string& id, TypeNode* type, ExprNode* initExpr, bool isImmutable);
    ~VarDeclNode();
    void print(int indent = 0) const override;
};

//////////////////////////////// Type and Parameter Nodes ////////////////////////////////
class TypeNode : public ASTNode {
public:
    PrimitiveType type;
    
    TypeNode(PrimitiveType type);
    void print(int indent = 0) const override;
};

class ParamNode : public ASTNode {
public:
    std::string id;
    TypeNode* type;

    ParamNode(const std::string& id, TypeNode* type);
    ~ParamNode();
    void print(int indent = 0) const override;
};

//////////////////////////////// Statement and Block Nodes ////////////////////////////////
class BlockNode : public ASTNode {
public:
    std::vector<CodeItemNode*>* statements; // Can be DeclNode or StmtNode

    BlockNode(std::vector<CodeItemNode*>* statements);
    ~BlockNode();
    void print(int indent = 0) const override;
};

class AssignmentStmtNode : public StmtNode {
public:
    std::string id;
    ExprNode* expr;

    AssignmentStmtNode(const std::string& id, ExprNode* expr);
    ~AssignmentStmtNode();
    void print(int indent = 0) const override;
};

class IfStmtNode : public StmtNode {
public:
    ExprNode* condition;
    BlockNode* thenBlock;
    BlockNode* elseBlock; // Optional: Can be nullptr

    IfStmtNode(ExprNode* condition, BlockNode* thenBlock, BlockNode* elseBlock);
    ~IfStmtNode();
    void print(int indent = 0) const override;
};

class WhileStmtNode : public StmtNode {
public:
    ExprNode* condition;
    BlockNode* body;

    WhileStmtNode(ExprNode* condition, BlockNode* body);
    ~WhileStmtNode();
    void print(int indent = 0) const override;
};

class PrintStmtNode : public StmtNode {
public:
    ExprNode* expr;

    PrintStmtNode(ExprNode* expr);
    ~PrintStmtNode();
    void print(int indent = 0) const override;
};

class ReturnStmtNode : public StmtNode {
public:
    ExprNode* expr;

    ReturnStmtNode(ExprNode* expr);
    ~ReturnStmtNode();
    void print(int indent = 0) const override;
};

//////////////////////////////// Expression Nodes ////////////////////////////////
class IdentifierNode : public ExprNode {
public:
    std::string id;

    IdentifierNode(const std::string& id);
    void print(int indent = 0) const override;
};

class LiteralNode : public ExprNode {
public:
    std::variant<int, double, bool> value;

    LiteralNode(int val);
    LiteralNode(double val);
    LiteralNode(bool val);
    void print(int indent = 0) const override;
};

class FuncCallNode : public ExprNode {
public:
    std::string id;
    std::vector<ExprNode*>* args;

    FuncCallNode(const std::string& id, std::vector<ExprNode*>* args);
    ~FuncCallNode();
    void print(int indent = 0) const override;
};

class UnaryOpNode : public ExprNode {
public:
    ExprNode* expr;
    // Operator is implicitly UNARY_MINUS based on the grammar

    UnaryOpNode(ExprNode* expr);
    ~UnaryOpNode();
    void print(int indent = 0) const override;
};

class BinaryOpNode : public ExprNode {
public:
    ExprNode* left;
    BinaryOperator op;
    ExprNode* right;

    BinaryOpNode(ExprNode* left, BinaryOperator op, ExprNode* right);
    ~BinaryOpNode();
    void print(int indent = 0) const override;
};

// Add other AST node declarations here...
#endif // ASTNODE_HPP