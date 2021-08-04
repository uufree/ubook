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
  ListNode* middleNode(ListNode* head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    int list_length = 0;
    ListNode* node = head;
    while (node) {
      ++list_length;
      node = node->next;
    }

    int step = list_length / 2;
    ListNode* node1 = head;
    for (int i=0; i<step; i++) {
      node1 = node1->next;
    }

    return node1;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(3);

  n1->next = n2;
  n2->next = n3;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution solution;
  std::cout << solution.middleNode(head)->val << std::endl;
  return 0;
}