/*************************************************************************
	> File Name: offer-06.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 08时22分33秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  vector<int> reversePrint(ListNode *head) {
    std::vector<int> result;
    ListNode* node = head;
    while (node) {
      result.push_back(node->val);
      node = node->next;
    }

    std::reverse(result.begin(), result.end());
    return result;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(3);
  ListNode* n3 = new ListNode(2);

  n1->next = n2;
  n2->next = n3;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  auto result = sol.reversePrint(head);
  for (auto res : result) {
    std::cout << res << std::endl;
  }

  return 0;
}