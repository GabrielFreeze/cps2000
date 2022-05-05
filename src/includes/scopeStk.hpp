#pragma once

#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;


enum identifier_type  {ID_INT, ID_FLOAT, ID_CHAR, ID_BOOL, ID_FUNC, ID_EMPTY};


class ScopeStk {
    public:
        ScopeStk();

        map<string,identifier_type> getTop();
        map<string,identifier_type> pop();
        void push(map<string,identifier_type> value);

        int isDecl(string id);
        int isFuncDecl(string id);

        pair<int,token_type> getFuncAttr(string id);

        void addSymbol(string lexeme, identifier_type typ);

    private:
        vector<map<string,identifier_type>> scopeStk;
        map<string,pair<int,token_type>> functions;
};