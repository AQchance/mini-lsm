#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "block/block.h"

namespace mini_lsm {

/**
 * BlockBuilder builds a block by appending key-value pairs.
 */
class BlockBuilder {
   public:
    explicit BlockBuilder(size_t block_size);

    // Adds a key-value pair to the block.
    // Returns true if the pair was added successfully.
    // Returns false if adding the pair would exceed the block size.
    // (Note: The first pair is always added regardless of size limit to ensure progress)
    RC add(std::string_view key, std::string_view value);

    // Finishes the block and returns the constructed Block.
    // This resets the builder state.
    Block build();

    // Returns the current estimated size of the block.
    size_t current_size() const;

    // Returns true if the block is empty.
    bool is_empty() const;

   private:
    std::vector<uint8_t> data_;
    std::vector<uint16_t> offsets_;
    size_t block_size_;
};

}  // namespace mini_lsm
