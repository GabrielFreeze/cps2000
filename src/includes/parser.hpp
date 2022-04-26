#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>

#include "lexer.hpp"
#include "ASTNode.hpp"

using namespace std;


class Parser {
    public:
        Parser(vector<Token>& tokens);
        bool parseProgram();


    private:
        node_type current_ASTNode;
        vector<Token> tokens;
        size_t next_token_index;
        size_t tokens_length;

        //For error reporting
        Token errToken;
        token_type expTyp;
        void printError();
        void setErrToken();

        Token consNextToken();
        Token peekNextToken();
        Token getCurrentToken();

        bool parseType(ASTNode node);
        bool parseBooleanLit(ASTNode node);
        bool parseIntegerLit(ASTNode node);
        bool parseFloatLit(ASTNode node);
        bool parseCharLit(ASTNode node);
        bool parseLit(ASTNode node);
        bool parseIdentifier(ASTNode node);
        bool parseMulOp(ASTNode node);
        bool parseAddOp(ASTNode node);
        bool parseRelOp(ASTNode node);
        bool parseActualParams(ASTNode node);
        bool parseFuncCall(ASTNode node);
        bool parseSubExpr(ASTNode node);
        bool parseUnOp(ASTNode node);
        bool parseFactor(ASTNode node);
        bool parseTerm(ASTNode node);
        bool parseSimpleExpr(ASTNode node);
        bool parseExpr(ASTNode node);
        bool parseAssign(ASTNode node);
        bool parseVarDecl(ASTNode node);
        bool parsePrintStmt(ASTNode node);
        bool parseReturnStmt(ASTNode node);
        bool parseIfStmt(ASTNode node);
        bool parseForStmt(ASTNode node);
        bool parseWhileStmt(ASTNode node);
        bool parseFormalParam(ASTNode node);
        bool parseFormalParams(ASTNode node);
        bool parseFuncDecl(ASTNode node);
        bool parseStmt(ASTNode node);
        bool parseBlock(ASTNode node);

};


