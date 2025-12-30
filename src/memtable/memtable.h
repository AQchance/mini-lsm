#pragma once

#include "common/rc.h"
#include "skiplist/skiplist.h"

namespace mini_lsm {

constexpr std::string TOMBSTONE = "";

class MemTable {
   public:
    MemTable();
    ~MemTable();

    // forbid copy and assignment
    MemTable(const MemTable &) = delete;
    MemTable &operator=(const MemTable &) = delete;

    // Get a value from the memtable
    RC get(const std::string &key, std::string &value);

    // Put a key-value pair into the memtable
    void put(const std::string &key, const std::string &value);

    // Remove a key from the memtable
    void remove(const std::string &key);

    // Get an iterator to the begining of the memtable
    SkipListIterator begin() const;

    // Get an iterator to the end of the memtable
    SkipListIterator end() const;

    // Get an iterator to the end of the memtable
    size_t size() const;

   private:
    SkipList table_;
};

};  // namespace mini_lsm
