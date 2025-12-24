#include "common/rc.h"

namespace mini_lsm {

const char *rc_to_string(RC rc) {
    switch (rc) {
        case RC::SUCCESS:
            return "RC::SUCCESS";
        case RC::INVALID_ARGUMENT:
            return "RC::INVALID_ARGUMENT";
        case RC::NO_IMPLEMENTATION:
            return "RC::NO_IMPLEMENTATION";
        case RC::ERROR:
            return "RC::ERROR";
        case RC::NOT_FOUND:
            return "RC::NOT_FOUND";
    }
    return "UNKNOWN_RC";
}

bool RC_SUCC(RC rc) {
    return rc == RC::SUCCESS;
}

bool RC_FAIL(RC rc) {
    return rc != RC::SUCCESS;
}

}  // namespace mini_lsm
