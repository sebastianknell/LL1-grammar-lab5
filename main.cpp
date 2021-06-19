#include <iostream>
#include "Grammar.h"

int main() {
    Grammar ll1;
    ll1.printRules();
    ll1.getFirstSets();
    ll1.printFirstSets();
    ll1.getNextSets();
    ll1.printNextSets();

    return 0;
}
