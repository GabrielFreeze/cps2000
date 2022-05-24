#pragma once
#include "ASTNode.hpp"
#include "scopeStk.hpp"

#include <map>
#include <stack>

using namespace std;

class Visitor {
    public:
        virtual bool visit(shared_ptr<ASTNode> node, int depth=0) = 0; // The operation of the visitor class will be in this function.
        identifier_type getIdentifierType(string typ);
        bool visitChildren(shared_ptr<ASTNode> node);
        return_type getReturnType(string lexeme);
    
    private:
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
        bool analyse(shared_ptr<ASTNode> root_node);


    private:
        
        bool exprIsChar(shared_ptr<ASTNode> block);
        bool blockReturns(shared_ptr<ASTNode> block);
        void printError();

        ScopeStk scopeStk;
        

};

class InterpreterVisitor: public Visitor {
    public: 
        InterpreterVisitor();
        virtual bool visit(shared_ptr<ASTNode> node, int depth=0);
        value evaluate(shared_ptr<ASTNode> node);
        identifier_type implicitTypeCast(value x, value y);

        


    private:
        float stringToFloat(string s);

        ScopeStk scopeStk;
};

