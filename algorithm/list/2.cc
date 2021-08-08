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
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    if (l1 == nullptr && l2 == nullptr) {
      return nullptr;
    }

    if (l1 == nullptr) {
      return l2;
    }

    if (l2 == nullptr) {
      return l1;
    }

    ListNode *node1 = l1;
    ListNode *node2 = l2;
    int last_flag = 0;
    int val = 0;
    ListNode *sentinel = new ListNode();
    ListNode *newhead = sentinel;
    while (node1 && node2) {
      int sum = node1->val + node2->val + last_flag;
      if (sum >= 10) {
        last_flag = 1;
        val = sum - 10;
      } else {
        last_flag = 0;
        val = sum;
      }
      //std::cout << "sum: " << sum << std::endl;
      //std::cout << "val: " << val << std::endl;
      //std::cout << "flag: " << last_flag << std::endl;

      newhead->next = new ListNode(val);
      newhead = newhead->next;

      node1 = node1->next;
      node2 = node2->next;
    }

    while (node1) {
      int sum = node1->val + last_flag;
      if (sum >= 10) {
        last_flag = 1;
        val = sum - 10;
      } else {
        last_flag = 0;
        val = sum;
      }

      newhead->next = new ListNode(val);
      newhead = newhead->next;
      node1 = node1->next;
    }

    while (node2) {
      int sum = node2->val + last_flag;
      if (sum >= 10) {
        last_flag = 1;
        val = sum - 10;
      } else {
        last_flag = 0;
        val = sum;
      }

      newhead->next = new ListNode(val);
      newhead = newhead->next;
      node2 = node2->next;
    }

    if (last_flag) {
      newhead->next = new ListNode(last_flag);
    }

    return sentinel->next;
  }
};

ListNode *a1, *b1;
void construct() {
  a1 = new ListNode(2);
  ListNode *a2 = new ListNode(4);
  ListNode *a3 = new ListNode(3);

  a1->next = a2;
  a2->next = a3;

  b1 = new ListNode(5);
  ListNode *b2 = new ListNode(6);
  ListNode *b3 = new ListNode(4);
  b1->next = b2;
  b2->next = b3;

  return;
}

void construct1() {
  a1 = new ListNode(0);
  b1 = new ListNode(0);
}

void construct2() {
  a1 = new ListNode(9);
  ListNode* a2 = new ListNode(9);
  ListNode* a3 = new ListNode(9);
  ListNode* a4 = new ListNode(9);
  ListNode* a5 = new ListNode(9);
  ListNode* a6 = new ListNode(9);
  ListNode* a7 = new ListNode(9);

  a1->next = a2;
  a2->next = a3;
  a3->next = a4;
  a4->next = a5;
  a5->next = a6;
  a6->next = a7;

  b1 = new ListNode(9);
  ListNode* b2 = new ListNode(9);
  ListNode* b3 = new ListNode(9);
  ListNode* b4 = new ListNode(9);

  b1->next = b2;
  b2->next = b3;
  b3->next = b4;
}

int main() {
  construct2();

  Solution sol;
  ListNode *node = sol.addTwoNumbers(a1, b1);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}