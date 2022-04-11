#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

enum lexer_cat {CAT_NONE, CAT_LETTER, CAT_DIGIT, CAT_ASCII_OTHER};

#define TRANS_SIZE 24
enum transition {TRANS_LETTER, TRANS_DIGIT, TRANS_ASCII_OTHER, TRANS_OPEN_BRACKET, TRANS_CLOSE_BRACKET,
                 TRANS_OPEN_CURLY, TRANS_CLOSE_CURLY, TRANS_SEMICOLON, TRANS_COLON, TRANS_COMMA,
                 TRANS_QUOTE, TRANS_EQUAL, TRANS_PERIOD, TRANS_BANG, TRANS_DIV, TRANS_MUL, TRANS_ADD,
                 TRANS_SUB, TRANS_LESS, TRANS_GREATER, TRANS_UNDERSCORE, TRANS_TAB, TRANS_NEWLINE, TRANS_SPACE};

enum token {TOK_OPEN_BRACKET, TOK_CLOSE_BRACKET,
            TOK_OPEN_CURLY, TOK_CLOSE_CURLY,
            TOK_SEMICOLON, TOK_COLON, TOK_COMMA,
            TOK_EQUAL, TOK_NUM, TOK_CONST, TOK_MULOP,
            TOK_ADDOP, TOK_RELOP, TOK_ARROW, TOK_COMMENT,
            TOK_ID}; //TOK_ID encapsulates identifiers and keywords. Distinction is made in the Parser.

enum state {S_START, S_A, S_B, S_C, S_D, S_E, S_F, S_G, S_EMPTY, S_ID,
            S_OPEN_BRACKET, S_CLOSE_BRACKET, S_OPEN_CURLY, S_CLOSE_CURLY,
            S_SEMICOLON, S_COLON, S_COMMA, S_EQUAL, S_NUM, S_CONST,
            S_MULOP_A, S_MULOP_B,
            S_ADDOP_A, S_ADDOP_B,
            S_RELOP_A, S_RELOP_B, S_RELOP_C,
            S_ARROW, S_COMMENT};




class Lexer {

    public:

        Lexer();
        lexer_cat getCat(char x);
        bool isFinal(state s);

        vector<vector<state>> dfa;
        vector<string> keywords;
        
    




    




};