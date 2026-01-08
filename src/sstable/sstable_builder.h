#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "block/block_builder.h"
#include "common/rc.h"

namespace mini_lsm {

struct BlockMeta {
    uint64_t offset;      // Offset of the block in the SSTable file
    uint32_t size;        // Size of the block in bytes
    std::string min_key;  // Minimum key in the block
    std::string max_key;  // Maximum key in the block
};

class SSTableBuilder {
   public:
    explicit SSTableBuilder(size_t block_size) : block_builder_(block_size) {}
    // add key-value pair to the SSTable
    RC add(std::string_view key, std::string_view value);

    size_t current_size() const {
        return sstable_data_.size();
    }

   private:
    RC flush_current_block();

   private:
    BlockBuilder block_builder_;
    std::vector<BlockMeta> block_metas_;
    std::vector<uint8_t> sstable_data_;

    std::string current_block_min_key_;
    std::string current_block_max_key_;
};

}  // namespace mini_lsm
