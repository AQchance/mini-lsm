#include "block/block.h"

#include <cstdint>
#include <cstring>

namespace mini_lsm {

RC Block::decode(const std::vector<uint8_t> &data, Block &decode_block) {
    RC rc = RC::SUCCESS;
    if (data.size() < sizeof(uint16_t)) {
        return RC::INVALID_ARGUMENT;
    }

    // Read number of elements from the end
    size_t data_len = data.size();
    uint16_t num_elements;
    // safely read the number of elements
    num_elements = *reinterpret_cast<const uint16_t *>(&data[data_len - sizeof(uint16_t)]);

    // calculate the start of the offsets section
    size_t offsets_start = data_len - sizeof(uint16_t) - num_elements * sizeof(uint16_t);

    if (offsets_start > data_len) {
        return RC::INVALID_ARGUMENT;
    }

    std::vector<uint16_t> offsets;
    offsets.reserve(num_elements);

    // read offsets
    for (int i = 0; i < num_elements; ++i) {
        uint16_t offset;
        memcpy(&offset, &data[offset + i * sizeof(uint16_t)], sizeof(uint16_t));
        offsets.push_back(offset);
    }

    // isolate the actual data part
    std::vector<uint8_t> actual_data(data.begin(), data.begin() + offsets_start);
    Block(std::move(actual_data), std::move(offsets));
    return rc;
}

RC Block::encode(std::vector<uint8_t> &encode_data) const {
    RC rc = RC::SUCCESS;
    size_t initial_size = encode_data.size();

    // Reserve space: current data + offsets + num_elements (u16)
    encode_data.reserve(initial_size + offsets_.size() * sizeof(uint16_t) + sizeof(uint16_t));

    // Append offsets
    for (uint16_t offset : offsets_) {
        size_t pos = encode_data.size();
        encode_data.resize(pos + sizeof(uint16_t));
        memcpy(&encode_data[pos], &offset, sizeof(uint16_t));
    }

    // Append number of elements
    uint16_t num_elements = static_cast<uint16_t>(offsets_.size());
    size_t pos = encode_data.size();
    encode_data.resize(pos + sizeof(uint16_t));
    memcpy(&encode_data[pos], &num_elements, sizeof(uint16_t));

    return rc;
}

}  // namespace mini_lsm
