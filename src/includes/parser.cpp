#include "parser.hpp"
#include "ASTNode.hpp"

Parser::Parser(vector<Token>& tokens) {
    current_ASTNode = AST_PROGRAM; //The starting node.
    this->tokens = tokens; //Pass all tokens to the parser before parsing begins.
    tokens_length = tokens.size();
    next_token_index = 0;

    expTyp = TOK_EMPTY;
}

void Parser::printError() {
    
    Token tok = getCurrentToken();
    vector<string> map = {"(",")","{","}",";",":",",","=","integer value","float value","char value","(*, /, and)","(not, -)","(+,-,or)",
                          "(<,>,==,!=,<=,>=)","->","<comment>","boolean value","identifier",
                          "for","while","if","else","let","print","return","fn","float","int","bool","char","<empty>"};
    
    
    string exp_lexeme = map[expTyp];

    if (expTyp == TOK_SEMICOLON &&
       (tok.type == TOK_ID || tok.type == TOK_BOOL_LIT || tok.type == TOK_FLOAT_LIT || tok.type == TOK_INT_LIT || tok.type == TOK_CHAR_LIT)) {
           
        cout << ANSI_RED << "\nSyntax Error: " << ANSI_ESC << ANSI_YEL << "Expected a " << exp_lexeme << ANSI_ESC
             << ANSI_RED << " [" << ANSI_ESC << ANSI_YEL
             << tok.ln << ANSI_ESC << ANSI_RED << ',' << ANSI_ESC << ANSI_YEL << tok.col
             << ANSI_ESC << ANSI_RED "]"  << ANSI_ESC << '\n';
        
        return;
    }


    cout << ANSI_RED << "\nSyntax Error: " << ANSI_YEL << "Expected \"" << exp_lexeme <<"\", got \"" << tok.lexeme << "\" "
         << ANSI_RED << "[" << ANSI_YEL
         <<  tok.ln << ANSI_RED << ',' << ANSI_YEL << tok.col
         << ANSI_RED "]"  << '\n';
}

Token Parser::consNextToken() {
    if (next_token_index < tokens_length)
        return tokens[next_token_index++];
    
    Token empty_token = {TOK_EMPTY,"",-1,-1};
    return empty_token;
}
Token Parser::peekNextToken() {
    
    if (next_token_index < tokens_length)
        return tokens[next_token_index];
    
    Token empty_token = {TOK_EMPTY,"",-1,-1};
    return empty_token;
}
Token Parser::getCurrentToken() {
    if (next_token_index > 0)
        return tokens[next_token_index-1];
    
    Token empty_token = {TOK_EMPTY,"",-1,-1};
    return empty_token;
}

