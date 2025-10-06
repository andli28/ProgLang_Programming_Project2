#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <vector>
#include <string>

// Base ASTNode class
class ASTNode {
 public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

class CodeItemNode : public ASTNode {};
class DeclNode : public CodeItemNode {};

class ProgramNode : public ASTNode {
 public:
    std::vector<DeclNode*> declarations;
    ~ProgramNode();
    void addDecl(DeclNode* decl);
    void print(int indent = 0) const override;
};

// Add other AST node declarations here...
#endif // ASTNODE_HPP