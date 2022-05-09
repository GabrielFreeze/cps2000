#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include "colors.hpp"
#include <bits/stdc++.h>

using namespace std;

#define TRANS_SIZE 25
enum transition {TRANS_LETTER, TRANS_DIGIT, TRANS_ASCII_OTHER, TRANS_OPEN_BRACKET, TRANS_CLOSE_BRACKET,
                 TRANS_OPEN_CURLY, TRANS_CLOSE_CURLY, TRANS_SEMICOLON, TRANS_COLON, TRANS_COMMA,
                 TRANS_QUOTE, TRANS_EQUAL, TRANS_PERIOD, TRANS_BANG, TRANS_DIV, TRANS_MUL, TRANS_ADD,
                 TRANS_SUB, TRANS_LESS, TRANS_GREATER, TRANS_UNDERSCORE, TRANS_TAB, TRANS_NEWLINE, TRANS_SPACE,
                 TRANS_EMPTY}; //TRANS_EMPTY is returned when given a symbol s, the lexer can't deduce which transition corresponds to s.

enum token_type {TOK_OPEN_BRACKET, TOK_CLOSE_BRACKET,
                 TOK_OPEN_CURLY, TOK_CLOSE_CURLY,
                 TOK_SEMICOLON, TOK_COLON, TOK_COMMA,
                 TOK_EQUAL, TOK_INT_LIT, TOK_FLOAT_LIT, TOK_CHAR_LIT, TOK_MULOP, TOK_UNOP,
                 TOK_ADDOP, TOK_RELOP, TOK_ARROW, TOK_COMMENT, TOK_BOOL_LIT,
                 TOK_ID, TOK_FOR, TOK_WHILE, TOK_IF, TOK_ELSE, TOK_LET, TOK_PRINT, TOK_RETURN, TOK_FN,
                 TOK_FLOAT, TOK_INT, TOK_BOOL, TOK_CHAR,
                 TOK_EMPTY}; //TOK_EMPTY is returned when a token cannot be returned.

#define STATE_SIZE 30
enum state {S_START, S_A, S_B, S_C, S_D, S_E, S_F, S_EMPTY, S_ID,
            S_OPEN_BRACKET, S_CLOSE_BRACKET, S_OPEN_CURLY, S_CLOSE_CURLY,
            S_SEMICOLON, S_COLON, S_COMMA, S_EQUAL, S_INT, S_FLOAT, S_CHAR_LIT,
            S_MULOP_A, S_MULOP_B,
            S_ADDOP_A, S_ADDOP_B,
            S_RELOP_A, S_RELOP_B, S_RELOP_C,
            S_ARROW, S_COMMENT_A, S_COMMENT_B};



typedef struct _token {
    token_type type;
    string lexeme;
    int ln;
    int col;
} Token;





class Lexer {

    public:

        Lexer();

        bool isFinal(state s);
        transition getTransition(char x);
        vector<Token> getTokens(string input_text);
        token_type state_to_token_type(state s);
        state nextState(state s, transition t);

        void printToken(vector<Token> tokens);
        token_type id_to_keyword(string lexeme);

        vector<vector<state>> dfa;
        vector<string> keywords;
        vector<token_type> keyword_types;
        vector<state> final;

        state start, empty, last_final, current_state;



};