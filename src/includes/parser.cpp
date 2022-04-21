#include "parser.hpp"

Parser::Parser(vector<Token> tokens) {
    current_ASTNode = AST_PROGRAM; //The starting node.
    this->tokens = tokens; //Pass all tokens to the parser before parsing begins.
    tokens_length = tokens.size();
}

void Parser::printError() {
    //TODO: Pretty print this.
    cout << "Syntax error on token " << getCurrentToken().lexeme << '\n';
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

    while (peekNextToken().type != TOK_EMPTY) {
        
        if (!parseStmt()) {
            printError();
            return false;
        }
    }

    return true;
}

bool Parser::parseStmt() {
    
    Token tok = consNextToken();

    bool expr = (parseVarDecl()   && tok.type == TOK_SEMICOLON) ||
                (parseAssign()    && tok.type == TOK_SEMICOLON) ||
                (parsePrintStmt() && tok.type == TOK_SEMICOLON) ||
                (parseIfStmt()                                ) ||
                (parseForStmt()                               ) ||
                (parseWhileStmt()                             ) ||
                (parseReturnStmt()                            ) ||
                (parseFuncDecl()                              ) ||
                (parseBlock()                                 );

    return expr;
}
bool Parser::parseVarDecl() {
    
    bool expr = (consNextToken().type == TOK_LET      ) &&
                (parseIdentifier()                    ) &&
                (consNextToken().type == TOK_SEMICOLON) &&
                (parseType()                          ) &&
                (consNextToken().type == TOK_EQUAL    ) &&
                (parseExpr()                          );

    return expr;
}
bool Parser::parseAssign() {
    
    bool expr = (parseIdentifier()                ) &&
                (consNextToken().type == TOK_EQUAL) &&
                (parseExpr()                      );

    return expr;
}
bool Parser::parsePrintStmt() {

    bool expr = (consNextToken().type == TOK_PRINT) &&
                (parseExpr()                      );

    return expr;
}
bool Parser::parseIfStmt() {

    bool expr = (consNextToken().type == TOK_IF           ) &&
                (consNextToken().type == TOK_OPEN_BRACKET ) &&
                (parseExpr()                              ) &&
                (consNextToken().type == TOK_CLOSE_BRACKET) &&
                (parseBlock()                             ) &&

                ((peekNextToken().type != TOK_ELSE        ) || //(*)
                   ((consNextToken().type == TOK_ELSE     ) &&
                   (parseBlock()                          )));


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
bool Parser::parseForStmt() {

    bool expr = (consNextToken().type == TOK_FOR         ) &&
                (consNextToken().type == TOK_OPEN_BRACKET) &&
                
                ((peekNextToken().type != TOK_LET        ) ||
                    (parseVarDecl()                      )) &&
                
                (consNextToken().type == TOK_SEMICOLON   ) &&
                (parseExpr()                             ) &&
                (consNextToken().type == TOK_SEMICOLON   ) &&

                ((peekNextToken().type != TOK_ID         ) ||
                    (parseAssign()                      )) &&
                
                (consNextToken().type == TOK_OPEN_BRACKET) &&
                (parseBlock()                            );


    return expr;
                
}
bool Parser::parseWhileStmt() {
    
    bool expr = (consNextToken().type == TOK_WHILE        ) &&
                (consNextToken().type == TOK_OPEN_BRACKET ) &&
                (parseExpr()                              ) &&
                (consNextToken().type == TOK_CLOSE_BRACKET) &&
                (parseBlock()                             );
    
    return expr;
}
bool Parser::parseReturnStmt() {
    
    bool expr = (consNextToken().type == TOK_RETURN) &&
                (parseExpr()                       );
    
    return expr;
}
bool Parser::parseFuncDecl() {

    bool expr = (consNextToken().type == TOK_FN           ) &&
                (parseIdentifier()                        ) &&
                (consNextToken().type == TOK_OPEN_BRACKET ) &&

                ((peekNextToken().type != TOK_ID          ) ||
                    (parseFormalParams()                 )) &&

                (consNextToken().type == TOK_CLOSE_BRACKET) &&
                (consNextToken().type == TOK_ARROW        ) &&
                (parseType()                              ) &&
                (parseBlock()                             );

    return expr;
}
bool Parser::parseBlock() {

    if (consNextToken().type != TOK_OPEN_CURLY)
        return false;


    while (peekNextToken().type != TOK_CLOSE_CURLY) {
        if (!parseBlock()) {
            return false;
        }
    }

    if (consNextToken().type != TOK_CLOSE_CURLY)
        return false;
    
    return true;
    
}
bool Parser::parseIdentifier() {
    
    bool expr = (consNextToken().type == TOK_ID);

    return expr;
}
bool Parser::parseExpr() {

    if (parseSimpleExpr())
        return false;

    while (peekNextToken().type == TOK_RELOP) {
        if (!(parseRelOp() && parseSimpleExpr())){
            return false;
        }
    }

    return true;
}
bool Parser::parseSimpleExpr() {
    
    if (!parseTerm())
        return false;
    
    while (peekNextToken().type == TOK_ADDOP) {
        if (!(parseAddOp() && parseTerm())) {
            return false;
        }
    }

    return true;
}
bool Parser::parseTerm() {

    if (!parseFactor())
        return false;

    while (peekNextToken().type == TOK_MULOP) {
        if (!(parseMulOp() && parseFactor())) {
            return false;
        }
    }

    return true;
}
bool Parser::parseFactor() {
    
}
bool Parser::parseMulOp() {
 
    bool expr = (consNextToken().type == TOK_MULOP);

    return expr;
}
