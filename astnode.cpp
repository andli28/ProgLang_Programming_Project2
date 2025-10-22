#include "astnode.hpp"
#include <iostream>

void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << " ";
}

std::string primitiveTypeToString(PrimitiveType type) {
    switch (type) {
        case PrimitiveType::INT:   return "int";
        case PrimitiveType::FLOAT: return "float";
        case PrimitiveType::BOOL:  return "bool";
        default:                 return "unknown_type";
    }
}

std::string binaryOpToString(BinaryOperator op) {
    switch (op) {
        case BinaryOperator::PLUS:     return "+";
        case BinaryOperator::MINUS:    return "-";
        case BinaryOperator::MULTIPLY: return "*";
        case BinaryOperator::DIVIDE:   return "/";
        case BinaryOperator::EQUAL:    return "==";
        case BinaryOperator::NEQ:      return "!=";
        case BinaryOperator::LT:       return "<";
        case BinaryOperator::GT:       return ">";
        case BinaryOperator::LEQ:      return "<=";
        case BinaryOperator::GEQ:      return ">=";
        default:                     return "unknown_op";
    }
}

// ProgramNode implementation
ProgramNode::~ProgramNode() {
    for (auto& decl : declarations) delete decl;
}

void ProgramNode::addDecl(DeclNode* decl) {
    declarations.push_back(decl);
}

void ProgramNode::print(int indent) const {
    printIndent(indent);
    std::cout << "ProgramNode:\n";
    for (auto& decl : declarations) decl->print(indent + 2);
}

#pragma region FuncDeclNode
//////////////////////////////// FuncDeclNode ////////////////////////////////
FuncDeclNode::FuncDeclNode(const std::string& id, std::vector<ParamNode*>* params, TypeNode* returnType, BlockNode* body)
    : id(id), params(params), returnType(returnType), body(body) {}

FuncDeclNode::~FuncDeclNode() {
    if (params) {
        for (auto param : *params) {
            delete param;
        }
        delete params;
    }
    delete returnType;
    delete body;
}

void FuncDeclNode::print(int indent) const {
    printIndent(indent);
    std::cout << "FuncDeclNode: " << id << std::endl;
    
    printIndent(indent + 1);
    std::cout << "Params:" << std::endl;
    if (params && !params->empty()) {
        for (const auto param : *params) {
            param->print(indent + 2);
        }
    } else {
        printIndent(indent + 2);
        std::cout << "(none)" << std::endl;
    }

    printIndent(indent + 1);
    std::cout << "Return Type:" << std::endl;
    returnType->print(indent + 2);

    printIndent(indent + 1);
    std::cout << "Body:" << std::endl;
    body->print(indent + 2);
}
#pragma endregion

#pragma region VarDeclNode
//////////////////////////////// VarDeclNode ////////////////////////////////
VarDeclNode::VarDeclNode(const std::string& id, TypeNode* type, ExprNode* initExpr, bool isImmutable)
    : id(id), type(type), initExpr(initExpr), isImmutable(isImmutable) {}

VarDeclNode::~VarDeclNode() {
    delete type;
    delete initExpr;
}

void VarDeclNode::print(int indent) const {
    printIndent(indent);
    std::cout << "VarDeclNode (" << (isImmutable ? "let" : "var") << "): " << id << std::endl;
    
    printIndent(indent + 1);
    std::cout << "Type:" << std::endl;
    type->print(indent + 2);

    printIndent(indent + 1);
    std::cout << "Init Expression:" << std::endl;
    initExpr->print(indent + 2);
}
#pragma endregion

#pragma region TypeNode
//////////////////////////////// TypeNode ////////////////////////////////
TypeNode::TypeNode(PrimitiveType type) : type(type) {}

void TypeNode::print(int indent) const {
    printIndent(indent);
    std::cout << "TypeNode: " << primitiveTypeToString(type) << std::endl;
}
#pragma endregion

#pragma region ParamNode
//////////////////////////////// ParamNode ////////////////////////////////
ParamNode::ParamNode(const std::string& id, TypeNode* type) : id(id), type(type) {}

ParamNode::~ParamNode() {
    delete type;
}

void ParamNode::print(int indent) const {
    printIndent(indent);
    std::cout << "ParamNode: " << id << std::endl;
    
    printIndent(indent + 1);
    std::cout << "Type:" << std::endl;
    type->print(indent + 2);
}
#pragma endregion

#pragma region BlockNode
//////////////////////////////// BlockNode ////////////////////////////////
BlockNode::BlockNode(std::vector<CodeItemNode*>* statements) : statements(statements) {}

BlockNode::~BlockNode() {
    if (statements) {
        for (auto stmt : *statements) {
            delete stmt;
        }
        delete statements;
    }
}

void BlockNode::print(int indent) const {
    printIndent(indent);
    std::cout << "BlockNode" << std::endl;
    if (statements) {
        for (const auto stmt : *statements) {
            stmt->print(indent + 1);
        }
    }
}
#pragma endregion

#pragma region AssignmentStmtNode
//////////////////////////////// AssignmentStmtNode ////////////////////////////////
AssignmentStmtNode::AssignmentStmtNode(const std::string& id, ExprNode* expr) : id(id), expr(expr) {}

AssignmentStmtNode::~AssignmentStmtNode() {
    delete expr;
}

void AssignmentStmtNode::print(int indent) const {
    printIndent(indent);
    std::cout << "AssignmentStmtNode: " << id << std::endl;
    expr->print(indent + 1);
}
#pragma endregion