shared_ptr<ASTNode> Parser::parseProgram() {

    auto programNode = make_shared<ASTNode>(AST_PROGRAM);
    auto blockNode = make_shared<ASTNode>(AST_BLOCK);
    programNode->add_child(blockNode);

    // Remove all comment tokens.
    tokens.erase(remove_if(tokens.begin(), tokens.end(), [](const Token& tok) { return tok.type == TOK_COMMENT;}), tokens.end());
    tokens_length = tokens.size();

    while (peekNextToken().type != TOK_EMPTY) {
        
        if (!parseStmt(blockNode)) {
            printError();
            return nullptr;
        }
    }

    return programNode;
}
bool Parser::parseStmt(shared_ptr<ASTNode> node) {
    
    Token tok = peekNextToken();
    bool expr;

    switch (tok.type) {
        case TOK_LET: { //Variable Declaration.
            expr = parseVarDecl(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_ID: { //Variable Assignment.
            expr = parseAssign(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_PRINT: { //Print Statement.
            expr = parsePrintStmt(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_IF: { //If Statement.
            expr = parseIfStmt(node);
        } break;

        case TOK_FOR: { //For Loop.
            expr = parseForStmt(node);
        } break;

        case TOK_WHILE: { //While Loop.
            expr = parseWhileStmt(node);
        } break;

        case TOK_RETURN: { //Return Statement.
            expr = parseReturnStmt(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_FN: { //Function Declaration.
            expr = parseFuncDecl(node);
        } break;

        case TOK_OPEN_CURLY: { //Block
            expr = parseBlock(node);
        } break;

        default: return false;
    }   

    return expr;
}
bool Parser::parseVarDecl(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_VAR_DECL);

    bool expr = (consNextToken().type == (expTyp=TOK_LET)  ) &&
                (parseIdentifier(new_node)                 ) &&
                (consNextToken().type == (expTyp=TOK_COLON)) &&
                (parseType(new_node)                       ) &&
                (consNextToken().type == (expTyp=TOK_EQUAL)) &&
                (parseExpr(new_node)                       );

    //Set VarDecl attribute to the name of the variable being declared.
    new_node->attr  = new_node->children[0]->attr;
    new_node->token = new_node->children[0]->token;
    node->add_child(new_node);

    return expr;
}
bool Parser::parseAssign(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_ASSIGN);

    bool expr = (parseIdentifier(new_node)                     ) &&
                (consNextToken().type == (expTyp=TOK_EQUAL)    ) &&
                (parseExpr(new_node)                           );

    node->add_child(new_node);

    return expr;
}
bool Parser::parsePrintStmt(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_PRINT_STMT);
    Token tok = consNextToken();
    
    bool expr = (tok.type == (expTyp=TOK_PRINT)                ) &&
                (parseExpr(new_node)                           );

    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}
bool Parser::parseIfStmt(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_IF_STMT);
    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_IF)                       ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(new_node)                               ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(new_node)                              ) &&

                ((peekNextToken().type != TOK_ELSE                 ) || //(*)
                   ((consNextToken().type == (expTyp=TOK_ELSE)     ) &&
                   (parseBlock(new_node)                         )));

    new_node->token = tok;
    node->add_child(new_node);

    return expr;
    /*
        (*) Treat this OR like an if statement.
        If peekNextToken() != TOK_ELSE (i.e the next token is not an else),
        then the expresion won't be evlauated further, since (True || ...) always evaluates to True.

        If peekNextToken() == TOK_ELSE (i.e the next token is an else) then the program will attempt to validate
        the next clause such that (False || ...) will return True. If the following clauses return True, (i.e there is a valid else branch),
        then the expression will be true. Else if the following clauses return false, then the whole if statement is incorrect. 
    */
}
bool Parser::parseForStmt(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_FOR_STMT);
    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_FOR)                      ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                 
                ((peekNextToken().type != TOK_LET                  ) ||
                    (parseVarDecl(new_node)                       )) &&
                 
                (consNextToken().type == (expTyp=TOK_SEMICOLON)    ) &&
                (parseExpr(new_node)                               ) &&
                (consNextToken().type == (expTyp=TOK_SEMICOLON)    ) &&
 
                ((peekNextToken().type != TOK_ID                   ) ||
                    (parseAssign(new_node)                        )) &&
                
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(new_node)                              );

    new_node->token = tok;
    node->add_child(new_node);

    return expr;
                
}
bool Parser::parseWhileStmt(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_WHILE_STMT);
    Token tok = consNextToken();


    bool expr = (tok.type == (expTyp=TOK_WHILE)                    ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(new_node)                               ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(new_node)                              );
    
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}
bool Parser::parseReturnStmt(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_RETURN_STMT);
    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_RETURN)) &&
                (parseExpr(new_node)                        );
    
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}

bool Parser::parseFuncDecl(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_FUNC_DECL);

    bool expr = (consNextToken().type == (expTyp=TOK_FN)           ) &&
                (parseIdentifier(new_node)                         ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&

                ((peekNextToken().type != TOK_ID                   ) ||
                    (parseFormalParams(new_node)                  )) &&

                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (consNextToken().type == (expTyp=TOK_ARROW)        ) &&
                (parseType(new_node)                               ) &&
                (parseBlock(new_node)                              );

    //Get IdToken attached to new_node
    new_node->token = new_node->children[0]->token;
    new_node->attr  = new_node->children[0]->token.lexeme;

    node->add_child(new_node);

    return expr;
}
bool Parser::parseFuncCall(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_FUNC_CALL);

    bool expr = ((parseIdentifier(new_node)                         ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) )) &&
                
                ((peekNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) ||
                    (parseActualParams(new_node)                   )) &&
                
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET));
    
    node->add_child(new_node);
    
    return expr;
}
bool Parser::parseFormalParams(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_FORMAL_PARAMS);

    if (!parseFormalParam(new_node))
        return false;
    
    while (peekNextToken().type == (expTyp=TOK_COMMA)) {
        if (!(consNextToken().type == (expTyp=TOK_COMMA) && parseFormalParam(new_node))) {
            return false;
        }
    }
    
    node->add_child(new_node);

    return true;
}
bool Parser::parseActualParams(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_ACTUAL_PARAMS);

    if (!parseExpr(new_node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_COMMA)) {
        if (!(consNextToken().type == (expTyp=TOK_COMMA) && parseExpr(new_node))) {
            return false;
        }
    }

    node->add_child(new_node);
    
    return true;
}
bool Parser::parseFormalParam(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_FORMAL_PARAM);

    bool expr = (parseIdentifier(new_node)                 ) &&
                (consNextToken().type == (expTyp=TOK_COLON)) &&
                (parseType(new_node)                       );
    
    node->add_child(new_node);

    return expr;
}

bool Parser::parseBlock(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_BLOCK);

    if (consNextToken().type != TOK_OPEN_CURLY)
        return false;


    while (peekNextToken().type != TOK_CLOSE_CURLY) {
        if (!parseStmt(new_node)) {
            return false;
        }
    }
    
    if (consNextToken().type != TOK_CLOSE_CURLY)
        return false;
    
    node->add_child(new_node);

    return true;
    
}
bool Parser::parseIdentifier(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_IDENTIFIER);
    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_ID));

    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}

