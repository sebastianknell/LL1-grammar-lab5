//
// Created by Sebastian Knell on 11/06/21.
//

#include "Grammar.h"

static bool operator<(const Token& a, const Token& b) {
    return a.text < b.text;
}

static bool operator<(const row_type& a, const row_type& b) {
    return a.lhs < b.lhs;
}

static bool operator==(const Token& a, const Token& b) {
    return a.text == b.text && a.tokenType == b.tokenType;
}

void operator<<(ostream& stream, const row_type& row) {
    stream << row.lhs.text;
    if (row.lhs.text == "error") return;
    stream << " -> ";
    for (const auto& token : row.rhs) {
        stream << token.text << " ";
    }
}

static void push_vector(stack<Token> &stack, vector<Token> vector) {
    for (int i = vector.size() - 1; i >= 0; i--) {
        stack.push(vector[i]);
    }
}

Grammar::Grammar() {
    generateTestGrammar();
}

set<Token> Grammar::getFirst(const Token& token) {
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
        for (int i = 0 ; i < rules.size(); i++) {
            auto rule = rules[i];
            auto *current = &firstSets[rule.lhs.text];
            auto prev_size = current->size();
            int k = 0;
            bool hasEpsilon = true; // TODO what if the while loop doesn't execute?
            while (hasEpsilon && k < rule.rhs.size()) {
                auto first = getFirst(rule.rhs[k]);
                hasEpsilon = first.erase(epsilon);
                k++;
                for (auto token : first) {
                    token.ruleFrom = i;
                    current->insert(token);
                }
            }
            if (hasEpsilon) {
                auto eps = epsilon;
                eps.ruleFrom = i;
                current->insert(eps);
            }
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
                    auto *current = &nextSets[rule.rhs[i].text];
                    int j = i+1;
                    bool hasEpsilon = true;
                    while (hasEpsilon && j < rule.rhs.size()) {
                        auto first = getFirst(rule.rhs[j]);
                        hasEpsilon = first.erase(epsilon);
                        auto prev_size = current->size();
                        nextSets[rule.rhs[i].text].merge(first);
                        if (prev_size < current->size()) hasChanged = true;
                        j++;
                    }
                    if (hasEpsilon) {
                        auto temp = nextSets[rule.lhs.text];
                        auto prev_size = current->size();
                        current->merge(temp);
                        if (prev_size < current->size()) hasChanged = true;
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
    terminals.erase(epsilon);
    terminals.insert(meta);

    // Map each terminal with each non-terminal
    for (const auto& rule : rules) {
        M[rule.lhs.text] = map<string, row_type>();
        for (const auto& term : terminals) {
            M[rule.lhs.text][term.text] = {error};
        }
    }
    // Fill according to first set of each non-terminal
    for (const auto& set : firstSets) {
        bool hasEpsilon = false;
        for (const auto& token : set.second) {
            if (token.ruleFrom == -1) throw runtime_error("Error building table");
            if (token == epsilon) hasEpsilon = true;
            if (!hasEpsilon) {
                M[set.first][token.text] = rules[token.ruleFrom];
            }
        }
        if (hasEpsilon) {
            auto nextSet = nextSets[set.first];
            for (const auto& token : nextSet) {
                vector<Token> temp;
                temp.push_back(epsilon);
                M[set.first][token.text] = {{set.first}, temp};
            }
        }
    }
}

static void printStack(stack<Token> s) {
    stack<Token> copy;
    while (!s.empty()) {
        copy.push(s.top());
        s.pop();
    }
    while (!copy.empty()) {
        cout << copy.top().text << " ";
        copy.pop();
    }
    cout << endl;
}

bool Grammar::processString(const string& s, bool showStack=false) {
    stack<Token> stack;
    stack.push(meta);
    stack.push(rules[0].lhs);
    vector<Token> input;
    stringstream ss(s);
    string line;
    while (getline(ss, line, ' '))
        input.push_back({line, TERM});
    input.push_back(meta);
    if (input.empty()) return false;
    int i = 0;
    if (showStack) {
        cout << "            Stack" << endl;
        cout << "-----------------------------" << endl;
    }
    while (i < input.size()) {
        if (showStack) printStack(stack);
        if (stack.top() == meta && input[i] == meta) return true;
        // Match
        if (stack.top().tokenType == TERM) {
            if (stack.top() == input[i]) {
                stack.pop();
                i++;
            }
            else if (stack.top() == epsilon) stack.pop();
            else return false;
        }
        else {
            auto rule = M[stack.top().text][input[i].text]; // TODO validate
            if (rule.lhs == error) return false;
            stack.pop();
            push_vector(stack, rule.rhs);
        }
    }
    cout << endl;
    return false;
}

void Grammar::generateTestGrammar() {
    vector<Token> temp;
    temp.push_back({"term", NTERM});
    temp.push_back({"exp'", NTERM});
    rules.push_back({{"exp", NTERM}, temp});

    temp.clear();
    temp.push_back({"opsuma", NTERM});
    temp.push_back({"term", NTERM});
    temp.push_back({"exp'", NTERM});
    rules.push_back({{"exp'", NTERM}, temp});

    temp.clear();
    temp.push_back(epsilon);
    rules.push_back({{"exp'", NTERM}, temp});

    temp.clear();
    temp.push_back({"+", TERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"-", TERM});
    rules.push_back({{"opsuma", NTERM}, temp});

    temp.clear();
    temp.push_back({"factor", NTERM});
    temp.push_back({"term'", NTERM});
    rules.push_back({{"term", NTERM}, temp});

    temp.clear();
    temp.push_back({"opmult", NTERM});
    temp.push_back({"factor", NTERM});
    temp.push_back({"term'", NTERM});
    rules.push_back({{"term'", NTERM}, temp});

    temp.clear();
    temp.push_back(epsilon);
    rules.push_back({{"term'", NTERM}, temp});

    temp.clear();
    temp.push_back({"*", TERM});
    rules.push_back({{"opmult", NTERM}, temp});

    temp.clear();
    temp.push_back({"(", TERM});
    temp.push_back({"exp", NTERM});
    temp.push_back({")", TERM});
    rules.push_back({{"factor", NTERM}, temp});

    temp.clear();
    temp.push_back({"numero", TERM});
    rules.push_back({{"factor", NTERM}, temp});
}

void Grammar::printRules() {
    cout << "           RULES       " << endl;
    cout << "---------------------------" << endl;
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
    cout << "       First Sets       " << endl;
    cout << "-------------------" << endl;
    for (const auto &set : firstSets) {
        cout << set.first << " = ";
        for (const auto &token : set.second) {
            cout << token.text << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grammar::printNextSets() {
    cout << "       Next Sets       " << endl;
    cout << "-------------------" << endl;
    for (const auto &set : nextSets) {
        cout << set.first << " = ";
        for (const auto &token : set.second) {
            cout << token.text << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grammar::printTable() {
    cout << "           M Table       " << endl;
    cout << "---------------------------" << endl;
    auto firstRow = M.begin();
    cout << "M" << setw(4) << " | ";
    for (const auto& col : firstRow->second) {
        cout << col.first << setw(4) << " | ";
    }
    cout << endl;
    for (const auto& row : M) {
        cout << row.first << setw(4) << " | ";
        for (const auto& col : row.second) {
            cout << col.second;
            cout << setw(4) << " | ";
        }
        cout << endl;
    }
    cout << endl;
}
