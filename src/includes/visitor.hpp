#pragma once
#include "ASTNode.hpp"
#include "scopeStk.hpp"

#include <map>
#include <stack>

using namespace std;

class Visitor {
    public:
        virtual bool visit(shared_ptr<ASTNode> node, int depth=0) = 0; // The operation of the visitor class will be in this function.
};

class XMLVisitor: public Visitor {
    public:
        XMLVisitor(string filepath);
        virtual bool visit(shared_ptr<ASTNode> node, int depth=0);
        
        ofstream outfile;
};

class SemanticVisitor: public Visitor {
    public: 
        SemanticVisitor();
        virtual bool visit(shared_ptr<ASTNode> node, int depth=0);
        bool visitFuncDecl(shared_ptr<ASTNode> node);
        bool analyseFuncDecl(shared_ptr<ASTNode> root_node);


    private:
        bool visitChildren(shared_ptr<ASTNode> node);
        identifier_type getIdentifierType(string typ);
        return_type getReturnType(string lexeme);
        bool blockReturns(shared_ptr<ASTNode> node);
        void printError();

        ScopeStk scopeStk;
        

};


