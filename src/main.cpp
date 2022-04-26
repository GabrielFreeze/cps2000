#include <iostream>
#include <fstream>
#include <sstream>
#include "includes/lexer.hpp"
#include "includes/parser.hpp"

using namespace std;

string file_to_string(const string& path);
void print_program(string& text);


int main() {


    Lexer lexer;
    

    string line;
    string filename = "programs/program_03.rs";

    string text;
    text = file_to_string(filename);

    print_program(text);

    vector<Token> tokens = lexer.getTokens(text);

    Parser parser(tokens);


    lexer.printToken(tokens);
    cout << '\n';

    if (auto node = parser.parseProgram())
        cout << "OK" << '\n';

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
    int nl_counter = 0;
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