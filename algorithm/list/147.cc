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
  ListNode *insertionSortList(ListNode *head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    ListNode* newHead = head;
    ListNode* oldHead = head->next;
    newHead->next = nullptr;

    while (oldHead) {
      ListNode* insertNode = oldHead;
      oldHead = oldHead->next;
      insertNode->next = nullptr;

      ListNode* node = newHead;
      if (node->val >= insertNode->val) {
        insertNode->next = newHead;
        newHead = insertNode;
      } else {
        bool insert = false;
        while (node->next) {
          if (node->next->val >= insertNode->val) {
            insert = true;
            ListNode* next = node->next;
            node->next = insertNode;
            insertNode->next = next;
            break;
          }
          node = node->next;
        }

        if (!insert) {
          node->next = insertNode;
        }
      }
    }

    return newHead;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(4);
  ListNode* n2 = new ListNode(2);

  n1->next = n2;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.insertionSortList(head);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}