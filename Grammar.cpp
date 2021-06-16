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

static bool operator<(const Token& a, const Token& b) {
    return a.text < b.text;
}

Grammar::Grammar() {
    generateTestGrammar();
}

set<Token> Grammar::getFirst(Token token) {
    if (token.tokenType == TERM) {
        return set<Token>{token};
    }
    else {
        return firstSets[token.text];
    }
}

void Grammar::getFirstSets() {
    // Initialize first sets
    for (const auto& rule : rules) {
        if (rule.lhs.tokenType != NTERM) throw runtime_error("LHS must be a non terminal");
        firstSets[rule.lhs.text] = set<Token>();
    }

    bool hasChanged = true;
    while (hasChanged) {
        hasChanged = false;
        for (auto rule : rules) {
            auto *current = &firstSets[rule.lhs.text];
            auto first = getFirst(rule.rhs.front());
            auto prev_size = current->size();
            current->merge(first);
            if (prev_size < current->size()) hasChanged = true;
        }
    }
}

void Grammar::getNextSets() {
    if (firstSets.empty()) return;

    for (const auto& rule : rules) {
        if (rule.lhs.tokenType != NTERM) throw runtime_error("LHS must be a non terminal");
        nextSets[rule.lhs.text] = set<Token>();
    }

    bool hasChanged = true;
    while (hasChanged) {
        hasChanged = false;
        for (const auto &rule : rules) {
            for (int i = 0; i< rule.rhs.size(); i++) {
                if (rule.rhs[i].tokenType == NTERM) {
                    int j = i;
                    while (j < rule.rhs.size()) {
                        nextSets[rule.rhs[i].text].merge(getFirst(rule.rhs[j]));
                        j++;
                    }
                }
            }

        }
    }
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
    rules.push_back({{"term", NTERM}, temp});

    temp.clear();
    temp.push_back({"factor", NTERM});
    rules.push_back({{"term", NTERM}, temp});

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
    cout << "       RULES       " << endl;
    cout << "-------------------" << endl;
    for (const auto& row : rules) {
        cout << row.lhs.text << " -> ";
        for (const auto& token : row.rhs) {
            cout << token.text << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grammar::printFirstSets() {
    for (const auto &set : firstSets) {
        cout << set.first << " = ";
        for (const auto &token : set.second) {
            cout << token.text << " ";
        }
        cout << endl;
    }
}

void Grammar::printNextSets() {
    for (const auto &set : firstSets) {
        cout << set.first << " = ";
        for (const auto &token : set.second) {
            cout << token.text << " ";
        }
        cout << endl;
    }
}
