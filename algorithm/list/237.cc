/*************************************************************************
	> File Name: 237.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月04日 星期三 08时32分29秒
 ************************************************************************/

#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  void deleteNode(ListNode* node) {
    ListNode* prev = nullptr;
    while (node->next) {
      node->val = node->next->val;
      prev = node;
      node = node->next;
    }
    prev->next = nullptr;
  }
};

ListNode* n1 = nullptr;
ListNode* construct() {
  n1 = new ListNode(4);
  ListNode* n2 = new ListNode(5);
  ListNode* n3 = new ListNode(1);
  ListNode* n4 = new ListNode(9);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;

  return n3;
}

int main() {
  ListNode* node = construct();
  Solution sol;
  sol.deleteNode(node);

  while (n1 != nullptr) {
    std::cout << n1->val << std::endl;
    n1 = n1->next;
  }

  return 0;
}