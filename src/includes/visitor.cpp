#include "visitor.hpp"

XMLVisitor::XMLVisitor (string filepath) {
    outfile.open(filepath);
}
void XMLVisitor::visit (shared_ptr<ASTNode> node, int depth) {
    
    vector<string> map = {"LETTER", "DIGIT", "PRINTABLE", "TYPE", "BOOL_LIT", "INT_LIT",
                          "FLOAT_LIT", "CHAR_LIT", "LIT", "IDENTIFIER", "MULOP",
                          "ADDOP", "RELOP", "ACTUAL_PARAMS", "FUNC_CALL", "SUB_EXPR", "UNOP",
                          "FACTOR", "TERM", "SIMPLE_EXPR", "EXPR", "ASSIGN", "VAR_DECL", "PRINT_STMT",
                          "RETURN_STMT", "IF_STMT", "FOR_STMT", "WHILE_STMT", "FORMAL_PARAM",
                          "FORMAL_PARAMS", "FUNC_DECL", "STMT", "BLOCK", "PROGRAM"};
    
    //Indent Opening Tag
    for (int i = 0; i < depth; i++) {
        outfile << '\t';
    }

    //Open XML Tag of node->type
    outfile << "<"+map[node->type]+(node->attr.empty()? "":" ")+node->attr+">";
    
    if (node->type == AST_IDENTIFIER || node->type == AST_MULOP ||
        node->type == AST_ADDOP      || node->type == AST_UNOP  ||
        node->type == AST_LIT) {
        outfile << " "+node->attr+" ";
    } else {
        if (!node->children.empty()) {
            outfile << "\n";
        } else {
            outfile << " ";
        }

    }

    //Visit Children
    for (auto c : node->children) {
        visit(c,depth+1);
    }

    //Indent Closing Tag
    if (!node->children.empty()) {
        for (int i = 0; i < depth; i++) {
            outfile << '\t';
        }
    }

    //Close XML Tag
    outfile << "</"+map[node->type]+">\n";
}

