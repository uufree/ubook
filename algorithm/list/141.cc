#include <iostream>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  bool hasCycle(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
      return false;
    }

    ListNode* slow = head;
    ListNode* quick = head;
    while (slow != nullptr && quick != nullptr) {
      if (!slow->next) {
        break;
      }
      slow = slow->next;

      if (!quick->next || !quick->next->next) {
        break;
      }
      quick = quick->next->next;

      if (slow == quick) {
        return true;
      }
    }

    return false;
  }
};

// [3,2,0,4] & 1
ListNode* constructL1() {
  ListNode* head = new ListNode(3);
  ListNode* node = head;

  ListNode* temp = new ListNode(2);
  node->next = temp;
  node = node->next;
  node->next = new ListNode(0);
  node = node->next;
  node->next = new ListNode(4);
  node = node->next;
  node->next = temp;

  return head;
}

// [1,2] & 0
ListNode* constructL2() {
  ListNode* node = new ListNode(1);
  ListNode* node1 = new ListNode(2);
  node->next = node1;
  node1->next = node;
  return node;
}

ListNode* constructL3() {
  ListNode* node = new ListNode(1);
  node->next = node;
  return node;
}

int main() {
  ListNode* node = constructL3();
  Solution sol;
  std::cout << sol.hasCycle(node) << std::endl;
  return 0;
}