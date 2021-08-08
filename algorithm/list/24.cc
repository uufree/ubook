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
  ListNode* swapPairs(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    bool first = true;
    ListNode* pre = new ListNode();
    ListNode* retNode = pre;
    ListNode* cur = head;
    ListNode* next = cur->next;
    while (cur && next) {
      ListNode* nnext = next->next;
      next->next = cur;
      cur->next = nnext;
      pre->next = next;

      pre = cur;
      if (pre && pre->next) {
        cur = pre->next;
      } else {
        break;
      }

      if (cur && cur->next) {
        next = cur->next;
      } else {
        break;
      }
    }

    return retNode->next;
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

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.swapPairs(head);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}