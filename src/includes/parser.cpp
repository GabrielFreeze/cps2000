#include "parser.hpp"


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
             << ANSI_RED << "[" << ANSI_ESC << ANSI_YEL
             << tok.ln << ANSI_ESC << ANSI_RED << ',' << ANSI_ESC << ANSI_YEL << tok.col
             << ANSI_ESC << ANSI_RED "]"  << ANSI_ESC << '\n';
        
        return;
    }


    cout << ANSI_RED << "\nSyntax Error: " << ANSI_ESC << ANSI_YEL << "Expected \"" << exp_lexeme <<"\", got \"" << tok.lexeme << "\" " << ANSI_ESC
         << ANSI_RED << "[" << ANSI_ESC << ANSI_YEL
         <<  tok.ln << ANSI_ESC << ANSI_RED << ',' << ANSI_ESC << ANSI_YEL << tok.col
         << ANSI_ESC << ANSI_RED "]"  << ANSI_ESC << '\n';
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

bool Parser::parseProgram() {

    ASTNode programNode(AST_PROGRAM);


    // Remove all comment tokens.
    tokens.erase(remove_if(tokens.begin(), tokens.end(), [](const Token& tok) { return tok.type == TOK_COMMENT;}), tokens.end());
    tokens_length = tokens.size();

    while (peekNextToken().type != TOK_EMPTY) {
        
        if (!parseStmt(programNode)) {
            printError();
            return false;
        }
    }


    return true;
}
bool Parser::parseStmt(ASTNode node) {
    
    Token tok = peekNextToken();
    bool expr;

    switch (tok.type) {
        case TOK_LET: {
            expr = parseVarDecl(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_ID: {
            expr = parseAssign(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_PRINT: {
            expr = parsePrintStmt(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_IF: {
            expr = parseIfStmt(node);
        } break;

        case TOK_FOR: {
            expr = parseForStmt(node);
        } break;

        case TOK_WHILE: {
            expr = parseWhileStmt(node);
        } break;

        case TOK_RETURN: {
            expr = parseReturnStmt(node) &&
                   (consNextToken().type == (expTyp=TOK_SEMICOLON));
        } break;

        case TOK_FN: {
            expr = parseFuncDecl(node);
        } break;

        case TOK_OPEN_CURLY: {
            expr = parseBlock(node);
        } break;

        default: return false;
    }   

    return expr;
}
bool Parser::parseVarDecl(ASTNode node) {
    
    bool expr = (consNextToken().type == (expTyp=TOK_LET)      ) &&
                (parseIdentifier(node)                         ) &&
                (consNextToken().type == (expTyp=TOK_COLON)    ) &&
                (parseType(node)                               ) &&
                (consNextToken().type == (expTyp=TOK_EQUAL)    ) &&
                (parseExpr(node)                               );

    return expr;
}
bool Parser::parseAssign(ASTNode node) {
    
    bool expr = (parseIdentifier(node)                         ) &&
                (consNextToken().type == (expTyp=TOK_EQUAL)    ) &&
                (parseExpr(node)                               );

    return expr;
}
bool Parser::parsePrintStmt(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_PRINT)    ) &&
                (parseExpr(node)                               );

    return expr;
}
bool Parser::parseIfStmt(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_IF)           ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(node)                                   ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(node)                                  ) &&

                ((peekNextToken().type != TOK_ELSE                 ) || //(*)
                   ((consNextToken().type == (expTyp=TOK_ELSE)     ) &&
                   (parseBlock(node)                               )));


    return expr;
    /*
        (*) Treat this OR like an if statement.
        If peekNextToken() != TOK_ELSE (i.e the next token is not an else),
        then the expresion won't be evlauated further, since (True || ...) always evaluates to True.

        If peekNextToken() == TOK_ELSE (i.e the next token is an else) then the program will attempt to validate
        the next clause such that (False || ...) will return True. If the following clauses return True, (i.e there is a valid else branch),
        then the expression will be true. Else if the following clauses return false, then the whole if statement is incorrect. 
    */

    // //Parse If Statement
    // bool exprA = (consNextToken() == TOK_IF           ) &&
    //              (consNextToken() == TOK_OPEN_BRACKET ) &&
    //              (parseExpr()                         ) &&
    //              (consNextToken() == TOK_CLOSE_BRACKET) &&
    //              (parseBlock()                        );

    // bool exprB = True;

    // //Check if there is an else block.
    // if (peekNextToken() == TOK_ELSE) {
        
    //     exprB = (consNextToken() == TOK_ELSE) &&
    //             (parseBlock()               );
    // }

    // return exprA && exprB;
}
bool Parser::parseForStmt(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_FOR)          ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                 
                ((peekNextToken().type != TOK_LET                  ) ||
                    (parseVarDecl(node)                            )) &&
                 
                (consNextToken().type == (expTyp=TOK_SEMICOLON)    ) &&
                (parseExpr(node)                                   ) &&
                (consNextToken().type == (expTyp=TOK_SEMICOLON)    ) &&
 
                ((peekNextToken().type != TOK_ID                   ) ||
                    (parseAssign(node)                             )) &&
                
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(node)                                  );



    return expr;
                
}
bool Parser::parseWhileStmt(ASTNode node) {
    
    bool expr = (consNextToken().type == (expTyp=TOK_WHILE)        ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(node)                                   ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (parseBlock(node)                                  );
    
    return expr;
}
bool Parser::parseReturnStmt(ASTNode node) {
    
    bool expr = (consNextToken().type == (expTyp=TOK_RETURN)) &&
                (parseExpr(node)                            );
    
    return expr;
}

bool Parser::parseFuncDecl(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_FN)           ) &&
                (parseIdentifier(node)                             ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&

                ((peekNextToken().type != TOK_ID                   ) ||
                    (parseFormalParams(node)                      )) &&

                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) &&
                (consNextToken().type == (expTyp=TOK_ARROW)        ) &&
                (parseType(node)                                   ) &&
                (parseBlock(node)                                  );

    return expr;
}
bool Parser::parseFuncCall(ASTNode node) {

    bool expr = (parseIdentifier(node                               ) &&
                (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) )) &&
                
                ((peekNextToken().type == (expTyp=TOK_CLOSE_BRACKET)) ||
                    (parseActualParams(node)                       )) &&
                
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET));
    
    
    return expr;
}
bool Parser::parseFormalParams(ASTNode node) {

    if (!parseFormalParam(node))
        return false;
    
    while (peekNextToken().type == (expTyp=TOK_COMMA)) {
        if (!(consNextToken().type == (expTyp=TOK_COMMA) && parseFormalParam(node))) {
            return false;
        }
    }

    return true;
}
bool Parser::parseActualParams(ASTNode node) {
    
    if (!parseExpr(node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_COMMA)) {
        if (!(consNextToken().type == (expTyp=TOK_COMMA) && parseExpr(node))) {
            return false;
        }
    }

    
    return true;
}
bool Parser::parseFormalParam(ASTNode node) {
    
    bool expr = (parseIdentifier(node)                     ) &&
                (consNextToken().type == (expTyp=TOK_COLON)) &&
                (parseType(node)                           );
    
    return expr;
}

