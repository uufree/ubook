#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  ListNode* deleteNode(ListNode* head, int val) {
    if (head == nullptr) {
      return head;
    }

    while (head->val == val) {
      head = head->next;
    }

    if (head->next == nullptr) {
      return head;
    }

    ListNode* prev = head;
    ListNode* cur = head->next;
    while (cur) {
      if (cur->val == val) {
        prev->next = cur->next;
        cur = cur->next;
      } else {
        prev = cur;
        cur = cur->next;
      }
    }

    return head;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(4);
  ListNode* n2 = new ListNode(5);
  ListNode* n3 = new ListNode(1);
  ListNode* n4 = new ListNode(9);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.deleteNode(head, 9);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}