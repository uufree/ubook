#include <iostream>
#include <set>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  ListNode *detectCycle(ListNode *head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return nullptr;
    }

    std::set<ListNode*> set;
    ListNode* node = head;
    while (node && node->next) {
      if (set.find(node->next) == set.end()) {
        set.insert(node);
      } else {
        return node->next;
      }
      node = node->next;
    }

    return nullptr;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(3);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(0);
  ListNode* n4 = new ListNode(4);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n2;

  return n1;
}

ListNode* construct1() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);

  n1->next = n2;
  n2->next = n1;

  return n1;
}

int main() {
  Solution sol;
  ListNode* node = construct1();
  ListNode* res = sol.detectCycle(node);
  if (res) {
    std::cout << res->val << std::endl;
  }
  return 0;
}