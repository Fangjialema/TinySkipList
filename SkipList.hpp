#ifndef SKIPLIST_HPP
#define SKIPLIST_HPP
#include <iostream>
#include <random>
#include <type_traits>
#include <vector>
using std::vector;
template <typename K, typename = void> class SkipList {};

template <typename K>
class SkipList<
    K, typename std::enable_if<
           std::is_same<decltype(std::declval<K>() < std::declval<K>()),
                        bool>::value &&
           std::is_same<decltype(std::declval<K>() == std::declval<K>()),
                        bool>::value &&
           std::is_same<decltype(std::declval<K>() != std::declval<K>()),
                        bool>::value>::type> {
  private:
    const int MAX_LEVEL = 4;
    int nowLevel;
    const float rate = 0.5;
    typedef struct ListNode {
        K val;
        vector<ListNode *> nextList;
        ListNode(K &&k, int level)
            : val(std::forward<K>(k)), nextList(level, nullptr) {}
        ListNode(const K &k, int level) : val(k), nextList(level, nullptr) {}
        ListNode(int level) : nextList(level, nullptr) {}
    } SNode;
    SNode *head;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    int randomLevel() {
        int level = 1;
        while (distribution(generator) < rate && level < MAX_LEVEL) {
            ++level;
        }
        return level;
    }
    void insert(SNode *newNode) {
        int newNodeLevel = newNode->nextList.size();
        if (newNodeLevel > nowLevel) {
            head->nextList.resize(newNodeLevel, nullptr);
            nowLevel = newNodeLevel;
        }
        auto current = head;
        auto &val = newNode->val;
        for (int i = nowLevel - 1; i >= 0; --i) {
            while (current->nextList[i] && current->nextList[i]->val < val) {
                current = current->nextList[i];
            }
            if (i < newNodeLevel) {
                if (current->val == val)
                    return;
                newNode->nextList[i] = current->nextList[i];
                current->nextList[i] = newNode;
            }
        }
    }

  public:
    SkipList() : nowLevel(1), distribution(0, 1) {
        head = new SNode(1);
        std::random_device rd;
        // generator.seed(rd());
        generator.seed(1);
    }
    ~SkipList() {
        auto current = head;
        while (current != nullptr) {
            auto next = current->nextList[0];
            delete current;
            current = next;
        }
    }
    void insert(K &&k) {
        auto newNode = new SNode(std::forward<K>(k), randomLevel());
        insert(newNode);
    }
    void insert(const K &k) {
        auto newNode = new SNode(k, randomLevel());
        insert(newNode);
    }
    bool search(const K &k) {
        auto current = head;
        for (int i = nowLevel - 1; i >= 0; --i) {
            while (current->nextList[i] && current->nextList[i]->val < k) {
                current = current->nextList[i];
            }
            if (current->nextList[i] && current->nextList[i]->val == k) {
                return true;
            }
        }
        return false;
    }
    void remove(const K &k) {
        auto current = head;
        SNode* temp=nullptr;
        for (int i = nowLevel - 1; i >= 0; --i) {
            while (current->nextList[i] && current->nextList[i]->val < k) {
                current = current->nextList[i];
            }
            if (current->nextList[i] && current->nextList[i]->val == k) {
                auto node = current->nextList[i];
                current->nextList[i] = current->nextList[i]->nextList[i];   
                temp=node;             
            }
        }
        delete temp;
    }
    void print() {
        for (int i = nowLevel - 1; i >= 0; i--) {
            SNode *current = head->nextList[i];
            std::cout << "Level " << i << ": ";
            while (current) {
                std::cout << current->val << " ";
                current = current->nextList[i];
            }
            std::cout << std::endl;
        }
    }
};
#endif
