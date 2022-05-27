#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "includes/lexer.hpp"
#include "includes/parser.hpp"
#include "includes/visitor.hpp"

using namespace std;

string file_to_string(const string& path);
void print_program(string& text);


int main(int argc, char* argv[]) {
    
    string filename;


    if (argc != 1) {
        filename = string(filesystem::current_path()) + "/" +string(argv[1]);
    } else filename = "programs/program_01.rs";

    Lexer lexer;

    string line;
    string xml_filename = "xml/xml_01.xml";

    string text;
    text = file_to_string(filename);

    print_program(text);

    vector<Token> tokens = lexer.getTokens(text);

    lexer.printToken(tokens);
    cout << '\n';

    shared_ptr<ASTNode> rootNode;
    Parser parser(tokens);
    if (!(rootNode = parser.parseProgram()))
        exit(EXIT_FAILURE);


    XMLVisitor xmlVisitor(xml_filename);
    xmlVisitor.visit(rootNode);

    SemanticVisitor semanticVisitor;
    
    if (!semanticVisitor.analyseFuncDecl(rootNode)) {
        exit(EXIT_FAILURE);
    };
    
    if (!semanticVisitor.analyse(rootNode)) {
        exit(EXIT_FAILURE);
    }

    InterpreterVisitor iVisitor;

    iVisitor.visitFuncDecl(rootNode);
    iVisitor.visit(rootNode);


    

    return 0;
}


string file_to_string(const string& path) {
    
    ifstream f(path);
    string line;
    string text = "";
    while (getline(f,line)) {
        text += line;
        text += '\n';
    }

    return text;
}

void print_program(string& text) {
    int nl_counter = 1;
    cout << ANSI_MAG << nl_counter++<<": " << ANSI_ESC;
    for (char& c : text) {
        if (c == '\n') {
            cout << ANSI_MAG << '\n' << nl_counter++<<": " << ANSI_ESC;
        } else {
            cout << ANSI_CYN << c << ANSI_ESC;
        }
        
    }
    cout << ANSI_ESC << "\n\n";

}