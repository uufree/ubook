#include <iostream>
#include <deque>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  void reorderList(ListNode *head) {
    if (head == nullptr) {
      return;
    }

    if (head->next == nullptr) {
      return;
    }

    std::deque<ListNode*> deque;
    ListNode* node = head;
    while (node) {
      deque.push_back(node);
      node = node->next;
    }

    ListNode* sentinal = new ListNode(0);
    ListNode* node1 = sentinal;
    while (!deque.empty()) {
      node1->next = deque.front();
      deque.pop_front();
      node1 = node1->next;
      node1->next = nullptr;

      if (!deque.empty()) {
        node1->next = deque.back();
        deque.pop_back();
        node1 = node1->next;
        node1->next = nullptr;
      } else {
        break;
      }
    }

    head = sentinal->next;
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
  ListNode* node = construct();
  Solution sol;
  sol.reorderList(node);

  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}