#include "block/block_builder.h"

#include <cstring>

namespace mini_lsm {

BlockBuilder::BlockBuilder(size_t block_size) : block_size_(block_size) {
    data_.reserve(block_size);
    offsets_.reserve(block_size / 64);
}

RC BlockBuilder::add(std::string_view key, std::string_view value) {
    size_t entry_size = sizeof(uint16_t) + key.size() + sizeof(uint16_t) + value.size();
    size_t overhead = sizeof(uint16_t);  // for the offset

    if (!is_empty() && current_size() + entry_size + overhead > block_size_) {
        return RC::OUT_OF_SIZE;
    }

    offsets_.push_back(static_cast<uint16_t>(data_.size()));

    // Encode key length
    uint16_t key_len = static_cast<uint16_t>(key.size());
    size_t current_pos = data_.size();
    data_.resize(current_pos + sizeof(uint16_t));
    memcpy(&data_[current_pos], &key_len, sizeof(uint16_t));

    // Encode key
    data_.insert(data_.end(), key.begin(), key.end());

    // Encode value length
    uint16_t val_len = static_cast<uint16_t>(value.size());
    current_pos = data_.size();
    data_.resize(current_pos + sizeof(uint16_t));
    memcpy(&data_[current_pos], &val_len, sizeof(uint16_t));

    // Encode value
    data_.insert(data_.end(), value.begin(), value.end());

    return RC::SUCCESS;
}

Block BlockBuilder::build() {
    Block block(std::move(data_), std::move(offsets_));

    // Reset state
    data_.clear();
    offsets_.clear();
    data_.reserve(block_size_);

    return block;
}

size_t BlockBuilder::current_size() const {
    return data_.size() + offsets_.size() * sizeof(uint16_t) +
           sizeof(uint16_t);  // data + offsets + num_elements(u16)
}

bool BlockBuilder::is_empty() const {
    return offsets_.empty();
}

}  // namespace mini_lsm
