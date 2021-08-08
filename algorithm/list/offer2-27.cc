#include <iostream>
#include <stack>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  bool isPalindrome(ListNode *head) {
    if (head == nullptr) {
      return true;
    }

    int list_length = 0;
    ListNode* node = head;
    while (node) {
      ++list_length;
      node = node->next;
    }

    std::stack<int> stack;
    ListNode* node1 = head;
    for (int i=0; i<list_length/2; i++) {
      stack.push(node1->val);
      node1 = node1->next;
    }

    if (list_length % 2) {
      node1 = node1->next;
    }

    while (node1 && !stack.empty()) {
      if (node1->val == stack.top()) {
        node1 = node1->next;
        stack.pop();
      } else {
        return false;
      }
    }

    return true;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(0);
  ListNode* n3 = new ListNode(1);

  n1->next = n2;
  n2->next = n3;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  std::cout << sol.isPalindrome(head) << std::endl;
  return 0;
}
