#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>

#include "lexer.hpp"

using namespace std;

class Parser {
    public:
        Parser(vector<Token> tokens);


    private:
        node_type current_ASTNode;
        vector<Token> tokens;
        size_t next_token_index;
        size_t tokens_length;


        //For error reporting
        Token errToken;
        void printError();
        void setErrToken();


        Token consNextToken();
        Token peekNextToken();
        Token getCurrentToken();


        bool parseLetter();
        bool parseDigit();
        bool parsePrintable();
        bool parseType();
        bool parseBooleanLit();
        bool parseIntegerLit();
        bool parseFloatLit();
        bool parseCharLit();
        bool parseLit();
        bool parseIdentifier();
        bool parseMulOp();
        bool parseAddOp();
        bool parseRelOp();
        bool parseActualParams();
        bool parseFuncCall();
        bool parseSubExpr();
        bool parseUnOp();
        bool parseFactor();
        bool parseTerm();
        bool parseSimpleExpr();
        bool parseExpr();
        bool parseAssign();
        bool parseVarDecl();
        bool parsePrintStmt();
        bool parseReturnStmt();
        bool parseIfStmt();
        bool parseForStmt();
        bool parseWhileStmt();
        bool parseFormalParam();
        bool parseFormalParams();
        bool parseFuncDecl();
        bool parseStmt();
        bool parseBlock();
        bool parseProgram();






};


enum node_type {AST_LETTER, AST_DIGIT, AST_PRINTABLE, AST_TYPE, AST_BOOL_LIT, AST_INT_LIT,
                AST_FLOAT_LIT, AST_CHAR_LIT, AST_LIT, AST_IDENTIFIER, AST_MULOP,
                AST_ADDOP, AST_RELOP, AST_ACTUAL_PARAMS, AST_FUNC_CALL, AST_SUB_EXPR, AST_UNOP,
                AST_FACTOR, AST_TERM, AST_SIMPLE_EXPR, AST_EXPR, AST_ASSIGN, AST_VAR_DECL, AST_PRINT_STMT,
                AST_RETURN_STMT, AST_IF_STMT, AST_FOR_STMT, AST_WHILE_STMT, AST_FORMAL_PARAM,
                AST_FORMAL_PARAMS, AST_FUNC_DECL, AST_STMT, AST_BLOCK, AST_PROGRAM};


typedef struct ASTNode_ {
    node_type type;
    vector<shared_ptr<ASTNode>> children;
    shared_ptr<ASTNode> parent;
    string literal;
} ASTNode;
