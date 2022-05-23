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

bool Visitor::visitChildren(shared_ptr<ASTNode> node) {
    
    for (auto c : node->children) {
        if (!visit(c)) {
            return false;
        }
    }

    return true;
}
identifier_type Visitor::getIdentifierType(string typ) {
    if (typ == "int")   return ID_INT;
    if (typ == "float") return ID_FLOAT;
    if (typ == "char")  return ID_CHAR;
    if (typ == "bool")  return ID_BOOL;

    return ID_EMPTY;
}
return_type Visitor::getReturnType(string lexeme) {
    if (lexeme == "int")   return RETURN_INT;
    if (lexeme == "float") return RETURN_FLOAT;
    if (lexeme == "char")  return RETURN_CHAR;
    if (lexeme == "bool")  return RETURN_BOOL;
    
    return RETURN_EMPTY;
}

SemanticVisitor::SemanticVisitor(){}
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

bool SemanticVisitor::exprIsChar(shared_ptr<ASTNode> node) {

    //This amount of children is guaranteed
    auto v = node->children[0]->children[0]->children[0]->children[0];
        
    if (v->type == AST_FUNC_CALL) {

        //Check if function strictly returns a char.
        auto identifier_node = v->children[0];

        auto func_it = find_if(scopeStk.functions.begin(), scopeStk.functions.end(), [&identifier_node] (pair<string,symbolFunc> p) {
            return p.first == identifier_node->attr;
        });

        return ((*func_it).second.type == RETURN_CHAR);
    }
    
    return v->type == AST_CHAR_LIT;

}


