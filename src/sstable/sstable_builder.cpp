#include "sstable/sstable_builder.h"

#include "common/rc.h"
#include "log/log.h"

namespace mini_lsm {

RC SSTableBuilder::add(std::string_view key, std::string_view value) {
    // update the block min/max keys
    if (block_builder_.is_empty()) {
        current_block_min_key_ = key;
        current_block_max_key_ = key;
    } else {
        if (key < current_block_min_key_) {
            current_block_min_key_ = key;
        }
        if (key > current_block_min_key_) {
            current_block_max_key_ = key;
        }
    }
    if (RC_FAIL(block_builder_.add(key, value))) {
        // out-of-size, need to flush current block first
        // flush current block
        RC rc = flush_current_block();
        if (RC_FAIL(rc)) {
            LOG_ERROR("Failed to flush current block: %s", rc_to_string(rc));
        }
        // after flushing, add the key-value again
        rc = block_builder_.add(key, value);
        if (RC_FAIL(rc)) {
            LOG_ERROR("Failed to add key-value after flushing block: %s", rc_to_string(rc));
            return rc;
        }
    }
    return RC::SUCCESS;
}

RC SSTableBuilder::flush_current_block() {
    Block block = block_builder_.build();
    std::vector<uint8_t> encoded_block_data;
    RC rc = block.encode(encoded_block_data);
    if (RC_FAIL(rc)) {
        LOG_ERROR("Failed to encode block: %s", rc_to_string(rc));
        return rc;
    }

    // NOTE: now we just assume that the blocks are storged in memory, not in files
    // append encoded block data to sstable data
    uint64_t offset = sstable_data_.size();
    sstable_data_.insert(sstable_data_.end(), encoded_block_data.begin(), encoded_block_data.end());

    // record block meta
    BlockMeta meta;
    meta.offset = offset;
    meta.size = static_cast<uint32_t>(encoded_block_data.size());
    meta.min_key = current_block_min_key_;
    meta.max_key = current_block_max_key_;
    block_metas_.push_back(std::move(meta));

    // reset current block min/max keys
    current_block_min_key_.clear();
    current_block_max_key_.clear();

    return RC::SUCCESS;
}

}  // namespace mini_lsm
