#include <iostream>
#include <algorithm>
#include <vector>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  ListNode *sortList(ListNode *head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    std::vector<int> vec;
    ListNode* node = head;
    while (node) {
      vec.push_back(node->val);
      node = node->next;
    }

    std::sort(vec.begin(), vec.end(), std::greater<int>());

    ListNode* node1 = head;
    while (node1) {
      node1->val = vec.back();
      vec.pop_back();
      node1 = node1->next;
    }

    return head;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(4);
  ListNode* n2 = new ListNode(3);
  ListNode* n3 = new ListNode(2);
  ListNode* n4 = new ListNode(1);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.sortList(head);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}