void SemanticVisitor::printError() {
    
    Token errTok = scopeStk.errToken;
    Token corrTok = scopeStk.corrToken;

    int exp = scopeStk.errExpectedParams;
    int act = scopeStk.errActualParams;

    string lex = scopeStk.errToken.lexeme;
    semantic_error_msg i = scopeStk.errMsg;
    stringstream msg;

    switch (i) {
        case SEMERR_FUNC_DECL_ALREADY_DECLARED:   {msg << ANSI_RED << "Multiple definitons of " << ANSI_YEL << lex << ANSI_RED << " found";} break;
        case SEMERR_FUNC_DECL_NO_RETURN:          {msg << ANSI_RED << "Function " << ANSI_YEL << lex << ANSI_RED << " does not guarantee return";} break;
        case SEMERR_VAR_DECL_ALREADY_DECLARED:    {msg << ANSI_RED << "Variable " << ANSI_YEL << lex << ANSI_RED << " has already been declared at ["
                                                       << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col << ANSI_RED << "]";} break;
        case SEMERR_VAR_DECL_IDENTICAL_FUNCTION:  {msg << ANSI_RED << "Variable " << ANSI_YEL << lex << ANSI_RED << " shares name with function declared at ["
                                                       << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col << ANSI_RED << "]";} break;
        case SEMERR_ID_NOT_DECLARED:              {msg << ANSI_RED << "Variable " << ANSI_YEL << lex << ANSI_RED << " is not declared";} break;
        case SEMERR_FUNC_CALL_NOT_DECLARED:       {msg << ANSI_RED << "Function " << ANSI_YEL << lex << ANSI_RED << " is not declared";} break;
        case SEMERR_FUNC_CALL_WRONG_PARAM:        {msg << ANSI_RED << "Function " << ANSI_YEL << lex << ANSI_RED << " defined at ["
                                                       << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col
                                                       << ANSI_RED << "] expects " << exp << " parameters but " << act << " were given";} break;
        case SEMERR_TYPE_INVALID_CHAR_CONVERSION: {msg << ANSI_RED << "Variable assignment of " << ANSI_YEL << lex << ANSI_RED << " of type" << ANSI_YEL << " char " 
                                                       << ANSI_RED <<"does not guarantee a char value";} break;
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
            scopeStk.errToken = identifier_node->token;

            shared_ptr<ASTNode> return_node, block_node, params_node;
            params_node = nullptr;

            //Determine if the function declaration has parameters.
            if (node->children.size() == 3) {
                return_node = node->children[1];
                block_node  = node->children[2];
            } else {
                params_node = node->children[1];
                return_node = node->children[2];
                block_node  = node->children[3];
            }

            // Assert that the function is not already declared
            if (scopeStk.isFuncDecl(identifier_node->attr)) {
                scopeStk.errMsg = SEMERR_FUNC_DECL_ALREADY_DECLARED;
                return false;
            }

            // Assert that the function returns an expression
            if (!blockReturns(block_node)) {
                scopeStk.errMsg = SEMERR_FUNC_DECL_NO_RETURN;
                return false;
            }
                 
            // Get number of parameters
            int param_num = params_node? params_node->children.size():0;

            // Add function to map of function declarations
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

    auto current_scope = scopeStk.getTop();

    switch (node->type) {

        case AST_PROGRAM: {
            if (!visitChildren(node)) {
                //Print error message
                return false;
            }
        } break;

        case AST_BLOCK: {
            //Push new scope
            scopeStk.push(map<string,symbol>());
            
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


            //Check if a variable with the same name is already declared in the current scope.
            auto symbol_it = find_if(current_scope.begin(), current_scope.end(), [&identifier_node] (pair<string,symbol> p) {
                return p.first == identifier_node->attr;
            });
            
            if (symbol_it != current_scope.end()) {
                scopeStk.errMsg = SEMERR_VAR_DECL_ALREADY_DECLARED;
                scopeStk.errToken = identifier_node->token;
                scopeStk.corrToken = (*symbol_it).second.token;

                return false;
            }

            //Check if a function with the same name is already declared.
            auto func_it = find_if(scopeStk.functions.begin(), scopeStk.functions.end(), [&identifier_node] (pair<string,symbolFunc> p) {
                return p.first == identifier_node->attr;
            });

            if (func_it != scopeStk.functions.end()) {
                scopeStk.errMsg = SEMERR_VAR_DECL_IDENTICAL_FUNCTION;
                scopeStk.errToken = node->token;
                scopeStk.corrToken = (*func_it).second.token;

                return false;
            }

            //Check if the type is a char then the RHS is strictly char.
            if (getIdentifierType(type_node->attr) == ID_CHAR && !exprIsChar(expression_node)) {
                scopeStk.errMsg = SEMERR_TYPE_INVALID_CHAR_CONVERSION;
                scopeStk.errToken = identifier_node->token;

                return false;
            }

            //If variable declaration is valid, add variable to symbol table
            scopeStk.addSymbol(identifier_node->attr, getIdentifierType(type_node->attr),{},identifier_node->token);
        
        } break;

        case AST_ASSIGN: {
            //Get Identifier, Type and Expression of variable declaration
            auto identifier_node = node->children[0];
            auto expression_node = node->children[1]; 
            
            //Assert ID and Expression ar valid.
            if (!visit(identifier_node) || !visit(expression_node)) {
                return false;
            }

            //Get entry in symbol table. We can guarantee an entry exists becasue we visit identifier_node.
            auto symbol_it = find_if(current_scope.begin(), current_scope.end(), [&identifier_node] (pair<string,symbol> p) {
                return p.first == identifier_node->attr;
            });
            
            //Check if the type is a char then the RHS is strictly char.
            if ((*symbol_it).second.type == ID_CHAR && !exprIsChar(expression_node)) {
                scopeStk.errMsg = SEMERR_TYPE_INVALID_CHAR_CONVERSION;
                scopeStk.errToken = identifier_node->token;

                return false;
            }
            
        } break;

        case AST_FUNC_DECL: {
            //Since function declarations are already handled, ignore all children of function declarations
            return true;
        } break;

        //Order is important. FuncCall must be before Identfier because FuncCall ⊆ Identifier.
        case AST_FUNC_CALL: {

            auto identifier_node    = node->children[0];
            auto actual_params_node = node->children[1];

            //Assert function exists
            if (!scopeStk.isFuncDecl(identifier_node->attr)) {
                scopeStk.errMsg = SEMERR_FUNC_CALL_NOT_DECLARED;
                scopeStk.errToken = identifier_node->token;

                return false;
            }

            //Assert function has same number of parameters as declaration.
            auto func = scopeStk.getFuncAttr(identifier_node->attr);
            scopeStk.errExpectedParams = func.params;
            scopeStk.errActualParams = actual_params_node->children.size();

            if (scopeStk.errExpectedParams != scopeStk.errActualParams) {
                scopeStk.errMsg    = SEMERR_FUNC_CALL_WRONG_PARAM;
                scopeStk.errToken  = node->token;
                scopeStk.corrToken = func.token;

                return false;
            }

            visit(actual_params_node);

        } break;

        //This case is only entered when the identifier is a variable being evaluated as an expression (not as a function call/decl)
        case AST_IDENTIFIER: {
            
            //Check if this variable has been declared before
            if (!scopeStk.isDecl(node->attr)) {
                scopeStk.errMsg = SEMERR_ID_NOT_DECLARED;
                scopeStk.errToken = node->token;

                return false;
            }

            //Check that there isn't a function with the same name
            //Check if a function with the same name is already declared.
            auto func_it = find_if(scopeStk.functions.begin(), scopeStk.functions.end(), [&node] (pair<string,symbolFunc> p) {
                return p.first == node->attr;
            });


            if (func_it != scopeStk.functions.end()) {
                scopeStk.errMsg = SEMERR_VAR_DECL_IDENTICAL_FUNCTION;
                scopeStk.errToken = node->token;
                scopeStk.corrToken = (*func_it).second.token;

                return false;
            }

        } break;

    

        default: {
            if(!visitChildren(node))
                return false;
        }
    }
    
    return true;

}

InterpreterVisitor::InterpreterVisitor() {}
bool InterpreterVisitor::visit(shared_ptr<ASTNode> node, int depth) {
    auto current_scope = scopeStk.getTop();

    switch (node->type) {

        case AST_PROGRAM: {
            if (!visitChildren(node)) {
                //Print error message
                return false;
            }
        } break;

        case AST_BLOCK: {
            //Push new scope
            scopeStk.push(map<string,symbol>());
            
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


            //Evaluate expression
            auto v = evaluate(expression_node);
            
            //If variable declaration is valid, add variable to symbol table
            scopeStk.addSymbol(identifier_node->attr, getIdentifierType(type_node->attr),v,identifier_node->token);
        
        } break;

        case AST_FUNC_DECL: {
            //Since function declarations are already handled, ignore all children of function declarations
            return true;
        } break;

        //This case is only entered when the identifier is a variable being evaluated as an expression (not as a function call/decl)
        case AST_IDENTIFIER: {
            

        } break;

        case AST_EXPR: {



        } break;
        


        default: {
            if(!visitChildren(node))
                return false;
        }
    }
    
    return true;
}

value InterpreterVisitor::evaluate(shared_ptr<ASTNode> node) {

    switch(node->type) {

        case AST_IDENTIFIER: {
            

            //TODO: CHECK ALL SCOPES; START FROM TOP AND GO TO BOTTOM.
            //Return value from symbol table. Must check all scopes.
            auto current_scope = scopeStk.getTop();

            auto symbol_it = find_if(current_scope.begin(), current_scope.end(), [&node] (pair<string,symbol> p) {
                return p.first == node->attr;
            });

            return (*symbol_it).second.val;
        }

        case AST_EXPR: {
            
            auto v = evaluate(node->children[0]);

            if (node->children.size() == 1) {
                return v;
            }

            //Evaluate relation operation chain.
            for (int i = 1; i < node->children.size(); i += 2) {
                
                auto relop = node->children[i];
                auto u     = evaluate(node->children[i+1]);


                //Evaluate relation operation.
                if (relop->attr[0] == '>')
                    if (relop->attr.size() != 1) v.data = v.data >= u.data;
                    else                         v.data = v.data > u.data;
                else
                    if (relop->attr.size() != 1) v.data = v.data <= u.data;
                    else                         v.data = v.data < u.data;             

                //Return type is strictly bool after relational operator.
                v.type = ID_BOOL;
            }

            return v;
        } break;

        case AST_SIMPLE_EXPR: {
            auto v = evaluate(node->children[0]);

            if (node->children.size() == 1) {
                return v;
            }

            //Evaluate addition operation chain.
            for (int i = 1; i < node->children.size(); i += 2) {
                
                auto addop = node->children[i];
                auto u     = evaluate(node->children[i+1]);

                //Evaluate addition operation.
                if (addop->attr == "+") {
                    v.data += u.data;
                } else if (addop->attr == "-") {
                    v.data -= u.data;
                } else  {
                    v.data = (int) v.data | (int) u.data;
                    v.type = ID_INT;
                }
            

            }

            return v;
        } break;


        case AST_TERM: {
            auto v = evaluate(node->children[0]);

            if (node->children.size() == 1) {
                return v;
            }

            //Evaluate multiplication operation chain.
            for (int i = 1; i < node->children.size(); i += 2) {
                
                auto mulop = node->children[i];
                auto u     = evaluate(node->children[i+1]);

                //Evaluate addition operation.
                if (mulop->attr == "*") {
                    v.data *= u.data;
                } else if (mulop->attr == "/") {
                    v.data /= u.data;
                } else  {
                    v.data = (int) v.data & (int) u.data;
                    v.type = ID_INT;
                }

            }

            return v;
        } break;

        case AST_FACTOR: {
            
            auto child = node->children[0];

            switch (child->type) {
                case AST_BOOL_LIT:   return {stof(child->attr), ID_BOOL };
                case AST_CHAR_LIT:   return {stof(child->attr), ID_CHAR };
                case AST_INT_LIT:    return {stof(child->attr), ID_INT  };
                case AST_FLOAT_LIT:  return {stof(child->attr), ID_FLOAT};
                
                case AST_UNOP: {
                    
                    auto u = evaluate(child->children[0]);

                    if (child->attr == "-") {    
                        u.data = -u.data;
                    } else {
                        u.data = ~ (int) u.data;
                        u.type = ID_INT;
                    }
                    
                    return u;
                }


                case AST_FUNC_CALL: {

                    //Find function declaration.
                    
                    auto identifier_node = child->children[0];

                    auto func_it = find_if(scopeStk.functions.begin(), scopeStk.functions.end(), [&identifier_node] (pair<string,symbolFunc> p) {
                        return p.first == identifier_node->attr;
                    });

                    //Access block node in function declaration.

                    //Run block node.

                    //Get return value ret.

                    //Return ret.

                    return {};     

                }
                
                default: return evaluate(child);
            }
        
        } break;




    }

    return {};


}

