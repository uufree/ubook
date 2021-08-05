/*************************************************************************
	> File Name: 02-06.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 07时38分38秒
 ************************************************************************/

#include <iostream>
#include <stack>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  bool isPalindrome(ListNode* head) {
    if (head == nullptr) {
      return true;
    }

    if (head->next == nullptr) {
      return true;
    }

    ListNode* node = head;
    int list_length = 0;
    while (node) {
      ++list_length;
      node = node->next;
    }

    std::stack<int> half;
    int stack_size = list_length / 2;
    ListNode* node1 = head;
    while (half.size() != stack_size) {
      half.push(node1->val);
      node1 = node1->next;
    }

    if (list_length % 2) {
      node1 = node1->next;
    }

    while (node1 && !half.empty()) {
      if (node1->val != half.top()) {
        return false;
      }
      node1 = node1->next;
      half.pop();
    }

    return true;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
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
  std::cout << sol.isPalindrome(head) << std::endl;
  return 0;
}
