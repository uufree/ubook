/*************************************************************************
	> File Name: list.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月03日 星期二 07时21分39秒
 ************************************************************************/

#include <iostream>

using namespace std;

struct ListNode {
  int key, value;
  ListNode *prev, *next;
};

class LRU {
 public:
  LRU(int capacity) : head_(new ListNode()), tail_(new ListNode()), capacity_(capacity), size_(0) {
    head_->prev = nullptr;
    head_->next = tail_;
    tail_->prev = head_;
    tail_->next = nullptr;
  }

  ~LRU() {
    ListNode* node = head_->next;
    while (node->next != nullptr) {
      ListNode* temp = node;
      node = node->next;
      delete temp;
    }
    delete head_;
    delete tail_;
    capacity_ = 0;
    size_ = 0;
  }

  int Size() {return size_;}
  int Capacity() {return capacity_;}
  int Get(int key) {
    ListNode* node = head_->next;
    while (node->next != nullptr) {
      if (node->key == key) {
        if (size_ > 1) {
          update(node);
        }
        return node->value;
      }
      node = node->next;
    }

    return -1;
  }

  int Insert(int key, int value) {
    if (Get(key) == -1) {
      if (size_ == capacity_) {
        ListNode* node = tail_->prev;
        node->prev->next = tail_;
        tail_->prev = node->prev;
        delete node;
        --size_;
      }
      ListNode* first = head_->next;
      ListNode* node = new ListNode;
      node->key = key;
      node->value = value;
      node->next = first;
      node->prev = head_;
      first->prev = node;
      head_->next = node;
      ++size_;
    }
    return 0;
  }

  void Print() {
    std::cout << "cap: " << capacity_ << std::endl;
    std::cout << "size: " << size_ << std::endl;
    ListNode* first = head_->next;
    while(first->next != nullptr) {
      std::cout << first->key << "    " << first->value << std::endl;
      first = first->next;
    }
  }

 private:
  void update(ListNode* node) {
    // remove node
    node->prev->next = node->next;
    node->next->prev = node->prev;

    // update node
    ListNode* first = head_->next;
    node->prev = head_;
    node->next = head_->next;
    first->prev = node;
  }

 private:
  ListNode *head_, *tail_;
  int capacity_;
  int size_;
};

int main() {
  LRU lru(2);
  lru.Insert(1,1);
  lru.Insert(2,2);
  lru.Insert(3,3);
  lru.Print();
  return 0;
}
