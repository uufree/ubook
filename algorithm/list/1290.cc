#include <iostream>
#include <bitset>
#include <cmath>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  int getDecimalValue(ListNode* head) {
    if (head == nullptr) {
      return -1;
    }

    ListNode* node = head;
    int list_length = 0;
    while (node) {
      ++list_length;
      node = node->next;
    }

    ListNode* node1 = head;
    int result = 0;
    while (node1) {
      result += node1->val * pow(2, --list_length);
      node1 = node1->next;
    }

    return result;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(0);
  ListNode* n2 = new ListNode(1);
  ListNode* n3 = new ListNode(1);
  ListNode* n4 = new ListNode(1);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  std::cout << sol.getDecimalValue(head) << std::endl;
  return 0;
}