#include <iostream>
#include <stack>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  int kthToLast(ListNode* head, int k) {
    if(head == nullptr || k<1) {
      return -1;
    }

    std::stack<int> stack;
    ListNode* node = head;
    while (node) {
      stack.push(node->val);
      node = node->next;
    }

    int ret = 0;
    for (int i=0; i<k; i++) {
      if (!stack.empty()) {
        ret = stack.top();
        stack.pop();
      } else {
        return -1;
      }
    }

    return ret;
  }
};

ListNode* construct() {
  ListNode* node1 = new ListNode(4);
  ListNode* node2 = new ListNode(1);
  ListNode* node3 = new ListNode(5);
  ListNode* node4 = new ListNode(9);

  node1->next = node2;
  node2->next = node3;
  node3->next = node4;

  return node1;
}


int main() {
  ListNode* head = construct();
  Solution sol;
  std::cout << sol.kthToLast(head, 4)  << std::endl;
  return 0;
}