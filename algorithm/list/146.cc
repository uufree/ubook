#include <iostream>
#include <list>
#include <map>

// todo: 裸写双向链表，用map保存ListNode*做快速查询
class LRUCache {
 public:
  LRUCache(int capacity) {
    capacity_ = capacity;
  }

  int get(int key) {
    for (auto iter=cache_.begin(); iter!=cache_.end(); ++iter) {
      if (iter->first == key) {
        int res = iter->second;
        cache_.erase(iter);
        cache_.push_front(std::pair<int, int>(key, res));
        return res;
      }
    }

    return -1;
  }

  void put(int key, int value) {
    if (get(key) != -1) {
      cache_.front().second = value;
    } else {
      if (cache_.size() == capacity_) {
        cache_.pop_back();
      }
      cache_.push_front(std::pair<int, int>(key, value));
    }

  }

  void print() {
    std::cout << "######" << std::endl;
    for (auto iter=cache_.begin(); iter!=cache_.end(); ++iter) {
      std::cout << iter->first << "  " << iter->second << std::endl;
    }
    std::cout << "######" << std::endl;
  }

 private:
  std::list<std::pair<int, int>> cache_;
  int capacity_;
};

int main() {
  LRUCache cache(2);
  cache.put(1,1);
  //cache.print();
  cache.put(2,2);
  std::cout << cache.get(1) << std::endl;
  cache.print();
  cache.put(3,3);
  cache.print();
  std::cout << cache.get(2) << std::endl;
  //cache.print();
  cache.put(4,4);
  std::cout << cache.get(1) << std::endl;
  std::cout << cache.get(3) << std::endl;
  std::cout << cache.get(4) << std::endl;

  return 0;
}

//int main() {
//  LRUCache cache(2);
//  cache.put(2,1);
//  //cache.print();
//  cache.put(2,2);
//  //cache.print();
//  std::cout << cache.get(2) << std::endl;
//  cache.put(1,1);
//  //cache.print();
//  cache.put(4,1);
//  std::cout << cache.get(2) << std::endl;
//
//  return 0;
//}