#pragma once

#include "lexer.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;


enum identifier_type    {ID_INT, ID_FLOAT, ID_CHAR, ID_BOOL, ID_FUNC, ID_EMPTY};
enum return_type        {RETURN_INT, RETURN_FLOAT, RETURN_CHAR, RETURN_BOOL, RETURN_EMPTY};

enum semantic_error_msg {SEMERR_FUNC_DECL_ALREADY_DECLARED,
                         SEMERR_FUNC_DECL_NO_RETURN,
                         SEMERR_VAR_DECL_ALREADY_DECLARED,
                         SEMERR_VAR_DECL_IDENTICAL_FUNCTION};

typedef struct symbol_ {
    string identifier;
    identifier_type type;
    float value;
    Token token;
} symbol;

typedef struct symbolFunc_ {
    string identifier;
    return_type type;
    int params;
    Token token;
} symbolFunc;


class ScopeStk {
    public:
        ScopeStk();

        map<string,symbol> getTop();
        map<string,symbol> pop();
        void push(map<string,symbol> value);

        int isDecl(string id);
        int isFuncDecl(string id);
        

        symbolFunc getFuncAttr(string id);
        map<string,symbolFunc> functions;

        Token errToken;
        semantic_error_msg errMsg;
        Token corrToken;

        void addFunction(string id, int params, return_type type_to_return, Token token);
        void addSymbol(string id, identifier_type typ, float value, Token token);

    private:
        vector<map<string,symbol>> scopeStk;

};