bool Parser::parseExpr(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_EXPR);

    if (!parseSimpleExpr(new_node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_RELOP)) {
        if (!(parseRelOp(new_node) && parseSimpleExpr(new_node))){
            return false;
        }
    }

    node->add_child(new_node);

    return true;
}
bool Parser::parseSimpleExpr(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_SIMPLE_EXPR);

    if (!parseTerm(new_node))
        return false;
    
    while (peekNextToken().type == (expTyp=TOK_ADDOP)) {
        if (!(parseAddOp(new_node) && parseTerm(new_node))) {
            return false;
        }
    }

    node->add_child(new_node);

    return true;
}
bool Parser::parseSubExpr(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_SUB_EXPR);

    bool expr = (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(new_node)                               ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET));

    node->add_child(new_node);

    return expr;
}

bool Parser::parseTerm(shared_ptr<ASTNode> node) {

    auto new_node = make_shared<ASTNode>(AST_TERM);

    if (!parseFactor(new_node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_MULOP)) {
        if (!(parseMulOp(new_node) && parseFactor(new_node))) {
            return false;
        }
    }

    node->add_child(new_node);

    return true;
}
bool Parser::parseFactor(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_FACTOR);
    size_t this_token_index = next_token_index;

    /*Attempt to match with Literal, Identifier, FunctionCall, SubExpr or Unary.
    Another approach was to check the next token, but it would be too long since
    literal expands to 4 more non-terminals */
    if (parseLit(new_node)) {
        node->add_child(new_node);
        return true;
    } 

    next_token_index = this_token_index;
    new_node->children.clear();

    //Order is important. FuncCall must be before Identfier because FuncCall ⊆ Identifier.
    if (parseFuncCall(new_node)) {
        node->add_child(new_node);
        return true;
    } 

    next_token_index = this_token_index;
    new_node->children.clear();

    if (parseIdentifier(new_node)) {
        node->add_child(new_node);
        return true;
    } 

    next_token_index = this_token_index;
    new_node->children.clear();

    if (parseSubExpr(new_node)) {
        node->add_child(new_node);
        return true;
    } 

    next_token_index = this_token_index;
    new_node->children.clear();

    if (parseUnOp(new_node)) {
        node->add_child(new_node);
        return true;
    } 

    next_token_index = this_token_index;
    new_node->children.clear();

    //In order to give correct error messages. Show that the next token should have been an ID.
    expTyp = TOK_ID;
    consNextToken();

    node->add_child(new_node);

    return false;

}
bool Parser::parseMulOp(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_MULOP);
    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_MULOP));

    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}
bool Parser::parseAddOp(shared_ptr<ASTNode> node) {
    
    auto new_node = make_shared<ASTNode>(AST_ADDOP);

    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_ADDOP));

    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);
    
    return expr;
}
bool Parser::parseUnOp(shared_ptr<ASTNode> node) {

    Token tok = consNextToken();
    auto new_node = make_shared<ASTNode>(AST_UNOP);

    bool expr = ((tok.type == (expTyp=TOK_ADDOP) && tok.lexeme == "-") ||
                 (tok.type == (expTyp=TOK_UNOP)                     )) &&
                 (parseExpr(new_node)                                );

    //ATTACHING WHOLE EXPRESSION TO UNOP?? SHOULD ONLY ATTACH IMMEDIATE TOKEN

    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return expr;   
}
bool Parser::parseRelOp(shared_ptr<ASTNode> node) {

    Token tok = consNextToken();
    auto new_node = make_shared<ASTNode>(AST_RELOP);

    bool expr = (tok.type == (expTyp=TOK_RELOP));

    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}

bool Parser::parseLit(shared_ptr<ASTNode> node) {

    Token tok = consNextToken();
    shared_ptr<ASTNode> new_node;

    switch (tok.type) {
        case TOK_BOOL_LIT:  new_node = make_shared<ASTNode>(AST_BOOL_LIT);  break;
        case TOK_INT_LIT:   new_node = make_shared<ASTNode>(AST_INT_LIT);   break;
        case TOK_CHAR_LIT:  new_node = make_shared<ASTNode>(AST_CHAR_LIT);  break;
        case TOK_FLOAT_LIT: new_node = make_shared<ASTNode>(AST_FLOAT_LIT); break;

        default: return false;
    }
    
    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return true;
}
bool Parser::parseType(shared_ptr<ASTNode> node) {

    Token tok = consNextToken();
    auto new_node = make_shared<ASTNode>(AST_TYPE);

    bool expr = (tok.type == (expTyp=TOK_FLOAT)) ||
                (tok.type == (expTyp=TOK_INT)  ) ||
                (tok.type == (expTyp=TOK_BOOL) ) ||
                (tok.type == (expTyp=TOK_CHAR) );


    new_node->attr = tok.lexeme;
    new_node->token = tok;
    node->add_child(new_node);

    return expr;
}

