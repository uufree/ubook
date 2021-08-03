/*************************************************************************
	> File Name: 21.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月03日 星期二 08时04分26秒
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
  ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
    if (l1 == nullptr && l2 == nullptr) {return nullptr;}
    if (l1 == nullptr) {return l2;}
    if (l2 == nullptr) {return l1;}

    ListNode* head = nullptr;
    if (l1->val > l2->val) {
      head = l2;
      l2 = l2->next;
      head->next = nullptr;
    } else {
      head = l1;
      l1 = l1->next;
      head->next = nullptr;
    }
    ListNode* node = head;

    while (l1 != nullptr && l2 != nullptr) {
      if (l1->val > l2->val) {
        head->next = l2;
        l2 = l2->next;
      } else {
        head->next = l1;
        l1 = l1->next;
      }
      head = head->next;
    }

    if (l1 == nullptr && l2 != nullptr) {
      head->next = l2;
    }

    if (l2 == nullptr && l1 != nullptr) {
      head->next = l1;
    }
    return node;
  }
};

ListNode* constructL1() {
  ListNode* l1 = new ListNode(1);
  ListNode* head = l1;
  head->next = new ListNode(2);
  head = head->next;
  head->next = new ListNode(4);
  return l1;
}

ListNode* constructL2() {
  ListNode* l1 = new ListNode(1);
  ListNode* head = l1;
  head->next = new ListNode(3);
  head = head->next;
  head->next = new ListNode(4);
  return l1;
}

int main() {
  ListNode* l1 = constructL1();
  ListNode* l2 = constructL2();

  Solution sol;
  auto* head = sol.mergeTwoLists(l1, l2);
  while (head != nullptr) {
    std::cout << head->val << std::endl;
    head = head->next;
  }

  return 0;
}