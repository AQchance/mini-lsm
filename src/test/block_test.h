#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "block/block.h"
#include "block/block_builder.h"
#include "common/rc.h"

namespace test {

void block_test() {
    using namespace mini_lsm;

    std::cout << "  Testing BlockBuilder and Block encoding/decoding..." << '\n';

    mini_lsm::RC rc = mini_lsm::RC::SUCCESS;

    // 1. Test basic building and encoding/decoding
    {
        BlockBuilder builder(4096);
        assert(builder.add("key1", "value1"));
        assert(builder.add("key2", "value2"));
        assert(builder.add("key3", "value3"));

        Block block = builder.build();
        std::vector<uint8_t> encoded;
        rc = block.encode(encoded);
        assert(rc == mini_lsm::RC::SUCCESS);
        Block decoded_block({}, {});
        rc = Block::decode(encoded, decoded_block);
        assert(rc == mini_lsm::RC::SUCCESS);

        // Verify offsets count
        assert(decoded_block.offsets().size() == 3);
        assert(block.offsets() == decoded_block.offsets());
        assert(block.data() == decoded_block.data());
    }

    // 2. Test BlockBuilder size limit
    {
        // Create a small block builder
        // "key" (3) + 2 + "value" (5) + 2 + offset(2) = 14 bytes per entry
        // We set limit to allow roughly 2 entries
        BlockBuilder builder(30);

        assert(builder.add("123", "12345") == true);  // 14 bytes
        assert(builder.add("234", "56789") == true);  // 28 bytes
        // Next one should fail or be forced if it's empty (but it's not)
        // 14 + 14 + 14 = 42 > 30.
        // The check is: if (!is_empty() && current_size + entry_size + overhead > block_size)
        // current_size will be roughly 28 + 2 (footer) = 30?
        // Let's check calculation in code:
        // current_size() = data_.size() + offsets_.size()*2 + 2.
        // After 1 entry: data=12, offsets=1*2, footer=2 => 16 bytes.
        // After 2 entries: data=24, offsets=2*2, footer=2 => 30 bytes.

        // So after 2 entries, current_size is 30.
        // Adding 3rd entry: 30 + 14 > 30. Should return false.
        assert(builder.add("345", "54321") == false);

        Block block = builder.build();
        assert(block.offsets().size() == 2);
    }

    std::cout << "  Block tests passed!" << '\n';
}

}  // namespace test
