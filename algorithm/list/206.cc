/*************************************************************************
	> File Name: 206.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月04日 星期三 08时07分50秒
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
  ListNode* reverseList(ListNode* head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    ListNode* node = head->next;
    ListNode* prev = head;
    prev->next = nullptr;
    while (node != nullptr) {
      ListNode* next = node->next;
      node->next = prev;
      prev = node;
      node = next;
    }

    return prev;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(3);
  ListNode* n4 = new ListNode(4);
  ListNode* n5 = new ListNode(5);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;

  return n1;
}

ListNode* construct1() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);

  n1->next = n2;
  return n1;
}

int main() {
  ListNode* node = construct1();
//  while (node != nullptr) {
//    std::cout << node->val << std::endl;
//    node = node->next;
//  }

  Solution sol;
  ListNode* head = sol.reverseList(node);
  while (head != nullptr) {
    std::cout << head->val << std::endl;
    head = head->next;
  }

  return 0;
}