#include "ASTNode.hpp"

ASTNode::ASTNode(node_type type) {
    this->type = type;
    attr = "";
}

void ASTNode::add_child(shared_ptr<ASTNode> node) {
    // node->parent = shared_from_this();
    children.push_back(node);
}

bool ASTNode::isParentOf(node_type child_type, string attr) {
    for (auto c : children) {
        
        if ((c->type == child_type) && (c->attr == attr))
            return true;
        else
            c->isParentOf(child_type,attr);
    }

    return false;
}


