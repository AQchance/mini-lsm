#pragma once

#include <cassert>
#include <iostream>

#include "common/rc.h"
#include "memtable/memtable.h"

namespace test {

void memtable_test() {
    mini_lsm::MemTable memtable;
    mini_lsm::RC rc = mini_lsm::RC::SUCCESS;

    // 1. Test Put and Get
    memtable.put("key1", "value1");
    std::string result;
    rc = memtable.get("key1", result);
    assert(RC_SUCC(rc) && result != mini_lsm::TOMBSTONE && result == "value1");
    std::cout << "Test 1 Passed: Put and Get successful.\n";

    // 2. Test Overwrite
    memtable.put("key1", "value1_new");
    rc = memtable.get("key1", result);
    assert(RC_SUCC(rc) && result != mini_lsm::TOMBSTONE && result == "value1_new");
    std::cout << "Test 2 Passed: Overwrite successful.\n";

    // 3. Test Get non-existent key
    std::string empty_str;
    rc = memtable.get("key2", empty_str);
    assert(rc == mini_lsm::RC::KEY_NOT_EXIST);
    std::cout << "Test 3 Passed: Get non-existent key successful.\n";

    // 4. Test Remove (Tombstone)
    memtable.put("key3", "value3");
    memtable.remove("key3");
    rc = memtable.get("key3", result);
    assert(rc == mini_lsm::RC::KEY_NOT_EXIST);
    std::cout << "Test 4 Passed: Remove (tombstone) successful.\n";

    // 5. Test iteration
    memtable.put("keyA", "valueA");
    memtable.put("keyC", "valueC");
    memtable.put("keyB", "valueB");

    auto it = memtable.begin();

    assert(it.key() == "key1");
    assert(it.value() == "value1_new");
    ++it;

    assert(it.key() == "key3");
    assert(it.value() == "");  // Tombstone
    ++it;

    assert(it.key() == "keyA");
    assert(it.value() == "valueA");
    ++it;

    assert(it.key() == "keyB");
    assert(it.value() == "valueB");
    ++it;

    assert(it.key() == "keyC");
    assert(it.value() == "valueC");
    ++it;

    assert(!it.valid());
    std::cout << "Test 5 Passed: Iteration order is correct.\n";
}

}  // namespace test
