#include "SkipList.hpp"
#include <iostream>
using namespace std;

void* operator new(std::size_t size) {
    void* ptr = std::malloc(size);
    std::cout << "Allocated memory: " << size << " bytes at address: " << ptr
    << std::endl; return ptr;
}

// 重载全局的 delete 操作符
void operator delete(void* ptr, std::size_t size) noexcept {
    std::cout << "Deallocating memory: " << size << " bytes at address: " <<
    ptr << std::endl; std::free(ptr);
}
class Integer {
  public:
    Integer() : val(INT32_MIN) {}
    Integer(int val) : val(val) {}
    Integer(const Integer &_int) : val(_int.val) { cout << "赋值构造" << endl; }
    Integer(Integer &&_int) : val(_int.val) {
        _int.val = INT32_MIN;
        cout << "移动构造" << endl;
    }
    int val;
    bool operator<(const Integer &right) { return val < right.val; }
    bool operator==(const Integer &right) { return val == right.val; }
    bool operator!=(const Integer &right) { return val != right.val; }
    friend ostream& operator<< (ostream& out,const Integer& i){
        out<<i.val;
        return out;
    }
};

int main() {
    SkipList<Integer> sl;
    for (int i = 60; i >= 0; --i) {
        sl.add(i);
        cout<<"-------"<<endl;
    }
    sl.print();
    return 0;
}
