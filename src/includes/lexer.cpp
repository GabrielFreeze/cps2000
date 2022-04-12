#include "lexer.hpp"

Lexer::Lexer() {

    //TRANSITIONSxSTATES:         START           A         B         C         D           E         F              G          EMPTY       ID     OPEN_BRACKET  CLOSE_BRACKET  OPEN_CURLY  CLOSE_CURLY  SEMICOLON   COLON     COMMA      EQUAL        NUM_A       NUM_B      CONST   MULOP_A       MULOP_B      ADDOP_A    ADDOP_B    RELOP_A     RELOP_B    RELOP_C     ARROW      COMMENT_A     COMMENT_B
    dfa =/*LETTER       :*/ {{S_ID           , S_B    ,  S_EMPTY,  S_EMPTY,  S_D     ,   S_EMPTY,  S_D        ,   S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,   S_EMPTY        }, /*LETTER       */
         /*DIGIT        :*/  {S_NUM_A        , S_B    ,  S_EMPTY,  S_NUM_B,  S_D     ,   S_EMPTY,  S_D        ,   S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_NUM_A,   S_NUM_B,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,   S_EMPTY        }, /*DIGIT        */
         /*ASCII_OTHER  :*/  {S_EMPTY        , S_B    ,  S_EMPTY,  S_EMPTY,  S_D     ,   S_EMPTY,  S_D        ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,   S_EMPTY        }, /*ASCII_OTHER  */
         /*OPEN_BRACKET :*/  {S_OPEN_BRACKET , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*OPEN_BRACKET */
         /*CLOSE_BRACKET:*/  {S_CLOSE_BRACKET, S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*CLOSE_BRACKET*/
         /*OPEN_CURLY   :*/  {S_OPEN_CURLY   , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*OPEN_CURLY   */
         /*CLOSE_CURLY  :*/  {S_CLOSE_CURLY  , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*CLOSE_CURLY  */
         /*SEMICOLON    :*/  {S_SEMICOLON    , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*SEMICOLON    */
         /*COLON        :*/  {S_COLON        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*COLON        */
         /*COMMA        :*/  {S_COMMA        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*COMMA        */
         /*QUOTE        :*/  {S_A            , S_EMPTY,  S_CONST,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*QUOTE        */
         /*EQUAL        :*/  {S_EQUAL        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_RELOP_C,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_RELOP_C,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_RELOP_B,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*EQUAL        */
         /*PERIOD       :*/  {S_EMPTY        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_C    ,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*PERIOD       */
         /*BANG!        :*/  {S_G            , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*BANG!        */
         /*DIV          :*/  {S_MULOP_A      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_COMMENT_B,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_COMMENT_A,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*DIV          */
         /*MUL          :*/  {S_MULOP_B      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_F     ,   S_F    ,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_COMMENT_B,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*MUL          */
         /*ADD          :*/  {S_ADDOP_A      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*ADD          */
         /*SUB          :*/  {S_ADDOP_B      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*SUB          */
         /*LESS         :*/  {S_RELOP_A      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_ARROW,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*LESS         */
         /*GREATER      :*/  {S_RELOP_B      , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*GREATER      */
         /*UNDERSCORE   :*/  {S_ID           , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*UNDERSCORE   */
         /*TAB          :*/  {S_START        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_D     ,   S_EMPTY,  S_D        ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*TAB          */
         /*NEWLINE      :*/  {S_START        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_E     ,   S_EMPTY,  S_D        ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }, /*NEWLINE      */
         /*SPACE        :*/  {S_START        , S_EMPTY,  S_EMPTY,  S_EMPTY,  S_EMPTY ,   S_EMPTY,  S_EMPTY    ,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,   S_EMPTY        }};/*SPACE        */
    //TRANSITIONSxSTATES:         START           A         B         C         D           E         F              G          EMPTY       ID     OPEN_BRACKET  CLOSE_BRACKET  OPEN_CURLY  CLOSE_CURLY  SEMICOLON   COLON     COMMA      EQUAL        NUM_A       NUM_B      CONST   MULOP_A       MULOP_B      ADDOP_A    ADDOP_B    RELOP_A     RELOP_B    RELOP_C     ARROW      COMMENT_A     COMMENT_B     


    keywords = {"for", "while", "if", "else", "let", "print", "return", "fn",
                "true", "false", "and", "or", "not", "float", "int", "bool","char"};

    final = {S_ID,
            S_OPEN_BRACKET, S_CLOSE_BRACKET, S_OPEN_CURLY, S_CLOSE_CURLY,
            S_SEMICOLON, S_COLON, S_COMMA, S_EQUAL, S_NUM_B, S_NUM_A, S_CONST,
            S_MULOP_A, S_MULOP_B,
            S_ADDOP_A, S_ADDOP_B,
            S_RELOP_A, S_RELOP_B, S_RELOP_C,
            S_ARROW, S_COMMENT_A, S_COMMENT_B};



    start = S_START;
    empty = S_EMPTY;

    last_final = empty;
    current_state = start;
   
}



transition Lexer::get_transition(char x) {
    
    switch (x) {

        case '*' : return TRANS_MUL;
        case '/' : return TRANS_DIV;
        case '+' : return TRANS_ADD;
        case '-' : return TRANS_SUB;
        case '\t': return TRANS_TAB;
        case '!' : return TRANS_BANG;
        case '<' : return TRANS_LESS;
        case '=' : return TRANS_EQUAL;
        case ':' : return TRANS_COLON;
        case ' ' : return TRANS_SPACE;
        case ',' : return TRANS_COMMA;
        case '\'': return TRANS_QUOTE;
        case '>' : return TRANS_GREATER;
        case '\n': return TRANS_NEWLINE;
        case ';' : return TRANS_SEMICOLON;
        case '{' : return TRANS_OPEN_CURLY;
        case '_' : return TRANS_UNDERSCORE;
        case '}' : return TRANS_CLOSE_CURLY;
        case '(' : return TRANS_OPEN_BRACKET;
        case ')' : return TRANS_CLOSE_BRACKET;

        default: {
            if ((65 <= x && x <= 90) || (97 <= x && x <= 122))
                return TRANS_LETTER;
            else if (48 <= x && x <= 57)
                return TRANS_DIGIT;
            else if (0x20 <= x && x <= 0x7E)
                return TRANS_ASCII_OTHER; //Everything in the printable range that is not LETTER of DIGIT.
            
            return TRANS_EMPTY;
        }
    }

}


state Lexer::next_state(state s, transition t) {
    return dfa[t][s];
}

bool Lexer::isFinal(state s) {
    return count(final.begin(), final.end(), s);
}

token_type Lexer::state_to_token_type(state s) {
    switch (s) {

        case S_ID:            return TOK_ID;
        case S_OPEN_BRACKET:  return TOK_OPEN_BRACKET;
        case S_CLOSE_BRACKET: return TOK_CLOSE_BRACKET;
        case S_OPEN_CURLY:    return TOK_OPEN_CURLY;
        case S_CLOSE_CURLY:   return TOK_CLOSE_CURLY;
        case S_SEMICOLON:     return TOK_SEMICOLON;
        case S_COLON:         return TOK_COLON;
        case S_COMMA:         return TOK_COMMA;
        case S_EQUAL:         return TOK_EQUAL;
        case S_NUM_B:         return TOK_NUM;
        case S_NUM_A:         return TOK_NUM;
        case S_CONST:         return TOK_CONST;
        case S_MULOP_A:       return TOK_MULOP;
        case S_MULOP_B:       return TOK_MULOP;
        case S_ADDOP_A:       return TOK_ADDOP;
        case S_ADDOP_B:       return TOK_ADDOP;
        case S_RELOP_A:       return TOK_RELOP;
        case S_RELOP_B:       return TOK_RELOP;
        case S_RELOP_C:       return TOK_RELOP;
        case S_ARROW:         return TOK_ARROW;
        case S_COMMENT_A:     return TOK_COMMENT;
        case S_COMMENT_B:     return TOK_COMMENT;

        default: {
            cerr << "In state_to_token: State " << s << " is not a final state.";
            exit(EXIT_FAILURE);
        }
    }
}


void Lexer::printToken(Token token) { 
    
    vector<string> labels = {"OPEN_BRACKET", "CLOSE_BRACKET",
        "OPEN_CURLY", "CLOSE_CURLY",
        "SEMICOLON", "COLON", "COMMA",
        "EQUAL", "NUM", "CONST", "MULOP",
        "ADDOP", "RELOP", "ARROW", "COMMENT",
        "ID"};
        
    cout << "<" << labels[token.type] << ", " << token.lexeme << ">\n"; 
}



vector<Token> Lexer::get_tokens(string input_text) {
    
    string current_lexeme = "";
    vector<Token> tokens;
    char c;
    int rollback_counter = 0;
    int nl_counter = 0;
    int char_counter = 0;

    for (int i = 0; i < input_text.length(); i++) {
        
        //For error reporting.
        if (c == '\n') {
            nl_counter++;
            char_counter = 1;
        } else {
            char_counter++;
        }

        

        current_lexeme += c;
        current_state = next_state(current_state, get_transition(c));

        if (isFinal(current_state))
            last_final = current_state;
        
        //Empty state. Attempt to get token.
        else if (current_state == empty) {

            //Invalid lexeme.
            if (last_final == empty) {
                cerr << "Invalid token " << current_lexeme << " at [" << nl_counter << ',' << char_counter << ']';
                exit(EXIT_FAILURE);
            }

            //The end of the lexeme is the character where the last final state was encountered.
            while (current_state != last_final) {
                current_lexeme.resize(current_lexeme.length() - 2);
                rollback_counter++;
            }


            //Generate token and add to vector of tokens.
            Token token = {state_to_token_type(last_final),current_lexeme};
            tokens.push_back(token);

            //Reset previously seen final state.
            last_final = empty;
            
            //Reset current_lexeme
            current_lexeme = "";
            
            //Rollback to the starting character of the next lexeme.
            i -= rollback_counter;
            rollback_counter = 0;
            
        }


    }
    return tokens;
}