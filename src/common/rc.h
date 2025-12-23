#pragma once

namespace mini_lsm {

// if you add new return codes, remember to update rc_to_string in rc.cpp
enum class RC {
    SUCCESS,
    INVALID_ARGUMENT,
    NO_IMPLEMENTATION,
    ERROR,
    NOT_FOUND,
};

const char *rc_to_string(RC rc);

}  // namespace mini_lsm
