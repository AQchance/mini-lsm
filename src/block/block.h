#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "common/rc.h"

namespace mini_lsm {
/**
 * Block represents a block of key-value pairs stored in a contiguous byte array.
 * It contains the encoded data and offsets to each key-value entry.
 */

class Block {
   public:
    // simple constructor for testing or builder usage
    Block(std::vector<uint8_t> data, std::vector<uint16_t> offsets)
        : data_(std::move(data)), offsets_(std::move(offsets)) {}

    ~Block() = default;

    // decodes a block from a byte buffer
    static RC decode(const std::vector<uint8_t> &data, Block &decode_block);

    // encodes the block back to a byte buffer
    RC encode(std::vector<uint8_t> &encode_data) const;

    // get the rew data
    const std::vector<uint8_t> &data() const {
        return data_;
    }

    // get the offsets
    const std::vector<uint16_t> &offsets() const {
        return offsets_;
    }

   private:
    std::vector<uint8_t> data_;
    std::vector<uint16_t> offsets_;
};

}  // namespace mini_lsm
