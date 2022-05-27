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
        node->type == AST_ADDOP      || node->type == AST_LIT) {
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

        return ((*func_it).second.type == ID_CHAR);
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
        case SEMERR_FUNC_DECL_NESTED_DECLARATION: {msg << ANSI_RED << "Function declaration " << ANSI_YEL << lex << ANSI_RED << " is nested in function declaration "
                                                       << ANSI_YEL << corrTok.lexeme << ANSI_RED << " at [" << ANSI_YEL << corrTok.ln << ANSI_RED << "," << ANSI_YEL << corrTok.col << ANSI_RED << "]";} break;
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
        case SEMERR_RETURN_NO_FUNCTION:            {msg << ANSI_RED << "Return statement outside function declaration";} break;                                               
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
   
    if (node->type == AST_FUNC_DECL){
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
            
        //Assert that the function is not declared in another function declaration
        shared_ptr<ASTNode> nested_func_decl = nullptr;

        if (nested_func_decl = node->isChildOf(AST_FUNC_DECL)) {
            scopeStk.corrToken = nested_func_decl->token;
            scopeStk.errMsg = SEMERR_FUNC_DECL_NESTED_DECLARATION;
            return false;
        }
        
        // Add function to map of function declarations
        scopeStk.addFunction(identifier_node->attr, getIdentifierType(return_node->attr), node->token, params_node, {});   
    }
        
    for (auto c : node->children) {
        if (!visitFuncDecl(c)) {
            return false;
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
                return false;
            }
        } break;

        case AST_BLOCK: {
            //Push new scope
            scopeStk.push(map<string,symbol>());
            
            //Run block
            bool eval = visitChildren(node);
            
            //Pop scope
            scopeStk.pop();

            return eval;
        } break;

        case AST_VAR_DECL: {
            //Get Identifier, Type and Expression of variable declaration
            auto identifier_node = node->children[0];
            auto type_node       = node->children[1];
            auto expression_node = node->children[2];

            //Evaluate expression and type
            if (!visit(expression_node) || !visit(type_node)) {
                return false;
            }

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
            auto identifier_node = node->children[0];
            int param_num = 0;

            if (node->children.size() != 1) {
                param_num = node->children[1]->children.size();
            }

            //Assert function exists
            if (!scopeStk.isFuncDecl(identifier_node->attr)) {
                scopeStk.errMsg = SEMERR_FUNC_CALL_NOT_DECLARED;
                scopeStk.errToken = identifier_node->token;

                return false;
            }

            //Assert function has same number of parameters as declaration.
            auto func = scopeStk.getFuncAttr(identifier_node->attr);
            scopeStk.errExpectedParams = func.params? func.params->children.size():0;
            scopeStk.errActualParams = param_num;
            

            if (scopeStk.errExpectedParams != scopeStk.errActualParams) {
                scopeStk.errMsg    = SEMERR_FUNC_CALL_WRONG_PARAM;
                scopeStk.errToken  = node->token;
                scopeStk.corrToken = func.token;

                return false;
            }

            if (param_num && !visitChildren(node->children[1])) {
                return false;
            }

        } break;

        //This case is only entered when the identifier is a variable being evaluated as an expression (not as a function call/decl)
        case AST_IDENTIFIER: {
            
            //Check if this variable has been declared before
            if (scopeStk.getSymbol(node->attr).type == ID_EMPTY) {
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
        
        case AST_FOR_STMT: case AST_WHILE_STMT: {
            
            //Push new scope for initial variable declaration in for loop.
            scopeStk.push(map<string,symbol>());

            if (!visitChildren(node)) {
                return false;
            }

            //Pop scope.
            scopeStk.pop();

        } break;

        case AST_RETURN_STMT: {
            
            //If return statement is not a child of a function declaration, raise error.
            if (!node->isChildOf(AST_FUNC_DECL)) {
                scopeStk.errToken = node->token;
                scopeStk.errMsg = SEMERR_RETURN_NO_FUNCTION;
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
float InterpreterVisitor::stringToFloat(string s) {
    
    //Boolean
    if (s == "true")  return 1;
    if (s == "false") return 0;

    //Char
    if (s[0] == '\'') return (float) s[1];

    //Int or Float
    return stof(s);
}

bool InterpreterVisitor::visitFuncDecl(shared_ptr<ASTNode> node) {

    switch (node->type) {
        
        case AST_FUNC_DECL: {
            auto identifier_node = node->children[0];

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
            
            // Add function to map of function declarations
            scopeStk.addFunction(identifier_node->attr, getIdentifierType(return_node->attr), node->token, params_node, block_node);   

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

            //Run block.
            bool eval = visitChildren(node);

            //Pop scope
            scopeStk.pop();
            return eval;

        } break;

        case AST_VAR_DECL: {
            //Get Identifier, Type and Expression of variable declaration
            auto identifier_node = node->children[0];
            auto type_node       = node->children[1];
            auto expression_node = node->children[2];

            //Evaluate expression
            auto v = evaluate(expression_node);
            v.type = getIdentifierType(type_node->attr);
            
            //If variable declaration is valid, add variable to symbol table
            scopeStk.addSymbol(identifier_node->attr, v.type ,v,identifier_node->token);
            return true;

        } break;

        case AST_ASSIGN: {
            
            //Find variable in symbol table and update value.
            auto identifier_node = node->children[0];
            auto expression_node = node->children[1];

            //Update value
            auto v = evaluate(expression_node);
            scopeStk.updateSymbol(identifier_node->attr, v);
            
            return true;

        } break;

        case AST_FUNC_DECL: {
            //Since function declarations are already handled, ignore all children of function declarations
            return true;
        } break;

        case AST_PRINT_STMT: {
            auto v = evaluate(node->children[0]);
            switch (v.type) {
                case ID_BOOL:  {cout << (v.data? "TRUE":"FALSE");} break;
                case ID_CHAR:  {cout << (char) v.data;} break;
                case ID_INT:   {cout << (int)  v.data;} break;
                case ID_FLOAT: {cout << v.data;} break;
            }

            return true;
        } break;
        
        case AST_IF_STMT: {

            if (evaluate(node->children[0]).data) {
                if (!visit(node->children[1]))
                    return false;
            } else if (node->children.size() == 3) {
                if (!visit(node->children[2]))
                    return false;
            }
            
            return true;
        } break;

        case AST_FOR_STMT: {
            
            shared_ptr<ASTNode> identifier_node = nullptr;
            shared_ptr<ASTNode> expression_node = nullptr;
            shared_ptr<ASTNode> increment_node  = nullptr;
            shared_ptr<ASTNode> block_node      = nullptr;

            bool has_id = false;

            //Push new scope
            scopeStk.push(map<string,symbol>());

            //Attempt to add initial variable declaration to block
            if (node->children[0]->type == AST_VAR_DECL) {
                identifier_node = node->children[0]->children[0];
                expression_node = node->children[1];
                
                visit(node->children[0]);
                has_id = true;

            } else {
                expression_node = node->children[0];
            }
            
            //Attempt to get increment assignment
            if (node->children[1+has_id]->type == AST_ASSIGN) {
                increment_node = node->children[1+has_id];
                block_node     = node->children[2+has_id];
            } else {
                block_node = node->children[1+has_id];
            }

            // While the condition is true
            while (evaluate(expression_node).data) {

                //Run block
                if (!visit(block_node)) {
                    scopeStk.pop(); 
                    return false;
                }

                if (increment_node) visit(increment_node); //Run increment
            }

            //Remove initial variable declaration
            scopeStk.pop(); //Pop scope
            
        } break;

        case AST_WHILE_STMT: {
            auto expression_node = node->children[0];
            auto block_node      = node->children[1];

            //Push new scope
            scopeStk.push(map<string,symbol>());

            while (evaluate(expression_node).data) {
                if (!visit(block_node)) {
                    scopeStk.pop();
                    return false;        
                };
            }

            //Pop scope
            scopeStk.pop();


        } break;

        case AST_RETURN_STMT: {
            auto expression_node = node->children[0];
            scopeStk.return_value = evaluate(expression_node);
            
            //Return false to stop execution from block.
            return false;
        }


        default: {
            if(!visitChildren(node))
                return false;
        }
    }
    
    return true;
}

value InterpreterVisitor::evaluate(shared_ptr<ASTNode> node) {

    switch(node->type) {
        
        //This block is strictly a function block.
        case AST_BLOCK: {
            visit(node);
            return scopeStk.return_value;
        } break;

        case AST_EXPR: {
            
            auto v = evaluate(node->children[0]);

            if (node->children.size() == 1) {
                return v;
            }

            //Evaluate relation operation chain.
            for (int i = 1; i < node->children.size(); i += 2) {
                
                auto relop = node->children[i];
                auto u     = evaluate(node->children[i+1]);

                cout << v.data << ' ' << u.data << '\n';

                //Evaluate relation operation.
                if (relop->attr[0] == '>')
                    if (relop->attr.size() != 1) v.data = v.data >= u.data;
                    else                         v.data = v.data > u.data;
                else if (relop->attr[0] == '<')
                    if (relop->attr.size() != 1) v.data = v.data <= u.data;
                    else                         v.data = v.data < u.data;
                else if (relop->attr[0] == '!')  v.data = v.data != u.data;
                     else                        v.data = v.data == u.data;

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

                v.type = max(v.type, u.type);

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

                v.type = max(v.type, u.type);

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
                case AST_BOOL_LIT:   return {stringToFloat(child->attr), ID_BOOL };
                case AST_CHAR_LIT:   return {stringToFloat(child->attr), ID_CHAR };
                case AST_INT_LIT:    return {stringToFloat(child->attr), ID_INT  };
                case AST_FLOAT_LIT:  return {stringToFloat(child->attr), ID_FLOAT};
                
                case AST_UNOP: {
                    
                    auto u = evaluate(child->children[0]);

                    if (child->attr == "-") {    

                        u.data = -u.data;
                    } else {
                        u.data = ~ (int) u.data;
                        u.type = ID_INT;
                    }
                    
                    return u;
                }  break;

                case AST_SUB_EXPR: {
                    return evaluate(child->children[0]);
                } break;

                case AST_FUNC_CALL: {
                    auto child = node->children[0];
                    auto identifier_node = child->children[0];

                    //Find function
                    auto func_it = scopeStk.functions.find(identifier_node->attr);

                    //Push Scope
                    scopeStk.push(map<string,symbol>()); 

                    //Evaluate actual parameters and add them to scope
                    if (child->children.size() != 1) {
                        auto actual_params_node = child->children[1];

                        for (int i = 0; i < actual_params_node->children.size(); i++) {
                            
                            //Find parameter from function declaration.
                            auto param_node = func_it->second.params->children[i];

                            //Get value passed for this parameter.
                            auto expression_node = actual_params_node->children[i];

                            string id = param_node->children[0]->attr;
                            identifier_type type = getIdentifierType(param_node->children[1]->attr);
                            Token token = param_node->token;

                            auto v = evaluate(expression_node);

                            scopeStk.addSymbol(id,type,v,token);

                        }
                    }

                    //Call function
                    auto v = evaluate(func_it->second.block);

                    //Set return type to match that of function declaration
                    v.type = func_it->second.type;

                    //Pop scope
                    scopeStk.pop(); 

                    //Return value from function
                    return v;
                } break;
                
                case AST_IDENTIFIER: {
                    return scopeStk.getSymbol(child->attr).val;
                }

            }
        
        } break;
        

    }

    return {};


}
