#include <iostream>

#include "test/log_test.h"
#include "test/rc_test.h"
#include "test/memtable_test.h"
#include "test/skiplist_test.h"
#include "test/block_test.h"

int main() {
    // block test
    std::cout << "Running block tests...\n";
    test::block_test();
    std::cout << "Block tests completed.\n\n";

    // log test
    std::cout << "Running log tests...\n";
    test::log_test();
    std::cout << "Log tests completed.\n\n";

    // skiplist test
    std::cout << "Running skiplist tests...\n";
    test::skiplist_test();
    std::cout << "Skiplist tests completed.\n\n";

    // memtable tests
    std::cout << "Running memtable tests...\n";
    test::memtable_test();
    std::cout << "Memtable tests completed.\n";

    // return code test
    std::cout << "\nRunning RC enum tests...\n";
    test::rc_test();
    std::cout << "RC enum tests completed.\n";

    return 0;
}
