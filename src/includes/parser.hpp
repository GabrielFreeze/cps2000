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
        shared_ptr<ASTNode> parseProgram();


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

        bool parseType(shared_ptr<ASTNode> node);
        bool parseBooleanLit(shared_ptr<ASTNode> node);
        bool parseIntegerLit(shared_ptr<ASTNode> node);
        bool parseFloatLit(shared_ptr<ASTNode> node);
        bool parseCharLit(shared_ptr<ASTNode> node);
        bool parseLit(shared_ptr<ASTNode> node);
        bool parseIdentifier(shared_ptr<ASTNode> node);
        bool parseMulOp(shared_ptr<ASTNode> node);
        bool parseAddOp(shared_ptr<ASTNode> node);
        bool parseRelOp(shared_ptr<ASTNode> node);
        bool parseActualParams(shared_ptr<ASTNode> node);
        bool parseFuncCall(shared_ptr<ASTNode> node);
        bool parseSubExpr(shared_ptr<ASTNode> node);
        bool parseUnOp(shared_ptr<ASTNode> node);
        bool parseFactor(shared_ptr<ASTNode> node);
        bool parseTerm(shared_ptr<ASTNode> node);
        bool parseSimpleExpr(shared_ptr<ASTNode> node);
        bool parseExpr(shared_ptr<ASTNode> node);
        bool parseAssign(shared_ptr<ASTNode> node);
        bool parseVarDecl(shared_ptr<ASTNode> node);
        bool parsePrintStmt(shared_ptr<ASTNode> node);
        bool parseReturnStmt(shared_ptr<ASTNode> node);
        bool parseIfStmt(shared_ptr<ASTNode> node);
        bool parseForStmt(shared_ptr<ASTNode> node);
        bool parseWhileStmt(shared_ptr<ASTNode> node);
        bool parseFormalParam(shared_ptr<ASTNode> node);
        bool parseFormalParams(shared_ptr<ASTNode> node);
        bool parseFuncDecl(shared_ptr<ASTNode> node);
        bool parseStmt(shared_ptr<ASTNode> node);
        bool parseBlock(shared_ptr<ASTNode> node);

};


