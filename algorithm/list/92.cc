#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 private:
  ListNode* reverseHead = nullptr;
  ListNode* reverseTail = nullptr;

 public:
  ListNode* reverse(ListNode* head, int step) {
    reverseTail = head;
    ListNode* next = head->next;
    for (int i=0; i<step; i++) {
      ListNode* newNext = next->next;
      next->next = head;
      head = next;
      next = newNext;
    }

    reverseHead = head;
    return next;
  }

  ListNode* reverseBetween(ListNode* head, int left, int right) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    int leftStep = left - 1;
    int rightStep = right - 1;

    ListNode* leftListNode = head;
    ListNode* rightListNode = head;

    ListNode* dummyNode = new ListNode();
    dummyNode->next = head;

    ListNode* cur = head;
    ListNode* prev = dummyNode;
    for (int i=0; i<leftStep; i++) {
      prev = cur;
      cur = cur->next;
    }
    leftListNode = prev;
    leftListNode->next = nullptr;

    rightListNode = reverse(cur, right - left);

    leftListNode->next = reverseHead;
    reverseTail->next = rightListNode;

    return dummyNode->next;
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
  ListNode* node = new ListNode(5);

  return node;
}

ListNode* construct2() {
  ListNode* n1 = new ListNode(2);
  ListNode* n2 = new ListNode(1);

  n1->next = n2;

  return n1;
}

int main() {
  ListNode* head = construct2();
  Solution sol;
  ListNode* node = sol.reverseBetween(head, 1, 2);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}