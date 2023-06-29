#include <iostream>
#include <vector>
#include <random>

// 跳表节点
struct SkipListNode {
    int value;
    std::vector<SkipListNode*> forward;

    SkipListNode(int val, int level) : value(val), forward(level, nullptr) {}
};

// 跳表
class SkipList {
private:
    int maxLevel; // 最大层数
    int currentLevel; // 当前层数
    SkipListNode* head; // 头节点
    std::random_device rd; // 随机数生成器
    std::default_random_engine generator; // 随机数引擎
    std::uniform_int_distribution<int> distribution; // 随机数分布

    // 返回一个随机层数
    int randomLevel() {
        int level = 1;
        while (distribution(generator) < 0.5 && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList(int maxLvl) : maxLevel(maxLvl), currentLevel(1), head(new SkipListNode(0, maxLvl)), distribution(0, 1) {
        std::random_device seed;
        generator.seed(seed());
    }

    // 插入元素
    void insert(int value) {
        // 创建一个新节点
        SkipListNode* newNode = new SkipListNode(value, randomLevel());

        // 如果新节点的层数大于当前层数，更新头节点的forward指针
        if (newNode->forward.size() > currentLevel) {
            head->forward.resize(newNode->forward.size(), nullptr);
            currentLevel = newNode->forward.size();
        }

        // 找到插入位置的前驱节点
        SkipListNode* current = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            if (i < newNode->forward.size()) {
                newNode->forward[i] = current->forward[i];
                current->forward[i] = newNode;
            }
        }
    }

    // 查找元素
    bool search(int value) {
        SkipListNode* current = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return (current != nullptr && current->value == value);
    }

    // 删除元素
    void remove(int value) {
        SkipListNode* current = head;
        std::vector<SkipListNode*> update(currentLevel, nullptr);
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        current = current->forward[0];
        if (current != nullptr && current->value == value) {
            for (int i = 0; i < currentLevel; i++) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (currentLevel > 1 && head->forward[currentLevel - 1] == nullptr) {
                currentLevel--;
            }
        }
    }

    // 打印跳表
    void print() {
        for (int i = currentLevel - 1; i >= 0; i--) {
            SkipListNode* current = head->forward[i];
            std::cout << "Level " << i << ": ";
            while (current != nullptr) {
                std::cout << current->value << " ";
                current = current->forward[i];
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    SkipList skipList(4);

    skipList.insert(3);
    skipList.insert(6);
    skipList.insert(2);
    skipList.insert(8);
    skipList.insert(1);
    skipList.insert(10);

    std::cout << "Skip List after insertion:" << std::endl;
    skipList.print();

    std::cout << "Search for 6: " << (skipList.search(6) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search for 5: " << (skipList.search(5) ? "Found" : "Not Found") << std::endl;

    skipList.remove(6);
    skipList.remove(2);

    std::cout << "Skip List after deletion:" << std::endl;
    skipList.print();

    return 0;
}
