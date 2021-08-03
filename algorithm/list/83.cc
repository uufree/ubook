/*************************************************************************
	> File Name: 83.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月03日 星期二 08时27分29秒
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
  ListNode* deleteDuplicates(ListNode* head) {
    if (head == nullptr) {
      return nullptr;
    }

    ListNode* node = head;
    while (node->next) {
      if (node->val == node->next->val) {
        node->next = node->next->next;
      } else {
        node = node->next;
      }
    }

    return head;
  }
};

ListNode* constructL1() {
  ListNode* l1 = new ListNode(1);
  ListNode* head = l1;
  head->next = new ListNode(1);
  head = head->next;
  head->next = new ListNode(2);
  head = head->next;
  return l1;
}

int main() {
  ListNode* l1 = constructL1();
  Solution sol;
  ListNode* l2 = sol.deleteDuplicates(l1);
  while(l2 != nullptr) {
    std::cout << l2->val << std::endl;
    l2 = l2->next;
  }
  return 0;
}