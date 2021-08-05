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
    ListNode* nodeA = headA;
    ListNode* nodeB = headB;
    while (nodeA) {
      stackA.push(nodeA);
      nodeA = nodeA->next;
    }

    while (nodeB) {
      stackA.push(nodeB);
      nodeB = nodeB->next;
    }

    while (!stackA.empty() && !stackB.empty()) {
      if (stackA.top() == stackB.top()) {
        stackA.pop();
        stackB.pop();
      } else {
        break;
      }
    }

    ListNode* retNode = nullptr;
    if (!stackA.empty()) {
      retNode = stackA.top()->next;
    } else if (!stackB.empty()) {
      retNode = stackB.top()->next;
    } else {
      retNode = nullptr;
    }

    return retNode;
  }
};