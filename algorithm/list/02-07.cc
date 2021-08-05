/*************************************************************************
	> File Name: 02-07.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 07时48分58秒
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
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (headA == nullptr && headB == nullptr) {
      return nullptr;
    }

    if (headA == nullptr) {
      return nullptr;
    }

    if (headB == nullptr) {
      return nullptr;
    }

    if (headA == headB) {
      return headA;
    }


    std::stack<ListNode*> stackA, stackB;
    ListNode* node1 = headA;
    while (node1) {
      stackA.push(node1);
      node1 = node1->next;
    }

    ListNode* node2 = headB;
    while (node2) {
      stackB.push(node2);
      node2 = node2->next;
    }

    while (!stackA.empty() && !stackB.empty()) {
      if (stackA.top() == stackB.top()) {
        stackA.pop();
        stackB.pop();
      } else {
        if (!stackA.empty()) {
          return stackA.top()->next;
        }

        if (!stackB.empty()) {
          return stackB.top()->next;
        }
      }
    }

    ListNode* ret = nullptr;
    if (!stackA.empty()) {
      ret = stackA.top()->next;
    } else if (!stackB.empty()) {
      ret = stackB.top()->next;
    } else {
      ret = nullptr;
    }

    return ret;
  }
};

ListNode* a1, *b1;
int construct() {
  a1 = new ListNode(4);
  ListNode* a2 = new ListNode(1);
  b1 = new ListNode(5);
  ListNode* b2 = new ListNode(0);
  ListNode* b3 = new ListNode(1);
  ListNode* c1 = new ListNode(8);
  ListNode* c2 = new ListNode(4);
  ListNode* c3 = new ListNode(5);

  // la
  a1->next = a2;
  a2->next = c1;

  b1->next = b2;
  b2->next = b3;
  b3->next = c1;

  c1->next = c2;
  c2->next = c3;

  return 0;
}

int construct1() {
  a1 = new ListNode(1);
  ListNode* a2 = new ListNode(9);
  ListNode* a3 = new ListNode(1);

  b1 = new ListNode(3);

  ListNode* c1 = new ListNode(2);
  ListNode* c2 = new ListNode(4);

  // la
  a1->next = a2;
  a2->next = a3;
  a3->next = c1;

  b1->next = c1;

  c1->next = c2;

  return 0;
}

int construct2() {
  a1 = new ListNode(3);

  b1 = new ListNode(2);

  // la
  b1->next = a1;

  return 0;
}

int construct3() {
  a1 = new ListNode(1);
  b1 = new ListNode(2);
  return 0;
}

int construct4() {
  a1 = new ListNode(1);
  b1 = new ListNode(1);

  a1->next - b1;

  return 0;
}

int main() {
  construct4();

  Solution sol;
  ListNode* node = sol.getIntersectionNode(a1, b1);
  if (node) {
    std::cout << node->val << std::endl;
  }

  return 0;
}



