//
// Created by Sebastian Knell on 11/06/21.
//

#include "Grammar.h"

static optional<row_type> find_token(table_type table, string text) {
    for (auto row : table) {
        if (row.lhs.text == text) return row;
    }
    return nullopt;
}

Grammar::Grammar() {
    generateTestGrammar();
}

void Grammar::getFirstSet() {
    // Initialize first sets
    for (const auto& rule : rules) {
        firstSets.push_back({rule.lhs, vector<Token>()});
    }

    bool hasChanged = true;
    while (hasChanged) {
        for (auto rule : rules) {
            auto lhs = rule.lhs;
            auto rhs = rule.rhs;
            vector<Token> temp;
            if (rhs.front().tokenType == TERM) {
                temp.push_back(rhs.front());
                firstSets.push_back({lhs, temp});
                hasChanged = true;
            }
            else {
                auto first = find_token(firstSets, lhs.text);
                if (first.has_value()) {

                } else hasChanged = false;
            }
        }
    }
}

void Grammar::getNextSet() {

}

void Grammar::generateTestGrammar() {
    vector<Token> temp;
    temp.push_back({"exp", NTERM});
    temp.push_back({"opsuma", NTERM});
    temp.push_back({"term", NTERM});
    rules.push_back({{"exp", NTERM}, temp});

    temp.clear();
    temp.push_back({"term", NTERM});
    rules.push_back({{"exp", NTERM}, temp});

    temp.clear();
    temp.push_back({"+", TERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"-", TERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"term", NTERM});
    temp.push_back({"opmult", NTERM});
    temp.push_back({"factor", NTERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"factor", NTERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"*", TERM});
    rules.push_back({{"opmult", NTERM}, temp});

    temp.clear();
    temp.push_back({"(", TERM});
    temp.push_back({"exp", NTERM});
    temp.push_back({")", TERM});
    rules.push_back({{"factor", NTERM}, temp});
}

static void printTable(const table_type &table) {
    for (const auto& row : table) {
        cout << row.lhs.text << " = ";
        for (const auto& token : row.rhs) {
            cout << token.text << " ";
        }
        cout << endl;
    }
}

void Grammar::printRules() {
    for (const auto& row : rules) {
        cout << row.lhs.text << " -> ";
        for (const auto& token : row.rhs) {
            cout << token.text << " ";
        }
        cout << endl;
    }
}

void Grammar::printFirstSets() {
    printTable(firstSets);
}

void Grammar::printNextSets() {
    printTable(nextSets);
}
