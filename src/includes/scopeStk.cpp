#include "scopeStk.hpp"

ScopeStk::ScopeStk() {
    push(map<string,symbol>());
}

void ScopeStk::push(map<string, symbol> value) {
    scopeStk.push_back(value);
}
map<string, symbol> ScopeStk::pop() {
    auto scope = getTop();
    scopeStk.pop_back();
    return scope;
}


symbolFunc ScopeStk::getFuncAttr(string id) {
    return functions.find(id)->second;
}

int ScopeStk::isFuncDecl(string id) {
    return functions.count(id);
}
void ScopeStk::addFunction(string id, int params, return_type type_to_return, Token token) {
    functions.insert(pair<string,symbolFunc>(id,{id,type_to_return,params,token}));   
}



int ScopeStk::isDecl(string id) {
    
    int count = 0;

    for (auto scope : scopeStk) {
        count += scope.count(id);
    }

    return count;
} 

map<string, symbol> ScopeStk::getTop() {
    return scopeStk[scopeStk.size()-1];
}


void ScopeStk::addSymbol(string id, identifier_type type, value val, Token token) {
    scopeStk[scopeStk.size()-1].insert(pair<string,symbol>(id, {id,type,val,token}));    
}