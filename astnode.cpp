#include "astnode.hpp"
#include <iostream>

void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << " ";
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
// Add other AST node implementations here...
