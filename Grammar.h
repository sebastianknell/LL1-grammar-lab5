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
#include <stack>

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
    map<string, map<string, row_type>> M; // Map from terminals to non-terminals
    void generateTestGrammar1();
    void generateTestGrammar2();
    set<Token> getFirst(const Token&);
    const Token meta{"$", TERM};
    const Token epsilon{"eps", TERM};
public:
    Grammar();
    void getFirstSets();
    void getNextSets();
    bool validRules();
    void buildTable();
    bool processString(string);
    void printRules();
    void printFirstSets();
    void printNextSets();
};


#endif //LAB5_GRAMMAR_H
