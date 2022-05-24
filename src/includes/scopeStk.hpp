#pragma once

#include "lexer.hpp"
#include "ASTNode.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;


enum identifier_type    {ID_EMPTY, ID_BOOL, ID_CHAR, ID_INT, ID_FLOAT, ID_FUNC}; //DO NOT CHANGE ORDER
enum return_type        {RETURN_INT, RETURN_FLOAT, RETURN_CHAR, RETURN_BOOL, RETURN_EMPTY};

enum semantic_error_msg {SEMERR_FUNC_DECL_ALREADY_DECLARED,
                         SEMERR_FUNC_DECL_NO_RETURN,
                         SEMERR_VAR_DECL_ALREADY_DECLARED,
                         SEMERR_VAR_DECL_IDENTICAL_FUNCTION,
                         SEMERR_ID_NOT_DECLARED,
                         SEMERR_FUNC_CALL_NOT_DECLARED,
                         SEMERR_FUNC_CALL_WRONG_PARAM,
                         SEMERR_TYPE_INVALID_CHAR_CONVERSION};

typedef struct value_ {
    float data;
    identifier_type type;
} value;

typedef struct symbol_ {
    string identifier;
    identifier_type type;
    value val;
    Token token;
} symbol;

typedef struct symbolFunc_ {
    string identifier;
    return_type type;
    int params;
    Token token;
    shared_ptr<ASTNode> block;
} symbolFunc;




class ScopeStk {
    public:
        ScopeStk();

        map<string,symbol>& getTop();
        map<string,symbol> pop();
        void push(map<string,symbol> value);

        symbol getSymbol(string id);
        void updateSymbol(string id, value v);
        int isFuncDecl(string id);
        

        symbolFunc getFuncAttr(string id);
        map<string,symbolFunc> functions;

        Token errToken;
        semantic_error_msg errMsg;
        Token corrToken;
        int errExpectedParams;
        int errActualParams;

        void addFunction(string id, int params, return_type type_to_return, Token token, shared_ptr<ASTNode> block);
        void addSymbol(string id, identifier_type typ, value val, Token token);
        void delSymbol(string id);

    private:
        vector<map<string,symbol>> scopeStk;

};


