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
    if (headA == nullptr || headB == nullptr) {
      return nullptr;
    }

    if (headA == headB) {
      return headA;
    }

    std::stack<ListNode*> stackA, stackB;
    while (headA != nullptr) {
      stackA.push(headA);
      headA = headA->next;
    }

    while (headB != nullptr) {
      stackB.push(headB);
      headB = headB->next;
    }

    std::stack<ListNode*>* baseline_stack, *compare_stack;
    if (stackA.size() < stackB.size()) {
      baseline_stack = &stackA;
      compare_stack = &stackB;
    } else {
      baseline_stack = &stackB;
      compare_stack = &stackA;
    }

    while (!baseline_stack->empty()) {
      if (baseline_stack->top() != compare_stack->top()) {
        return baseline_stack->top()->next;
      }
      baseline_stack->pop();
      compare_stack->pop();
    }

    return compare_stack->top()->next;
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

int main() {
  construct2();

  Solution sol;
  ListNode* node = sol.getIntersectionNode(a1, b1);
  std::cout << node->val << std::endl;

  return 0;
}