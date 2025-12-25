#pragma once

#include "skiplist/skiplist.h"

namespace mini_lsm {

class MemTable {
   public:
    MemTable();
    ~MemTable();

    // forbid copy and assignment
    MemTable(const MemTable &) = delete;
    MemTable &operator=(const MemTable &) = delete;

    // Get a value from the memtable.
    std::optional<std::string> get(const std::string &key);

    // Put a key-value pair into the memtable.
    void put(const std::string &key, const std::string &value);

    // Remove a key from the memtable by inserting a tombstone.
    void remove(const std::string &key);

    // Get an iterator to the beginning of the memtable.
    SkipListIterator begin() const;

    // Get an iterator to the end of the memtable.
    SkipListIterator end() const;

    // Get the number of elements in the memtable.
    size_t size() const;

   private:
    SkipList table_;
};

}  // namespace mini_lsm
