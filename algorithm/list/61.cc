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
  ListNode* rotateRight(ListNode* head, int k) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    std::deque<int> queue;
    ListNode* node = head;
    while (node) {
      queue.push_back(node->val);
      node = node->next;
    }

    int rotate = k % queue.size();
    for (int i=0; i<rotate; i++) {
      queue.push_front(queue.back());
      queue.pop_back();
    }

    ListNode* node1 = head;
    while (node1 && !queue.empty()) {
      node1->val = queue.front();
      queue.pop_front();
      node1 = node1->next;
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

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.rotateRight(head, 2);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}