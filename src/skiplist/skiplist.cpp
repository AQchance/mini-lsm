#include "skiplist/skiplist.h"

#include <ctime>

#include "common/rc.h"

namespace mini_lsm {
SkipList::SkipList() : current_level_(1), size_(0) {
    // seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // initialize header node with maximum level
    header_ = new SkipListNode(kMaxLevel);
}

SkipList::~SkipList() {
    // delete all nodes in the skip list
    // begin from the header node
    SkipListNode *current = header_;
    while (current != nullptr) {
        SkipListNode *next = current->forward[0];
        delete current;
        current = next;
    }
}

int SkipList::random_level() const {
    int level = 1;

    // increase level with 1/kBranching probability
    while (std::rand() % kBranching == 0 && level < kMaxLevel) {
        level++;
    }
    return level;
}

std::vector<SkipListNode *> SkipList::find_predecessors(const std::string &key) const {
    std::vector<SkipListNode *> predecessors(kMaxLevel, nullptr);
    SkipListNode *current = header_;

    // traverse from the highest level down to level 0
    for (int i = current_level_ - 1; i >= 0; i--) {
        // move forward while the next node's key is less than the target key
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        predecessors[i] = current;
    }

    return predecessors;
}

void SkipList::insert(const std::string &key, const std::string &value) {
    // find predecessors at each level
    std::vector<SkipListNode *> predecessors = find_predecessors(key);

    // check if the key already exists
    SkipListNode *current = predecessors[0]->forward[0];
    if (current != nullptr && current->key == key) {
        // key exists, update the value
        current->value = value;
        return;
    }

    // generate a random level for the new node
    int new_level = random_level();

    // if new level is greater than current level, update predecessors
    if (new_level > current_level_) {
        for (int i = current_level_; i < new_level; i++) {
            predecessors[i] = header_;
        }
        current_level_ = new_level;
    }

    // create the new node
    SkipListNode *new_node = new SkipListNode(key, value, new_level);

    // update forward pointers
    for (int i = 0; i < new_level; i++) {
        new_node->forward[i] = predecessors[i]->forward[i];
        predecessors[i]->forward[i] = new_node;
    }

    size_++;
}

RC SkipList::get(const std::string &key, std::string &value) const {
    SkipListNode *current = header_;

    // traverse from the highest level down to level 0
    for (int i = current_level_ - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    // move to the next node at level 0
    current = current->forward[0];

    // check if the key is found
    if (current != nullptr && current->key == key) {
        value = current->value;
    }
    return RC::KEY_NOT_EXIST;
}

bool SkipList::contains(const std::string &key) const {
    std::string value;
    RC rc = get(key, value);
    return RC_SUCC(rc);
}

void SkipList::remove(const std::string &key) {
    // find predecessors at each level
    std::vector<SkipListNode *> predecessors = find_predecessors(key);

    // check if the key exists
    SkipListNode *target = predecessors[0]->forward[0];
    if (target == nullptr || target->key != key) {
        // key not found, nothing to remove
        return;
    }

    // update forward pointers to remove the target node
    for (int i = 0; i < current_level_; i++) {
        if (predecessors[i]->forward[i] != target) {
            break;
        }
        predecessors[i]->forward[i] = target->forward[i];
    }

    // delete the target node
    delete target;
    size_--;

    // update current level if necessary
    while (current_level_ > 1 && header_->forward[current_level_ - 1] == nullptr) {
        current_level_--;
    }
}

SkipListIterator SkipList::begin() const {
    return SkipListIterator(header_->forward[0]);
}

SkipListIterator SkipList::end() const {
    return SkipListIterator(nullptr);
}

}  // namespace mini_lsm
