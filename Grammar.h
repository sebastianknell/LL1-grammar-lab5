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
#include <stdexcept>
#include <set>

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
    map<string, set<Token>> firstSets;
    map<string, set<Token>> nextSets;
    void generateTestGrammar();
    set<Token> getFirst(Token);
public:
    Grammar();
    void getFirstSets();
    void getNextSets();
    bool validRules();
    void buildTable();
    void processString();
    void printRules();
    void printFirstSets();
    void printNextSets();
};


#endif //LAB5_GRAMMAR_H
