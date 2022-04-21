#include "parser.hpp"

Parser::Parser(vector<Token> tokens) {
    current_ASTNode = AST_PROGRAM; //The starting node.
    this->tokens = tokens; //Pass all tokens to the parser before parsing begins.
    tokens_length = tokens.size();
}

void Parser::printError() {
    cout << "Implement me =)";
}

Token Parser::getNextToken() {
    
    if (current_token_index < tokens_length) {
        return tokens[current_token_index++];
    }

    return TOK_EMPTY; //TOK_EMPTY shows that there are no more tokens to be returned.
}


bool Parser::parseProgram() {

    while (getNextToken() != TOK_EMPTY) {
        
        if (!parseStmt()) {
            printError();
            return false;
        }

    }

    return true;
}