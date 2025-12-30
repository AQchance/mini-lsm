#include "memtable/memtable.h"

namespace mini_lsm {

// An empty string is used as a tombstone to indicate that a key has been deleted.

MemTable::MemTable() : table_() {}

MemTable::~MemTable() {}

RC MemTable::get(const std::string &key, std::string &value) {
    RC rc = table_.get(key, value);
    if (RC_SUCC(rc) && value == TOMBSTONE) {
        // The key was found but marked as deleted
        return RC::KEY_NOT_EXIST;
    }
    return rc;
}

void MemTable::put(const std::string &key, const std::string &value) {
    table_.insert(key, value);
}

void MemTable::remove(const std::string &key) {
    // Insert a tombstone to mark the key as deleted
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

};  // namespace mini_lsm
