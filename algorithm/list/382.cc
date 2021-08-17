#include <iostream>
#include <vector>

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
 std::vector<int> vec;
 public:
  Solution(ListNode *head) {
    ListNode* node = head;
    while (node) {
      vec.push_back(node->val);
      node = node->next;
    }

    srand(unsigned(time(0)));
  }

  /** Returns a random node's value. */
  int getRandom() {
    return vec[rand() % vec.size()];
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(2);
  ListNode* n3 = new ListNode(3);
  ListNode* n4 = new ListNode(4);
  ListNode* n5 = new ListNode(5);

  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol(head);
  std::cout << sol.getRandom() << std::endl;
  return 0;
}