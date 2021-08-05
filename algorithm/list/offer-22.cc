#include <iostream>
#include <stack>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  ListNode* getKthFromEnd(ListNode* head, int k) {
    if (head == nullptr) {
      return nullptr;
    }

    if (k < 1) {
      return nullptr;
    }

    ListNode* node = head;
    std::stack<ListNode*> stack;
    while (node) {
      stack.push(node);
      node = node->next;
    }

    ListNode* ret = nullptr;
    for (int i=0; i<k; i++) {
      if (!stack.empty()) {
        ret = stack.top();
        stack.pop();
      } else {
        return nullptr;
      }
    }

    return ret;
  }
};


ListNode* construct() {
  ListNode* n1 = new ListNode(1);


  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.getKthFromEnd(head, 1);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}