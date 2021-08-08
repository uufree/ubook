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
  ListNode* partition(ListNode* head, int x) {
    if (head == nullptr) {
      return head;
    }

    if (head->next == nullptr) {
      return head;
    }

    ListNode* sentinal = new ListNode();
    sentinal->next = head;

    ListNode* lessListSentinal = new ListNode();
    ListNode* lessNode = lessListSentinal;

    ListNode* node = head;
    ListNode* prev = sentinal;
    while (node) {
      if (node->val < x) {
        ListNode* temp = node;

        prev->next = node->next;
        node = node->next;

        temp->next = nullptr;
        lessNode->next = temp;
        lessNode = lessNode->next;
      } else {
        prev = node;
        node = node->next;
      }
    }
    lessNode->next = sentinal->next;

    return lessListSentinal->next;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(4);
  ListNode* n3 = new ListNode(3);
  ListNode* n4 = new ListNode(2);
  ListNode* n5 = new ListNode(5);
  ListNode* n6 = new ListNode(2);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = n6;

  return n1;
}

ListNode* construct1() {
  ListNode* n1 = new ListNode(2);
  ListNode* n2 = new ListNode(1);

  n1->next = n2;

  return n1;
}

int main() {
  ListNode* head = construct1();

  Solution sol;
  ListNode* node = sol.partition(head, 2);

  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }
  return 0;
}