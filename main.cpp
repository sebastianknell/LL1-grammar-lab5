#include <iostream>
#include "Grammar.h"

int main() {
    Grammar ll1;
    ll1.printRules();
    ll1.getFirstSets();
    ll1.printFirstSets();

    return 0;
}
