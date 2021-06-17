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

static bool operator==(const Token& a, const Token& b) {
    return a.text == b.text && a.tokenType == b.tokenType;
}

static void push_vector(stack<Token> &stack, vector<Token> vector) {
    for (auto i = vector.size(); i >= 0; i--) {
        stack.push(vector[i]);
    }
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
    nextSets[rules[0].lhs.text].insert(meta);

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

void Grammar::buildTable() {
    // Get all terminals
    set<Token> terminals;
    for (const auto& rule : rules) {
        for (const auto& token : rule.rhs) {
            if (token.tokenType == TERM) terminals.insert(token);
        }
    }
    // Map each terminal with each non-terminal
    for (const auto& rule : rules) {
        M[rule.lhs.text] = map<string, row_type>();
        for (const auto& term : terminals) {
            M[rule.lhs.text][term.text] = {"0"};
        }
    }
    // Fill according to first set of each non-terminal
    for (const auto& set : firstSets) {
        for (const auto& token : set.second) {
            auto rule = find_if(rules.begin(), rules.end(), [set](const row_type& row) {return row.lhs.text == set.first;});
            if (rule != rules.end())
                M[set.first][token.text] = *rule;
        }
    }


}

bool Grammar::processString(string s) {
    stack<Token> stack;
    stack.push(meta);
    stack.push(rules[0].lhs);
    vector<Token> input;
    for (auto c : s) input.push_back({to_string(c), TERM});
    input.push_back(meta);

    int i = 0;
    while (i < input.size()) {
        if (stack.top() == meta && input[0] == meta) return true;
        // Match
        if (stack.top().tokenType == TERM) {
            if (stack.top() == input[i]) {
                stack.pop();
                i++;
            }
            else return false;
        }
        else {
            auto rule = M[stack.top().text][input[i].text]; // TODO validate
            push_vector(stack, rule.rhs);
        }
    }
    return false;
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
