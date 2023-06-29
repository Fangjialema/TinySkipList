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
    const int MAX_LEVEL = 8;
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
    void find(const K &val, vector<SNode *> &pre) {
        SNode *p = head;
        for (int i = nowLevel - 1; i >= 0; --i) {
            while (p->nextList[i] && p->nextList[i]->val < val)
                p = p->nextList[i];
            pre[i] = p;
        }
    }
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    int randomLevel() {
        int level = 1;
        while (distribution(generator) < rate && level < MAX_LEVEL) {
            ++level;
        }
        return level;
    }
    void add(SNode *newNode) {
        if (newNode->nextList.size() > nowLevel) {
            nowLevel = newNode->nextList.size();
            head->nextList.resize(nowLevel, nullptr);
        }
        auto current = head;
        for (int i = nowLevel - 1; i >= 0; --i) {
            while (current->nextList[i] != nullptr &&
                   current->nextList[i]->val < newNode->val) {
                current = current->nextList[i];
            }
            if (i < newNode->nextList.size()) {
                newNode->nextList[i] = current->nextList[i];
                current->nextList[i] = newNode;
            }
        }
    }

  public:
    SkipList() : nowLevel(1), distribution(0, 1) {
        head = new SNode(1);
        std::random_device rd;
        generator.seed(rd());
    }
    ~SkipList() {
        auto current = head;
        while (current != nullptr) {
            auto next = current->nextList[0];
            delete current;
            current = next;
        }
    }
    bool search(const K &val) {
        vector<SNode *> pre(nowLevel);
        find(val, pre);
        auto p = pre[0]->nextList[0];
        return p && p->val == val;
    }
    void add(K &&val) {
        auto newNode = new SNode(std::forward<K>(val), randomLevel());
        add(newNode);
    }
    void add(const K &val) {
        auto newNode = new SNode(val, randomLevel());
        add(newNode);
    }

    bool remove(const K &&val) {
        vector<SNode *> pre(nowLevel);
        find(val, pre);
        auto p = pre[0]->nextList[0];
        if (!p || p->val != val)
            return false;
        for (int i = 0; i < nowLevel && pre[i]->nextList[i] == p; ++i) {
            pre[i]->nextList[i] = p->nextList[i];
        }
        delete p;
        return true;
    }
    void print() {
        for (int i = nowLevel - 1; i >= 0; i--) {
            SNode *current = head->nextList[i];
            std::cout << "Level " << i << ": ";
            while (current != nullptr) {
                std::cout << current->val << " ";
                current = current->nextList[i];
            }
            std::cout << std::endl;
        }
    }
};
#endif
