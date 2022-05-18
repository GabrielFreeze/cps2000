#include "visitor.hpp"
#include "colors.hpp"
#include <sstream>

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

SemanticVisitor::SemanticVisitor(){
    
}

identifier_type SemanticVisitor::getIdentifierType(string typ) {
    if (typ == "int")   return ID_INT;
    if (typ == "float") return ID_FLOAT;
    if (typ == "char")  return ID_CHAR;
    if (typ == "bool")  return ID_BOOL;

    return ID_EMPTY;
}
return_type SemanticVisitor::getReturnType(string lexeme) {
    if (lexeme == "int")   return RETURN_INT;
    if (lexeme == "float") return RETURN_FLOAT;
    if (lexeme == "char")  return RETURN_CHAR;
    if (lexeme == "bool")  return RETURN_BOOL;
    
    return RETURN_EMPTY;
}

bool SemanticVisitor::blockReturns(shared_ptr<ASTNode> block) {

    //Base Case: Find a return statement somewhere in the block.
    for (auto c : block->children) {
        if (c->type == AST_RETURN_STMT)
            return true;
    }
    
    //Find last if statement.
    auto if_node_it = find_if(block->children.rbegin(), block->children.rend(), [] (shared_ptr<ASTNode> node) {
                           return node->type == AST_IF_STMT;
                       });

    if (if_node_it == block->children.rend())
        return false;
    
    //Find else statement.
    if ((*if_node_it)->children.size() != 3)
        return false;

    //Check if if_block and else_block return.
    auto if_block   = (*if_node_it)->children[1];
    auto else_block = (*if_node_it)->children[2];

    return blockReturns(if_block) && blockReturns(else_block);

}

bool SemanticVisitor::visitChildren(shared_ptr<ASTNode> node) {
    
    for (auto c : node->children) {
        if (!visit(c)) {
            return false;
        }
    }

    return true;
}

void SemanticVisitor::printError() {
    
    Token errTok = scopeStk.errToken;
    Token corrTok = scopeStk.corrToken;

    string lex = scopeStk.errToken.lexeme;
    semantic_error_msg i = scopeStk.errMsg;
    stringstream msg;

    switch (i) {
        case SEMERR_FUNC_DECL_ALREADY_DECLARED:  {msg << ANSI_RED << "Multiple definitons of " << ANSI_YEL << lex << ANSI_RED << " found";} break;
        case SEMERR_FUNC_DECL_NO_RETURN:         {msg << ANSI_RED << "Function " << ANSI_YEL << lex << ANSI_RED << " does not guarantee return";} break;
        case SEMERR_VAR_DECL_ALREADY_DECLARED:   {msg << ANSI_RED << "Variable " << ANSI_YEL << lex << ANSI_RED << " has already been declared at ["
                                                      << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col << ANSI_RED << "]";} break;
        case SEMERR_VAR_DECL_IDENTICAL_FUNCTION: {msg << ANSI_RED << "Variable " << ANSI_YEL << lex << ANSI_RED << " shares name with function declared at ["
                                                      << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col << ANSI_RED << "]";} break;
    }


    cout << msg.str() << ANSI_RED << ": [" << ANSI_YEL
         << errTok.ln << ANSI_RED << ',' << ANSI_YEL << errTok.col
         << ANSI_RED "]"  << ANSI_ESC << '\n';
}

bool SemanticVisitor::analyseFuncDecl(shared_ptr<ASTNode> root_node) {
    if (!visitFuncDecl(root_node)) {
        printError();
        return false;
    }
    
    return true;
}

