#include "lexer.hpp"
#include <algorithm>

Lexer::Lexer() {

    //TRANSITIONSxSTATES:         START           A         B         C         D           E            F          EMPTY       ID     OPEN_BRACKET  CLOSE_BRACKET  OPEN_CURLY  CLOSE_CURLY   SEMICOLON   COLON     COMMA      EQUAL         INT       FLOAT      CHAR    MULOP_A       MULOP_B      ADDOP_A    ADDOP_B    RELOP_A     RELOP_B    RELOP_C     ARROW      COMMENT_A     COMMENT_B
    dfa =/*LETTER       :*/ {{S_ID           , S_B    ,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*LETTER       */
         /*DIGIT        :*/  {S_INT          , S_B    ,  S_EMPTY,   S_FLOAT,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_INT  ,   S_FLOAT,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*DIGIT        */
         /*ASCII_OTHER  :*/  {S_EMPTY        , S_B    ,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*ASCII_OTHER  */
         /*OPEN_BRACKET :*/  {S_OPEN_BRACKET , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*OPEN_BRACKET */
         /*CLOSE_BRACKET:*/  {S_CLOSE_BRACKET, S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*CLOSE_BRACKET*/
         /*OPEN_CURLY   :*/  {S_OPEN_CURLY   , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*OPEN_CURLY   */
         /*CLOSE_CURLY  :*/  {S_CLOSE_CURLY  , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*CLOSE_CURLY  */
         /*SEMICOLON    :*/  {S_SEMICOLON    , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*SEMICOLON    */
         /*COLON        :*/  {S_COLON        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*COLON        */
         /*COMMA        :*/  {S_COMMA        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*COMMA        */
         /*QUOTE        :*/  {S_A            , S_EMPTY,  S_CHAR_LIT,S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*QUOTE        */
         /*EQUAL        :*/  {S_EQUAL        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_RELOP_C,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_RELOP_C,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_RELOP_B,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*EQUAL        */
         /*PERIOD       :*/  {S_EMPTY        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_C    ,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*PERIOD       */
         /*BANG!        :*/  {S_F            , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*BANG!        */
         /*DIV          :*/  {S_MULOP_A      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,  S_COMMENT_B,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_COMMENT_A,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*DIV          */
         /*MUL          :*/  {S_MULOP_B      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_E     ,     S_E     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_D        ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*MUL          */
         /*ADD          :*/  {S_ADDOP_A      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*ADD          */
         /*SUB          :*/  {S_ADDOP_B      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*SUB          */
         /*LESS         :*/  {S_RELOP_A      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*LESS         */
         /*GREATER      :*/  {S_RELOP_B      , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_ARROW,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*GREATER      */
         /*UNDERSCORE   :*/  {S_ID           , S_EMPTY,  S_B    ,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_ID   ,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*UNDERSCORE   */
         /*TAB          :*/  {S_START        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}, /*TAB          */
         /*NEWLINE      :*/  {S_START        , S_EMPTY,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY    ,    S_EMPTY}, /*NEWLINE      */
         /*SPACE        :*/  {S_START        , S_B    ,  S_EMPTY,   S_EMPTY,  S_D     ,     S_D     ,  S_EMPTY  ,   S_EMPTY,   S_EMPTY,    S_EMPTY,       S_EMPTY,     S_EMPTY,    S_EMPTY,     S_EMPTY,  S_EMPTY,  S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY, S_EMPTY    ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_EMPTY  ,   S_EMPTY,   S_EMPTY,   S_EMPTY,   S_COMMENT_A,    S_EMPTY}};/*SPACE        */
    //TRANSITIONSxSTATES:         START           A         B         C         D           E            F          EMPTY       ID     OPEN_BRACKET  CLOSE_BRACKET  OPEN_CURLY  CLOSE_CURLY   SEMICOLON   COLON     COMMA      EQUAL        INT        FLOAT      CHAR    MULOP_A       MULOP_B      ADDOP_A    ADDOP_B    RELOP_A     RELOP_B    RELOP_C     ARROW      COMMENT_A     COMMENT_B     


    keywords = {"for", "while", "if", "else", "let", "print", "return", "fn",
                "true", "false", "and", "or", "not", "float", "int", "bool","char"};
    
    keyword_types = {TOK_FOR, TOK_WHILE, TOK_IF, TOK_ELSE, TOK_LET, TOK_PRINT, TOK_RETURN, TOK_FN,
                     TOK_BOOL_LIT, TOK_BOOL_LIT,TOK_MULOP, TOK_ADDOP, TOK_UNOP, TOK_FLOAT, TOK_INT, TOK_BOOL, TOK_CHAR};


    //Assert
    if(keywords.size() != keyword_types.size()) {
        cerr << "Vector keywords must be the same length as vector keyword_types\n";
        exit(EXIT_FAILURE);
    }
        


    final = {S_ID,
            S_OPEN_BRACKET, S_CLOSE_BRACKET, S_OPEN_CURLY, S_CLOSE_CURLY,
            S_SEMICOLON, S_COLON, S_COMMA, S_EQUAL, S_INT, S_FLOAT, S_CHAR_LIT,
            S_MULOP_A, S_MULOP_B,
            S_ADDOP_A, S_ADDOP_B,
            S_RELOP_A, S_RELOP_B, S_RELOP_C,
            S_ARROW, S_COMMENT_A, S_COMMENT_B};



    start = S_START;
    empty = S_EMPTY;

    last_final = empty;
    current_state = start;
   
}

transition Lexer::getTransition(char x) {
    
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
        case '.' : return TRANS_PERIOD;
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

token_type Lexer::id_to_keyword(string lexeme) {
    

    for (int i = 0; i < keywords.size(); i++) {
        if (lexeme == keywords[i])
            return keyword_types[i];        
    }
    
    return TOK_ID;
}

state Lexer::nextState(state s, transition t) {
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
        case S_FLOAT:         return TOK_FLOAT_LIT;
        case S_INT:           return TOK_INT_LIT;
        case S_CHAR_LIT:         return TOK_CHAR_LIT;
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

void Lexer::printToken(vector<Token> tokens) { 
    
    vector<string> labels = {"OPEN_BRACKET", "CLOSE_BRACKET",
                            "OPEN_CURLY", "CLOSE_CURLY",
                            "SEMICOLON", "COLON", "COMMA",
                            "EQUAL", "INT_LIT", "FLOAT_LIT", "CHAR_LIT", "MULOP",
                            "UNOP","ADDOP", "RELOP", "ARROW", "COMMENT", "BOOL_LIT",
                            "ID","FOR", "WHILE", "IF", "ELSE", "LET", "PRINT", "RETURN", "FN",
                            "FLOAT", "INT", "BOOL", "CHAR"};
    
    int prev_nl = 0;    
    
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];

        if (token.ln > prev_nl) {
            cout << '\n';
            prev_nl = token.ln;
        }
        
        cout << "[" << ANSI_YEL << labels[token.type] << ANSI_ESC << ", " << ANSI_GRN <<token.lexeme << ANSI_ESC << "]";



    }


    
    
}

vector<Token> Lexer::getTokens(string input_text) {
    
    vector<Token> tokens;
    string current_lexeme = "";

    char c;
    transition t;
    int nl_counter = 0;
    int char_counter = 0;
    int rollback_counter = 0;
    int prev_char_counter = 0;

    for (int i = 0; i < input_text.size(); i++) {
        
        c = input_text[i];
        
        if (c == '\n') {
            nl_counter ++;
            prev_char_counter = char_counter;
            char_counter = 0;
        } char_counter ++;



        //Get the transition corresponding to c.
        if ((t = getTransition(c)) == TRANS_EMPTY) {
            cerr << "Invalid character" << c << "supplied to get_transition()" << '\n';
            exit(EXIT_FAILURE);
        }
            
        //Get next state.
        
        current_state = nextState(current_state, t);

        //If after taking a transition the current state is the starting state or is the empty state, then don't consume the character as it is a whitespace character.
        if (current_state != start && current_state != empty) {
            current_lexeme += c;    
        }        
        

        //If the current_state is an accepting state then remember it.
        if (isFinal(current_state)) {
            last_final = current_state;
            rollback_counter = 0;
        }
        
        //If the current state is the empty state then attempt to retrieve the token.
        else if (current_state == empty) {

            //If the current_state is empty and it hasn't traversed an accepting state before then there is a syntax error.
            if (last_final == empty) {
                cerr << "\nInvalid token " << current_lexeme << " at [" << nl_counter <<","<< char_counter-3 << "] -> [" << c << "]\n";
                exit(EXIT_FAILURE);
            }
            
            //Otherwise, the end of the lexeme is the character where the last final state was encountered.
            // cout << current_lexeme << '\n';
            //Truncate lexeme.
            current_lexeme.resize(current_lexeme.length() - rollback_counter); 
            
            //The next iteration will start where the last lexeme ended.
            i -= rollback_counter+1;
            
            char_counter -= rollback_counter+1;
            if (char_counter <= 0) {
                char_counter = prev_char_counter;
                nl_counter--;
            }
            

            //Generate token and add to vector of tokens.
            token_type type = state_to_token_type(last_final);
            
            
            if (type == TOK_ID) {
                type = id_to_keyword(current_lexeme);
            }
          

            Token token = {type,current_lexeme, nl_counter, int(char_counter-current_lexeme.size()-1)};

            
            tokens.push_back(token);


            //Reset previously seen final state.
            last_final = empty;
            
            //Reset current_lexeme.
            current_lexeme = "";

            //Reset current_state.
            current_state = start;

            //Reset rollback_counter.
            rollback_counter = 0;

        }
        
        //If the state is not empty or a final state, then increment the rollback_counter and continue.
        else rollback_counter++;


    }

    return tokens;
}