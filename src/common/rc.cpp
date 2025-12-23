#include "common/rc.h"

#include <string_view>

namespace mini_lsm {

std::string_view rc_to_string(RC rc) {
    switch (rc) {
        case RC::SUCCESS:
            return "SUCCESS";
        case RC::INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";
        case RC::NO_IMPLEMENTATION:
            return "NO_IMPLEMENTATION";
        case RC::ERROR:
            return "ERROR";
        case RC::NOT_FOUND:
            return "NOT_FOUND";
    }
}

}  // namespace mini_lsm
