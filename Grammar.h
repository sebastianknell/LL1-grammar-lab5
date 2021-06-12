//
// Created by Sebastian Knell on 11/06/21.
//

#ifndef LAB5_GRAMMAR_H
#define LAB5_GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>

using namespace std;

enum token_type {
    TERM,
    NTERM
};

struct Token {
    string text;
    token_type tokenType;
};

struct row_type {
    Token lhs;
    vector<Token> rhs;
};
using table_type = vector<row_type>;

/* TEST GRAMMAR
 * exp -> exp opsuma term | term
 * opsuma -> + | -
 * term -> term opmult factor | factor
 * opmult -> *
 * factor -> (exp) | numero
*/

class Grammar {
    table_type rules;
    table_type firstSets;
    table_type nextSets;
    void generateTestGrammar();
public:
    Grammar();
    void getFirstSet();
    void getNextSet();
    bool validRules();
    void buildTable();
    void processString();
    void printRules();
    void printFirstSets();
    void printNextSets();
};


#endif //LAB5_GRAMMAR_H
