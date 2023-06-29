#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
class Integer {
  public:
    Integer(int val) : val(val) {}
    Integer(const Integer &_int) : val(_int.val) { cout << "赋值构造" << endl; }
    Integer(Integer &&_int) : val(_int.val) {
        _int.val = -1;
        cout << "移动构造" << endl;
    }
    int val;
    bool operator<(const Integer &right) { return val < right.val; }
    bool operator==(const Integer &right) { return val == right.val; }
    bool operator!=(const Integer &right) { return val != right.val; }
};

template <typename K, typename = void> struct ListNode {};

template <typename K>
struct ListNode<
    K, typename std::enable_if<
           std::is_same<decltype(std::declval<K>() < std::declval<K>()),
                        bool>::value &&
           std::is_same<decltype(std::declval<K>() == std::declval<K>()),
                        bool>::value &&
           std::is_same<decltype(std::declval<K>() != std::declval<K>()),
                        bool>::value>::type> {
    K val;
    vector<ListNode *> nextList;
    ListNode(K &&k, int level)
        : val(std::forward<K>(k)), nextList(level, nullptr) {}
};
void test(Integer&& _int){
    ListNode<Integer> aa(std::forward<Integer>(_int), 1); 
}
int main() { 
    Integer one(1);
    test(one);
}