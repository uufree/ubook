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
  ListNode *reverseList(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode* prev = head;
    ListNode* cur = head->next;
    prev->next = nullptr;
    while (cur) {
      ListNode* next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }

    return prev;
  }
};

ListNode* construct() {
  ListNode* a1 = new ListNode(1);
  ListNode* a2 = new ListNode(2);
  ListNode* a3 = new ListNode(3);
  ListNode* a4 = new ListNode(4);
  ListNode* a5 = new ListNode(5);

  a1->next = a2;
  a2->next = a3;
  a3->next = a4;
  a4->next = a5;

  return a1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.reverseList(head);

  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}