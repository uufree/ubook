/*************************************************************************
	> File Name: 203.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月04日 星期三 07时53分06秒
 ************************************************************************/

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
  ListNode *removeElements(ListNode *head, int val) {
    while (head != nullptr && head->val == val) {
      head = head->next;
    }

    if (head == nullptr) {
      return nullptr;
    }

    if (head->val == val && head->next == nullptr) {
      return nullptr;
    }

    ListNode* node = head->next;
    ListNode* last = head;
    while (node != nullptr) {
      if (node->val == val) {
        last->next = node->next;
        node = node->next;
      } else {
        last = node;
        node = node->next;
      }
    }

    return head;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(6);
  ListNode* n4 = new ListNode(3);
  ListNode* n5 = new ListNode(4);
  ListNode* n6 = new ListNode(5);
  ListNode* n7 = new ListNode(6);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = n6;
  n6->next = n7;
  return n1;
}

ListNode* construct1() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(1);
  ListNode* n3 = new ListNode(1);
  ListNode* n4 = new ListNode(1);
  ListNode* n5 = new ListNode(1);
  ListNode* n6 = new ListNode(1);
  ListNode* n7 = new ListNode(1);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = n6;
  n6->next = n7;
  return n1;
}

int main() {
  ListNode* node = construct1();
//  while (node) {
//    std::cout << node->val << std::endl;
//    node = node->next;
//  }

  Solution sol;
  ListNode* head = sol.removeElements(node, 1);
  while (head) {
    std::cout << head->val << std::endl;
    head = head->next;
  }

  return 0;
}