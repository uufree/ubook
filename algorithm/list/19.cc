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
  ListNode *removeNthFromEnd(ListNode *head, int n) {
    if (head == nullptr) {
      return nullptr;
    }

    ListNode* node = head;
    int list_length = 0;
    while (node) {
      ++list_length;
      node = node->next;
    }

    int index = list_length - n;
    int step = index - 1;
    if (step < 0) {
      return head->next;
    }

    ListNode* node1 = head;
    for (int i=0; i<step; i++) {
      node1 = node1->next;
    }

    if (node1->next->next) {
      node1->next = node1->next->next;
    } else {
      node1->next = nullptr;
    }

    return head;
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

ListNode* construct1() {
  ListNode* n1 = new ListNode(1);
  return n1;
}

ListNode* construct2() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);

  n1->next = n2;

  return n1;
}

int main() {
  ListNode* head = construct2();
  Solution sol;
  ListNode* node = sol.removeNthFromEnd(head, 1);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}