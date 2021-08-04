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
  ListNode *removeElements(ListNode *head, int val) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr && head->val == val) {
      return nullptr;
    }

    ListNode* node = head->next;
    ListNode* last = head;
    while (node != nullptr) {
      if (node->val == val) {

      }
    }
  }
};