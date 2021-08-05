/*************************************************************************
	> File Name: offer-25.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 08时27分50秒
 ************************************************************************/

#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr && l2 == nullptr) {
      return nullptr;
    }

    if (l1 == nullptr) {
      return l2;
    }

    if (l2 == nullptr) {
      return l1;
    }

    ListNode* node = nullptr;
    ListNode* node1 = l1;
    ListNode* node2 = l2;

    if (node1->val < node2->val) {
      node = node1;
      node1 = node1->next;
    } else {
      node = node2;
      node2 = node2->next;
    }

    ListNode* ret = node;
    while (node1 && node2) {
      if (node1->val < node2->val) {
        node->next = node1;
        node = node->next;
        node1 = node1->next;
      } else {
        node->next = node2;
        node = node->next;
        node2 = node2->next;
      }
    }

    if (node1) {
      node->next = node1;
    }

    if (node2) {
      node->next = node2;
    }

    return ret;
  }
};

ListNode* a, *b;
int construct() {
  a = new ListNode(1);
  ListNode* a1 = new ListNode(2);
  ListNode* a2 = new ListNode(4);

  b = new ListNode(1);
  ListNode* b1 = new ListNode(3);
  ListNode* b2 = new ListNode(4);

  a->next = a1;
  a1->next = a2;

  b->next = b1;
  b1->next = b2;

  return 0;
}

int main() {
  construct();
  Solution sol;
  ListNode* node = sol.mergeTwoLists(a, b);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }
  return 0;
}