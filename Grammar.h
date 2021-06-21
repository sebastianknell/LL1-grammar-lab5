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
#include <iomanip>
#include <sstream>

using namespace std;

enum token_type {
    TERM,
    NTERM
};

struct Token {
    string text;
    token_type tokenType;
    int ruleFrom = -1; // Rule index used for building M table. -1 if unused
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
    void generateTestGrammar();
    set<Token> getFirst(const Token&);
    const Token meta{"$", TERM};
    const Token epsilon{"ε", TERM};
    const Token error{"error", TERM};
public:
    Grammar();
    void getFirstSets();
    void getNextSets();
    bool validRules();
    void buildTable();
    bool processString(const string&, bool);
    void printRules();
    void printFirstSets();
    void printNextSets();
    void printTable();
    friend void operator<<(ostream& stream, const row_type& row);
};


#endif //LAB5_GRAMMAR_H
