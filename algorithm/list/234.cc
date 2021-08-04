/*************************************************************************
	> File Name: 234.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月04日 星期三 08时23分15秒
 ************************************************************************/

#include <iostream>
#include <stack>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 public:
  bool isPalindrome(ListNode* head) {
    if (head == nullptr) {
      return false;
    }

    if (head->next == nullptr) {
      return true;
    }

    std::stack<int> stack;
    ListNode* n1 = head;
    while (n1) {
      stack.push(n1->val);
      n1 = n1->next;
    }

    ListNode* n2 = head;
    while (!stack.empty()) {
      if (stack.top() != n2->val) {
        return false;
      }

      stack.pop();
      n2 = n2->next;
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
  ListNode* node = construct();
  Solution sol;
  std::cout << sol.isPalindrome(node) << std::endl;
  return 0;
}