#pragma once
#include "ASTNode.hpp"

using namespace std;

class Visitor {
    public:
        virtual void visit(shared_ptr<ASTNode> node, int depth=0) = 0; // The operation of the visitor class will be in this function.
};

class XMLVisitor: public Visitor {
    public:
        XMLVisitor(string filepath);
        virtual void visit(shared_ptr<ASTNode> node, int depth=0);
        
        ofstream outfile;
};

