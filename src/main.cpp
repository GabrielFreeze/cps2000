#include <iostream>
#include <fstream>
#include <sstream>
#include "includes/lexer.hpp"

using namespace std;

string file_to_string(const string& path) {
    //TODO: FIXXX DISS!!?
    return "";
}




int main() {


    Lexer lexer;

    string line;
    string filename = "program.rs";

    
    string text;
    text = file_to_string("program.rs");

    cout << text.length();


    vector<Token> tokens = lexer.get_tokens(text);

    for (Token& token: tokens) {
        lexer.printToken(token);
    }
    
    
    

    return 0;
}