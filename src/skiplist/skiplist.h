#pragma once

#include <optional>
#include <string>
#include <vector>

namespace mini_lsm {
// the maximum number of levels in the skip list
constexpr int kMaxLevel = 12;
// the branching factor for the skip list
constexpr int kBranching = 4;

struct SkipListNode {
    std::string key;
    std::string value;
    std::vector<SkipListNode *> forward;  // forward pointers for each level

    SkipListNode(const std::string &k, const std::string &v, int level)
        : key(k), value(v), forward(level, nullptr) {}
    explicit SkipListNode(int level) : forward(level, nullptr) {}
};

// forward declaration
class SkipListIterator;

// skip list class

class SkipList {
   public:
    SkipList();
    ~SkipList();

    // forbid copy and assignment
    SkipList(const SkipList &) = delete;
    SkipList &operator=(const SkipList &) = delete;

    // insert and search methods
    void insert(const std::string &key, const std::string &value);

    // returns std::nullprtopt if not found
    std::optional<std::string> get(const std::string &key) const;

    // check if the key exists
    bool contains(const std::string &key) const;

    // remove a key from the skip list
    void remove(const std::string &key);

    // return the number of elements in the skip list
    size_t size() const {
        return size_;
    }

    // check if the skip list is empty
    bool empty() const {
        return size_ == 0;
    }

    // support iteration
    SkipListIterator begin() const;
    SkipListIterator end() const;

   private:
    // generate a random level for a new node
    int random_level() const;

    // find the predecessors of a given key at each level
    std::vector<SkipListNode *> find_predecessors(const std::string &key) const;

   private:
    size_t size_;           // number of elements in the skip list
    int current_level_;     // current maximum level of the skip list
    SkipListNode *header_;  // header node
};

// SkipListIterator implementation
class SkipListIterator {
   public:
    using value_type = std::pair<std::string, std::string>;

    SkipListIterator() : node_(nullptr) {}
    explicit SkipListIterator(SkipListNode *node) : node_(node) {}

    // dereference
    value_type operator*() const {
        return {node_->key, node_->value};
    }

    // get key
    const std::string &key() const {
        return node_->key;
    }

    // get value
    const std::string &value() const {
        return node_->value;
    }

    // prefix increment
    SkipListIterator &operator++() {
        if (node_) {
            node_ = node_->forward[0];
        }
        return *this;
    }

    // postfix increment
    SkipListIterator operator++(int) {
        SkipListIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // comparison operators
    bool operator==(const SkipListIterator &other) const {
        return node_ == other.node_;
    }

    bool operator!=(const SkipListIterator &other) const {
        return node_ != other.node_;
    }

    // check if valid
    bool valid() const {
        return node_ != nullptr;
    }

   private:
    SkipListNode *node_;
};

}  // namespace mini_lsm

