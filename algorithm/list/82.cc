#include <iostream>
#include <map>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  ListNode* deleteDuplicates(ListNode* head) {
    if (head == nullptr) {
      return head;
    }

    if (head->next == nullptr) {
      return head;
    }

    std::map<int, int> repeat;
    ListNode* node = head;
    while (node) {
      ++repeat[node->val];
      node = node->next;
    }

    ListNode* sentinal = new ListNode();
    sentinal->next = head;
    ListNode* prev = sentinal;
    ListNode* node1 = head;
    while (node1) {
      if (repeat[node1->val] > 1) {
        prev->next = node1->next;
      } else {
        prev = node1;
      }
      node1 = node1->next;
    }

    return sentinal->next;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(3);
  ListNode* n4 = new ListNode(3);
  ListNode* n5 = new ListNode(4);
  ListNode* n6 = new ListNode(4);
  ListNode* n7 = new ListNode(5);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = n6;
  n6->next = n7;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.deleteDuplicates(head);

  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}