#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

enum node_type {AST_LETTER, AST_DIGIT, AST_PRINTABLE, AST_TYPE, AST_BOOL_LIT, AST_INT_LIT,
                AST_FLOAT_LIT, AST_CHAR_LIT, AST_LIT, AST_IDENTIFIER, AST_MULOP,
                AST_ADDOP, AST_RELOP, AST_ACTUAL_PARAMS, AST_FUNC_CALL, AST_SUB_EXPR, AST_UNOP,
                AST_FACTOR, AST_TERM, AST_SIMPLE_EXPR, AST_EXPR, AST_ASSIGN, AST_VAR_DECL, AST_PRINT_STMT,
                AST_RETURN_STMT, AST_IF_STMT, AST_FOR_STMT, AST_WHILE_STMT, AST_FORMAL_PARAM,
                AST_FORMAL_PARAMS, AST_FUNC_DECL, AST_STMT, AST_BLOCK, AST_PROGRAM};


class ASTNode: public enable_shared_from_this<ASTNode>{
    public:
        node_type type;
        vector<shared_ptr<ASTNode>> children;
        shared_ptr<ASTNode> parent;
        string attr;

        ASTNode(node_type type);
        void add_child(shared_ptr<ASTNode> node);

};