#pragma region IfStmtNode
//////////////////////////////// IfStmtNode ////////////////////////////////
IfStmtNode::IfStmtNode(ExprNode* condition, BlockNode* thenBlock, BlockNode* elseBlock)
    : condition(condition), thenBlock(thenBlock), elseBlock(elseBlock) {}

IfStmtNode::~IfStmtNode() {
    delete condition;
    delete thenBlock;
    if (elseBlock) {
        delete elseBlock;
    }
}

void IfStmtNode::print(int indent) const {
    printIndent(indent);
    std::cout << "IfStmtNode" << std::endl;

    printIndent(indent + 1);
    std::cout << "Condition:" << std::endl;
    condition->print(indent + 2);

    printIndent(indent + 1);
    std::cout << "Then Block:" << std::endl;
    thenBlock->print(indent + 2);

    if (elseBlock) {
        printIndent(indent + 1);
        std::cout << "Else Block:" << std::endl;
        elseBlock->print(indent + 2);
    }
}
#pragma endregion

#pragma region WhileStmtNode
//////////////////////////////// WhileStmtNode ////////////////////////////////
WhileStmtNode::WhileStmtNode(ExprNode* condition, BlockNode* body)
    : condition(condition), body(body) {}

WhileStmtNode::~WhileStmtNode() {
    delete condition;
    delete body;
}

void WhileStmtNode::print(int indent) const {
    printIndent(indent);
    std::cout << "WhileStmtNode" << std::endl;

    printIndent(indent + 1);
    std::cout << "Condition:" << std::endl;
    condition->print(indent + 2);
    
    printIndent(indent + 1);
    std::cout << "Body:" << std::endl;
    body->print(indent + 2);
}
#pragma endregion

#pragma region PrintStmtNode
//////////////////////////////// PrintStmtNode ////////////////////////////////
PrintStmtNode::PrintStmtNode(ExprNode* expr) : expr(expr) {}

PrintStmtNode::~PrintStmtNode() {
    delete expr;
}

void PrintStmtNode::print(int indent) const {
    printIndent(indent);
    std::cout << "PrintStmtNode" << std::endl;
    expr->print(indent + 1);
}
#pragma endregion

#pragma region ReturnStmtNode
//////////////////////////////// ReturnStmtNode ////////////////////////////////
ReturnStmtNode::ReturnStmtNode(ExprNode* expr) : expr(expr) {}

ReturnStmtNode::~ReturnStmtNode() {
    delete expr;
}

void ReturnStmtNode::print(int indent) const {
    printIndent(indent);
    std::cout << "ReturnStmtNode" << std::endl;
    expr->print(indent + 1);
}
#pragma endregion

#pragma region IdentifierNode
//////////////////////////////// IdentifierNode ////////////////////////////////
IdentifierNode::IdentifierNode(const std::string& id) : id(id) {}

void IdentifierNode::print(int indent) const {
    printIndent(indent);
    std::cout << "IdentifierNode: " << id << std::endl;
}
#pragma endregion

#pragma region LiteralNode
//////////////////////////////// LiteralNode ////////////////////////////////
LiteralNode::LiteralNode(int val) : value(val) {}
LiteralNode::LiteralNode(double val) : value(val) {}
LiteralNode::LiteralNode(bool val) : value(val) {}

void LiteralNode::print(int indent) const {
    printIndent(indent);
    std::cout << "LiteralNode: ";
    std::visit([](auto&& arg) { std::cout << arg; }, value);
    std::cout << std::endl;
}
#pragma endregion

#pragma region FuncCallNode
//////////////////////////////// FuncCallNode ////////////////////////////////
FuncCallNode::FuncCallNode(const std::string& id, std::vector<ExprNode*>* args) : id(id), args(args) {}

FuncCallNode::~FuncCallNode() {
    if (args) {
        for (auto arg : *args) {
            delete arg;
        }
        delete args;
    }
}

void FuncCallNode::print(int indent) const {
    printIndent(indent);
    std::cout << "FuncCallNode: " << id << std::endl;
    
    printIndent(indent + 1);
    std::cout << "Arguments:" << std::endl;
    if (args && !args->empty()) {
        for (const auto arg : *args) {
            arg->print(indent + 2);
        }
    } else {
        printIndent(indent + 2);
        std::cout << "(none)" << std::endl;
    }
}
#pragma endregion

#pragma region UnaryOpNode
//////////////////////////////// UnaryOpNode ////////////////////////////////
UnaryOpNode::UnaryOpNode(ExprNode* expr) : expr(expr) {}

UnaryOpNode::~UnaryOpNode() {
    delete expr;
}

void UnaryOpNode::print(int indent) const {
    printIndent(indent);
    std::cout << "UnaryOpNode: -" << std::endl;
    expr->print(indent + 1);
}
#pragma endregion

#pragma region BinaryOpNode
//////////////////////////////// BinaryOpNode ////////////////////////////////
BinaryOpNode::BinaryOpNode(ExprNode* left, BinaryOperator op, ExprNode* right)
    : left(left), op(op), right(right) {}

BinaryOpNode::~BinaryOpNode() {
    delete left;
    delete right;
}

void BinaryOpNode::print(int indent) const {
    printIndent(indent);
    std::cout << "BinaryOpNode: " << binaryOpToString(op) << std::endl;
    
    printIndent(indent + 1);
    std::cout << "LHS:" << std::endl;
    left->print(indent + 2);

    printIndent(indent + 1);
    std::cout << "RHS:" << std::endl;
    right->print(indent + 2);
}
#pragma endregion

// Add other AST node implementations here...
