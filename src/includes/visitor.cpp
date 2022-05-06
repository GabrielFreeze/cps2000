#include "visitor.hpp"

XMLVisitor::XMLVisitor (string filepath) {
    outfile.open(filepath);
}
bool XMLVisitor::visit (shared_ptr<ASTNode> node, int depth) {
    
    vector<string> map = {"LETTER", "DIGIT", "PRINTABLE", "TYPE", "BOOL_LIT", "INT_LIT",
                          "FLOAT_LIT", "CHAR_LIT", "LIT", "IDENTIFIER", "MULOP",
                          "ADDOP", "RELOP", "ACTUAL_PARAMS", "FUNC_CALL", "SUB_EXPR", "UNOP",
                          "FACTOR", "TERM", "SIMPLE_EXPR", "EXPR", "ASSIGN", "VAR_DECL", "PRINT_STMT",
                          "RETURN_STMT", "IF_STMT", "FOR_STMT", "WHILE_STMT", "FORMAL_PARAM",
                          "FORMAL_PARAMS", "FUNC_DECL", "STMT", "BLOCK", "PROGRAM"};
    
    //Indent Opening Tag
    for (int i = 0; i < depth; i++) {
        outfile << '\t';
    }

    //Open XML Tag of node->type
    outfile << "<"+map[node->type]+(node->attr.empty()? "":" ")+node->attr+">";
    
    if (node->type == AST_IDENTIFIER || node->type == AST_MULOP ||
        node->type == AST_ADDOP      || node->type == AST_UNOP  ||
        node->type == AST_LIT) {
        outfile << " "+node->attr+" ";
    } else {
        if (!node->children.empty()) {
            outfile << "\n";
        } else {
            outfile << " ";
        }

    }

    //Visit Children
    for (auto c : node->children) {
        visit(c,depth+1);
    }

    //Indent Closing Tag
    if (!node->children.empty()) {
        for (int i = 0; i < depth; i++) {
            outfile << '\t';
        }
    }

    //Close XML Tag
    outfile << "</"+map[node->type]+">\n";

    return true;
}


identifier_type SemanticVisitor::getIdentifierType(string typ) {
    if (typ == "int")   return ID_INT;
    if (typ == "float") return ID_FLOAT;
    if (typ == "char")  return ID_CHAR;
    if (typ == "bool")  return ID_BOOL;

    return ID_EMPTY;
}
bool SemanticVisitor::blockReturns(shared_ptr<ASTNode> node) {

    //Base Case: Find a return statement at the end of the block

    /*General Case:
        • Find last if-else statement
            •If none return false

        • blockReturns(if_node)
            •If false return false
        
        • blockReturns(else_node)
            •If false return false
        
        •return true
    */

}

bool SemanticVisitor::visitChildren(shared_ptr<ASTNode> node) {
    
    for (auto c : node->children) {
        if (!visit(c)) {
            return false;
        }
    }

    return true;
}

bool SemanticVisitor::visit(shared_ptr<ASTNode> node, int depth) {

    auto current_scope = scopeStk.getTop();


    switch (node->type) {

        case AST_PROGRAM: {
            if (!visitChildren(node)) {
                //Print error message
            }
        }


        case AST_BLOCK: {
            //Push new scope
            scopeStk.push(map<string,identifier_type>());
            
            if (!visitChildren(node))
                return false;

            //Pop scope
            scopeStk.pop();
        } break;

        case AST_VAR_DECL: {
            
            //Get Identifier, Type and Expression of variable declaration
            auto identifier_node = node->children[0];
            auto type_node       = node->children[1];
            auto expression_node = node->children[2];
            
            //Check that variable has not been declared before in current scope.
            if (current_scope.count(identifier_node->attr)) {
                //Save error message: Variable is already declared
                return false;
            }

            if (!visitChildren(node))
                return false;

            //If successful, add variable to symbol table
            scopeStk.addSymbol(identifier_node->attr, getIdentifierType(type_node->attr));
        
        } break;

        case AST_IDENTIFIER: {
            //Check if this identifier has been declared before (as a function or variable)
            
            if (!scopeStk.isDecl(node->attr)) {
                //Save error message: Identifier has not been declared
                return false;
            }

        } break;

        case AST_FUNC_CALL: {

            auto identifier_node = node->children[0];
            auto actual_params_node = node->children[1];

            //Assert function exists
            if (!scopeStk.isFuncDecl(identifier_node->attr)) {
                //Save error message: Function has no declaration
                return false;
            }

            //Assert function has same number of parameters as declaration.
            int expected = scopeStk.getFuncAttr(identifier_node->attr).first;
            int actual = actual_params_node->children.size();

            if (expected != actual) {
                //Save error message: Function expects _ parameters, _ given.
                return false;
            }      

        } break;

        case AST_FUNC_DECL: {

            auto identifier_node = node->children[0];

            //Assert that the function is declared once
            if (scopeStk.isFuncDecl(identifier_node->attr) != 1) {
                //Save error message: Multiple definitions of __ found
                return false;
            }

            //Assert that the function returns an expression

            if (!blockReturns(node)) {
                //Save error message: Function does not return a value in all branchees of execution
                return false;
            }
                 

           
        }
    }
    
    return true;

}