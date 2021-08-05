/*************************************************************************
	> File Name: 02-01.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年08月05日 星期四 07时14分37秒
 ************************************************************************/

#include <iostream>
#include <map>

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
  ListNode* removeDuplicateNodes(ListNode* head) {
    while (head && head->next && head->val == head->next->val) {
      head = head->next;
    }

    if (head == nullptr) {
      return nullptr;
    }

    if (head->next == nullptr) {
      return head;
    }

    std::map<int, int> times;
    std::map<int, bool> del;

    ListNode* node1 = head;
    while (node1) {
      ++times[node1->val];
      node1 = node1->next;
    }

    for (const auto& item : times) {
      if (item.second > 1) {
        del[item.first] = false;
      }
    }

    ListNode* prev = head;
    ListNode* cur = head->next;

    if (del.find(prev->val) != del.end()) {
      del[prev->val] = true;
    }

    while(cur) {
      if (del.find(cur->val) == del.end()) {
        prev = cur;
        cur = cur->next;
      } else {
        if (del[cur->val]) {
          prev->next = cur->next;
          cur = cur->next;
        } else {
          del[cur->val] = true;
          prev = cur;
          cur = cur->next;
        }
      }
    }

    return head;
  }
};

ListNode* construct() {
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(1);

  n1->next = n2;

  return n1;
}

int main() {
  ListNode* head = construct();
  Solution sol;
  ListNode* node = sol.removeDuplicateNodes(head);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}