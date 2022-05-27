#include "ASTNode.hpp"

ASTNode::ASTNode(node_type type) {
    this->type = type;
    attr = "";
}

void ASTNode::add_child(shared_ptr<ASTNode> node) {
    node->parent = shared_from_this();
    children.push_back(node);
}

shared_ptr<ASTNode> ASTNode::isChildOf(node_type type) {
    
    if (parent->type == AST_PROGRAM)
        return nullptr;
    
    if (parent->type == type)
        return parent;
    
    return parent->isChildOf(type);
    
}

shared_ptr<ASTNode> ASTNode::isParentOf(node_type type) {
    for (auto c : children) {
        if (c->type == type)
            return c;
        
        return c->isParentOf(type);
    }

    return nullptr;
}