bool Parser::parseBlock(ASTNode node) {

    if (consNextToken().type != TOK_OPEN_CURLY)
        return false;


    while (peekNextToken().type != TOK_CLOSE_CURLY) {
        if (!parseStmt(node)) {
            return false;
        }
    }
    
    if (consNextToken().type != TOK_CLOSE_CURLY)
        return false;
    
    return true;
    
}
bool Parser::parseIdentifier(ASTNode node) {
    
    bool expr = (consNextToken().type == (expTyp=TOK_ID));

    return expr;
}

bool Parser::parseExpr(ASTNode node) {

    if (!parseSimpleExpr(node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_RELOP)) {
        if (!(parseRelOp(node) && parseSimpleExpr(node))){
            return false;
        }
    }

    return true;
}
bool Parser::parseSimpleExpr(ASTNode node) {
    
    if (!parseTerm(node))
        return false;
    
    while (peekNextToken().type == (expTyp=TOK_ADDOP)) {
        if (!(parseAddOp(node) && parseTerm(node))) {
            return false;
        }
    }

    return true;
}
bool Parser::parseSubExpr(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_OPEN_BRACKET) ) &&
                (parseExpr(node)                                   ) &&
                (consNextToken().type == (expTyp=TOK_CLOSE_BRACKET));

    return expr;
}

bool Parser::parseTerm(ASTNode node) {

    if (!parseFactor(node))
        return false;

    while (peekNextToken().type == (expTyp=TOK_MULOP)) {
        if (!(parseMulOp(node) && parseFactor(node))) {
            return false;
        }
    }

    return true;
}
bool Parser::parseFactor(ASTNode node) {
    size_t this_token_index = next_token_index;

    /*Attempt to match with Literal, Identifier, FunctionCall, SubExpr or Unary.
    Another approach was to check the next token, but it would be too long since
    literal expands to 4 more non-terminals */
    if (parseLit(node)) return true;
    next_token_index = this_token_index;

    //Order is important. FuncCall must be before Identfier because FuncCall ⊆ Identifier.
    if (parseFuncCall(node)) return true; 
    next_token_index = this_token_index;

    if (parseIdentifier(node)) return true;
    next_token_index = this_token_index;

    if (parseSubExpr(node)) return true;
    next_token_index = this_token_index;

    if (parseUnOp(node)) return true;
    next_token_index = this_token_index;

    //In order to give correct error messages. Show that the next token should have been an ID.
    expTyp = TOK_ID;
    consNextToken();

    return false;

}
bool Parser::parseMulOp(ASTNode node) {
 
    bool expr = (consNextToken().type == (expTyp=TOK_MULOP));

    return expr;
}
bool Parser::parseAddOp(ASTNode node) {
    
    bool expr = (consNextToken().type == (expTyp=TOK_ADDOP));

    return expr;
}
bool Parser::parseUnOp(ASTNode node) {

    Token tok = consNextToken();

    bool expr = ((tok.type == (expTyp=TOK_ADDOP) && tok.lexeme == "-") ||
                 (tok.type == (expTyp=TOK_UNOP)                     )) &&
                 (parseExpr(node)                                        );

    return expr;   
}
bool Parser::parseRelOp(ASTNode node) {

    bool expr = (consNextToken().type == (expTyp=TOK_RELOP));

    return expr;
}

bool Parser::parseLit(ASTNode node) {

    Token tok = consNextToken();
    
    bool expr = (tok.type == (expTyp=TOK_BOOL_LIT) ) ||
                (tok.type == (expTyp=TOK_INT_LIT)  ) ||
                (tok.type == (expTyp=TOK_FLOAT_LIT)) ||
                (tok.type == (expTyp=TOK_CHAR_LIT) );
                
    return expr;
}
bool Parser::parseType(ASTNode node) {

    Token tok = consNextToken();

    bool expr = (tok.type == (expTyp=TOK_FLOAT)) ||
                (tok.type == (expTyp=TOK_INT)  ) ||
                (tok.type == (expTyp=TOK_BOOL) ) ||
                (tok.type == (expTyp=TOK_CHAR) );

    return expr;
}
