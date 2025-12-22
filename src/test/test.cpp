#include <iostream>

#include "test/log_test.h"
#include "test/skiplist_test.h"

int main() {
    // log tests
    std::cout << "Running log tests...\n";
    test::log_test();
    std::cout << "Log tests completed.\n\n";

    // skiplist tests
    std::cout << "Running skiplist tests...\n";
    test::skiplist_test();
    std::cout << "Skiplist tests completed.\n";

    return 0;
}
