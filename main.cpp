#include <iostream>
#include "Grammar.h"

int main() {
    Grammar ll1;
    ll1.printRules();
    ll1.getFirstSets();
    ll1.printFirstSets();
    ll1.getNextSets();
    ll1.printNextSets();
    ll1.buildTable();
    ll1.printTable();

    string validInput = "numero * ( numero + numero )";
    bool success = ll1.processString(validInput, true);
    if (success) cout << "Valid string" << endl;
    else cout << "Invalid string" << endl;

    string invalidInput = "numero ** (( numero + numero )";
    success = ll1.processString(invalidInput, false);
    if (success) cout << "Valid string" << endl;
    else cout << "Invalid string" << endl;
    return 0;
}
