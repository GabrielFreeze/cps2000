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
void ScopeStk::addFunction(string id, int params, return_type type_to_return, Token token, shared_ptr<ASTNode> block) {
    functions.insert(pair<string,symbolFunc>(id,{id,type_to_return,params,token}));   
}



symbol ScopeStk::getSymbol(string id) {
    
    int count = 0;

    for (int i = scopeStk.size()-1; i >= 0; i--) {
        auto scope = scopeStk[i];

        auto symbol_it = find_if(scope.begin(), scope.end(), [&id] (pair<string,symbol> p) {
            return p.first == id;
        });

        if (symbol_it != scope.end()) {
            return (*symbol_it).second;
        }

    }

    return {"",ID_EMPTY,{},{}};
} 

void ScopeStk::updateSymbol(string id, value v) {
    
    for (auto& scope : scopeStk) {
        
        auto symbol_it = scope.find(id);

        if (symbol_it != scope.end()) {
            symbol_it->second.val = v;
            return;
        }

    }
    
    
    // scopeStk.getTop().find(identifier_node->attr)->second.val = evaluate(expression_node);
}



map<string, symbol>& ScopeStk::getTop() {
    return scopeStk[scopeStk.size()-1];
}


void ScopeStk::addSymbol(string id, identifier_type type, value val, Token token) {
    scopeStk[scopeStk.size()-1].insert(pair<string,symbol>(id, {id,type,val,token}));    
}
void ScopeStk::delSymbol(string id) {
    
    auto current_scope = getTop();
    auto it = current_scope.find(id);

    if (it != current_scope.end())
        current_scope.erase (it);  


}