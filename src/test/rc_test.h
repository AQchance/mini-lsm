#include <cstdio>
#include <iostream>

#include "common/rc.h"

namespace test {

void rc_test() {
    using namespace mini_lsm;
    std::cout << "=== RC Enum Tests ===\n";
    printf("RC::SUCCESS: %s\n", rc_to_string(RC::SUCCESS));
    printf("RC::INVALID_ARGUMENT: %s\n", rc_to_string(RC::INVALID_ARGUMENT));
    printf("RC::NO_IMPLEMENTATION: %s\n", rc_to_string(RC::NO_IMPLEMENTATION));
    printf("RC::ERROR: %s\n", rc_to_string(RC::ERROR));
    printf("RC::NOT_FOUND: %s\n", rc_to_string(RC::NOT_FOUND));

    printf("RC_SUCC(RC::SUCCESS): %s\n", RC_SUCC(RC::SUCCESS) ? "true" : "false");
    printf("RC_SUCC(RC::ERROR): %s\n", RC_SUCC(RC::ERROR) ? "true" : "false");
    printf("RC_FAIL(RC::SUCCESS): %s\n", RC_FAIL(RC::SUCCESS) ? "true" : "false");
    printf("RC_FAIL(RC::ERROR): %s\n", RC_FAIL(RC::ERROR) ? "true" : "false");
}

}  // namespace test