bool SemanticVisitor::visitFuncDecl(shared_ptr<ASTNode> node) {
    switch (node->type) {

        case AST_FUNC_DECL: {

            auto identifier_node = node->children[0];
            auto params_node     = node->children[1];
            auto return_node     = node->children[2];
            auto block_node      = node->children[3];
            scopeStk.errToken = identifier_node->token;

            //Assert that the function is not already declared
            if (scopeStk.isFuncDecl(identifier_node->attr)) {
                scopeStk.errMsg = SEMERR_FUNC_DECL_ALREADY_DECLARED;
                return false;
            }

            //Assert that the function returns an expression
            if (!blockReturns(block_node)) {
                scopeStk.errMsg = SEMERR_FUNC_DECL_NO_RETURN;
                return false;
            }
                 
            //Get number of parameters
            int param_num = params_node->children.size();

            //Add function to map of function declarations
            scopeStk.addFunction(identifier_node->attr, param_num, getReturnType(return_node->attr), node->token);   
        } break;

        default: {
            for (auto c : node->children) {
                if (!visitFuncDecl(c)) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool SemanticVisitor::analyse(shared_ptr<ASTNode> root_node) {    
    
    if (!visit(root_node)) {
        printError();
        return false;
    }
    
    return true;
}


bool SemanticVisitor::visit(shared_ptr<ASTNode> node, int depth) {

    //TODO WHEN ENTERING THIS FUNCTION SEGMENTATION FAULT OCCURS.
    auto current_scope = scopeStk.getTop();
    
    // switch (node->type) {

    //     case AST_PROGRAM: {
    //         if (!visitChildren(node)) {
    //             //Print error message
    //         }
    //     }

    //     case AST_BLOCK: {
    //         //Push new scope
    //         scopeStk.push(map<string,symbol>());
            
    //         if (!visitChildren(node))
    //             return false;

    //         //Pop scope
    //         scopeStk.pop();
    //     } break;

    //     case AST_VAR_DECL: {
    //         cout << "?";
    //         //Get Identifier, Type and Expression of variable declaration
    //         auto identifier_node = node->children[0];
    //         auto type_node       = node->children[1];
    //         auto expression_node = node->children[2];
            
            
            
    //         for (auto it : current_scope) {
    //            cout << it.first << endl;
    //         }

    //         //Check if a variable with the same name is already declared in the current scope.
    //         auto symbol_it = find_if(current_scope.begin(), current_scope.end(), [&identifier_node] (pair<string,symbol> p) {
    //             return p.first == identifier_node->attr;
    //         });
            
    //         if (symbol_it != current_scope.end()) {
    //             scopeStk.errMsg = SEMERR_VAR_DECL_ALREADY_DECLARED;
    //             scopeStk.errToken = node->token;
    //             scopeStk.corrToken = (*symbol_it).second.token;

    //             return false;
    //         }

            
    //         //Check if a function with the same name is already declared.
    //         auto func_it = find_if(scopeStk.functions.begin(), scopeStk.functions.end(), [&identifier_node] (pair<string,symbolFunc> p) {
    //             return p.first == identifier_node->attr;
    //         });

    //         if (func_it != scopeStk.functions.end()) {
    //             scopeStk.errMsg = SEMERR_VAR_DECL_IDENTICAL_FUNCTION;
    //             scopeStk.errToken = node->token;
    //             scopeStk.corrToken = (*func_it).second.token;

    //             return false;
    //         }

    //         //Check the type and the expression of the variable declaration.
    //         if (!visit(type_node) || !visit(expression_node)) {
    //             return false;
    //         }

    //         //If variable declaration is valid, add variable to symbol table
    //         scopeStk.addSymbol(identifier_node->attr, getIdentifierType(type_node->attr),0,identifier_node->token);
        
    //     } break;

    //     //This case is only entered when the identifier is a variable being evaluated as an expression (not as a function call/decl)
    //     case AST_IDENTIFIER: {

    //         //Check if this variable has been declared before
    //         if (!scopeStk.isDecl(node->attr)) {
    //             //Save error message: Variiable has not been declared
    //             return false;
    //         }

    //         //Check that there isn't a function with the same name
    //         if (scopeStk.isFuncDecl(node->attr)) {
    //             //Save error message: There exists a function with the same name.
    //         }

    //     } break;

    //     case AST_FUNC_CALL: {

    //         auto identifier_node = node->children[0];
    //         auto actual_params_node = node->children[1];

    //         //Assert function exists
    //         if (!scopeStk.isFuncDecl(identifier_node->attr)) {
    //             //Save error message: Function has no declaration
    //             return false;
    //         }

    //         //Assert function has same number of parameters as declaration.
    //         int expected = scopeStk.getFuncAttr(identifier_node->attr).params;
    //         int actual = actual_params_node->children.size();

    //         if (expected != actual) {
    //             //Save error message: Function expects _ parameters, _ given.
    //             return false;
    //         }

    //         visit(actual_params_node);

    //     } break;

        
    // }
    
    return true;

}