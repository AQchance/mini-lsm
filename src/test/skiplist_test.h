#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include "skiplist/skiplist.h"

namespace test {

inline void skiplist_test() {
    std::cout << "=== Skip List Tests ===\n";

    // test 1: basic insert and get
    {
        std::cout << "Test 1: Basic insert and get... ";
        mini_lsm::SkipList list;

        list.insert("key1", "value1");
        list.insert("key2", "value2");
        list.insert("key3", "value3");

        assert(list.get("key1") == "value1");
        assert(list.get("key2") == "value2");
        assert(list.get("key3") == "value3");
        assert(list.size() == 3);

        std::cout << "PASSED\n";
    }

    // test 2: update existing key
    {
        std::cout << "Test 2: Update existing key... ";
        mini_lsm::SkipList list;

        list.insert("key1", "value1");
        list.insert("key1", "value1_updated");

        assert(list.get("key1") == "value1_updated");
        assert(list.size() == 1);

        std::cout << "PASSED\n";
    }

    // test 3: get non-existent key
    {
        std::cout << "Test 3: Get non-existent key... ";
        mini_lsm::SkipList list;

        list.insert("key1", "value1");

        assert(!list.get("key2").has_value());
        assert(!list.contains("key2"));
        assert(list.contains("key1"));

        std::cout << "PASSED\n";
    }

    // test 4: remove operation
    {
        std::cout << "Test 4: Remove operation... ";
        mini_lsm::SkipList list;

        list.insert("key1", "value1");
        list.insert("key2", "value2");
        list.insert("key3", "value3");

        list.remove("key2");

        assert(list.get("key1") == "value1");
        assert(!list.get("key2").has_value());
        assert(list.get("key3") == "value3");
        assert(list.size() == 2);

        std::cout << "PASSED\n";
    }

    // test 5: iterator traversal
    {
        std::cout << "Test 5: Iterator traversal... ";
        mini_lsm::SkipList list;

        list.insert("c", "3");
        list.insert("a", "1");
        list.insert("b", "2");

        // range-based for loop
        std::string result;
        for (auto it = list.begin(); it != list.end(); ++it) {
            result += it.key();
        }
        assert(result == "abc");

        std::cout << "PASSED\n";
    }

    // test 6: large dataset
    {
        std::cout << "Test 6: Large dataset... ";
        mini_lsm::SkipList list;

        const int count = 1000;
        for (int i = 0; i < count; i++) {
            list.insert("key" + std::to_string(i), "value" + std::to_string(i));
        }

        assert(list.size() == count);

        for (int i = 0; i < count; i++) {
            auto val = list.get("key" + std::to_string(i));
            assert(val.has_value());
            assert(val.value() == "value" + std::to_string(i));
        }

        std::cout << "PASSED\n";
    }

    // test 7: empty list operations
    {
        std::cout << "Test 7: Empty list operations... ";
        mini_lsm::SkipList list;

        assert(list.empty());
        assert(list.size() == 0);
        assert(!list.get("any").has_value());
        list.remove("any");  // 不应该崩溃
        assert(list.begin() == list.end());

        std::cout << "PASSED\n";
    }

    std::cout << "=== All Skip List Tests Passed! ===\n";
}

}  // namespace test
