/*************************************************************************
	> File Name: 02-03.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 08时16分19秒
 ************************************************************************/

#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  void deleteNode(ListNode* node) {
    ListNode* prev = nullptr;
    while (node && node->next) {
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

  return n2;
}

int main() {
  ListNode* node = construct();
  Solution sol;
  sol.deleteNode(node);

  while (n1) {
    std::cout << n1->val << std::endl;
    n1 = n1->next;
  }

  return 0;
}
