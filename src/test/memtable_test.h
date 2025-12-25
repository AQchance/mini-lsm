#pragma once

#include <cassert>
#include <iostream>

#include "memtable/memtable.h"

namespace test {

void memtable_test() {
    mini_lsm::MemTable memtable;

    // 1. Test Put and Get
    memtable.put("key1", "value1");
    auto val1 = memtable.get("key1");
    assert(val1.has_value() && val1.value() == "value1");
    std::cout << "Test 1 Passed: Put and Get successful.\n";

    // 2. Test Overwrite
    memtable.put("key1", "value1_new");
    auto val1_new = memtable.get("key1");
    assert(val1_new.has_value() && val1_new.value() == "value1_new");
    std::cout << "Test 2 Passed: Overwrite successful.\n";

    // 3. Test Get non-existent key
    auto val2 = memtable.get("key2");
    assert(!val2.has_value());
    std::cout << "Test 3 Passed: Get non-existent key successful.\n";

    // 4. Test Remove (Tombstone)
    memtable.put("key3", "value3");
    memtable.remove("key3");
    auto val3 = memtable.get("key3");
    assert(!val3.has_value());  // Should be 'not found' due to tombstone
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
