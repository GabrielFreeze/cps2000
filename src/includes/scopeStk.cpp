#include "scopeStk.hpp"

ScopeStk::ScopeStk() {

}
//TODO: implement
void ScopeStk::push(map<string, identifier_type> value) {
    scopeStk.push_back(value);
}
map<string, identifier_type> ScopeStk::pop() {
    auto scope = getTop();
    scopeStk.pop_back();
    return scope;
}
pair<int,token_type> ScopeStk::getFuncAttr(string id) {
    return functions.find(id)->second;
}

int ScopeStk::isFuncDecl(string id) {
    return functions.count(id);
}

int ScopeStk::isDecl(string id) {
    
    int count = 0;

    for (auto scope : scopeStk) {
        count += scope.count(id);
    }

    return count;
} 

map<string, identifier_type> ScopeStk::getTop() {
    return scopeStk[scopeStk.size()-1];
}


void ScopeStk::addSymbol(string lexeme, identifier_type type) {
    scopeStk[scopeStk.size()-1].insert(pair<string,identifier_type>(lexeme, type));    
}