#pragma once

#include "parser.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;


enum identifier_type  {ID_INT, ID_FLOAT, ID_CHAR, ID_BOOL, ID_FUNC, ID_EMPTY};


class ScopeStk {
    public:
        ScopeStk();

        map<string,string> getTop();
        map<string,string> pop();
        void push(map<string,string>);

        bool isDecl(string id);

        //TODO: change type to enum
        void addSymbol(string lexeme, identifier_type typ);

    private:
        vector<map<string,string>> scopeStk;
        int x;
};