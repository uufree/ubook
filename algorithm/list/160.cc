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

    ListNode* last = nullptr;
    while (!baseline_stack->empty()) {
      if (baseline_stack->top()->val != compare_stack->top()->val) {
        break;
      }
      last = baseline_stack->top();
      baseline_stack->pop();
      compare_stack->pop();
    }

    return nullptr;
  }
};