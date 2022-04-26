#include "ASTNode.hpp"

ASTNode::ASTNode(node_type type) {
    this->type = type;
    attr = "";
}