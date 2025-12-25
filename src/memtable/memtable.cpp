#include "memtable/memtable.h"

namespace mini_lsm {

// An empty string is used as a tombstone to indicate that a key has been deleted.
const std::string TOMBSTONE = "";

MemTable::MemTable() : table_() {}

MemTable::~MemTable() {}

std::optional<std::string> MemTable::get(const std::string &key) {
    auto result = table_.get(key);
    if (result.has_value() && result.value() == TOMBSTONE) {
        // The key was found but is marked as deleted.
        return std::nullopt;
    }
    return result;
}

void MemTable::put(const std::string &key, const std::string &value) {
    table_.insert(key, value);
}

void MemTable::remove(const std::string &key) {
    // Insert a tombstone to mark the key as deleted.
    table_.insert(key, TOMBSTONE);
}

SkipListIterator MemTable::begin() const {
    return table_.begin();
}

SkipListIterator MemTable::end() const {
    return table_.end();
}

size_t MemTable::size() const {
    return table_.size();
}

}  // namespace mini_lsm
