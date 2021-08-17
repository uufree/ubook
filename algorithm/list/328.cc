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
  ListNode *oddEvenList(ListNode *head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    if (head->next->next == nullptr) {
      return head;
    }

    ListNode* sentinal = new ListNode(0);
    ListNode* even = sentinal;

    ListNode* node = head;
    ListNode* prev = nullptr;
    while (node) {
      ListNode* next = node->next;
      if (next == nullptr) {
        break;
      }
      ListNode* next_n = next->next;



      even->next = next;
      even = even->next;
      even->next = nullptr;

      node->next = next_n;
      prev = node;
      node = node->next;
    }

    if (node) {
      node->next = sentinal->next;
    } else {
      prev->next = sentinal->next;
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
  ListNode* n6 = new ListNode(6);
  ListNode* n7 = new ListNode(7);
  ListNode* n8 = new ListNode(8);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = n6;
  n6->next = n7;
  n7->next = n8;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.oddEvenList(head);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}