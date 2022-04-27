#include "ASTNode.hpp"

ASTNode::ASTNode(node_type type) {
    this->type = type;
    attr = "";
}

void ASTNode::add_child(shared_ptr<ASTNode> node) {
    node->parent = shared_from_this();
    children.push_back(node);
}


