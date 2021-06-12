//
// Created by Sebastian Knell on 11/06/21.
//

#ifndef LAB5_GRAMMAR_H
#define LAB5_GRAMMAR_H

#include <string>
#include <vector>
#include <map>

using namespace std;

enum token_type {
    TERM,
    NTERM
};

struct Token {
    string text;
    token_type tokenType;
};

/* TEST GRAMMAR
 * exp -> exp opsuma term | term
 * opsuma -> + | -
 * term -> term opmult factor | factor
 * opmult -> *
 * factor -> (exp) | numero
*/

class Grammar {
    map<Token, vector<Token>> rules;
};


#endif //LAB5_GRAMMAR_H
