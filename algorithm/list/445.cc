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
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    if (l1 == nullptr) {
      return l2;
    }

    if (l2 == nullptr) {
      return l1;
    }

    std::stack<int> stack1, stack2;
    ListNode* node1 = l1;
    ListNode* node2 = l2;
    while (node1) {
      stack1.push(node1->val);
      node1 = node1->next;
    }

    while (node2) {
      stack2.push(node2->val);
      node2 = node2->next;
    }

    ListNode* head = nullptr;
    int flag = 0;
    while (!stack1.empty() && !stack2.empty()) {
      int sum = stack1.top() + stack2.top() + flag;
      if (sum >= 10) {
        sum = sum - 10;
        flag = 1;
      } else {
        sum = sum;
        flag = 0;
      }

      ListNode* newNode = new ListNode(sum);
      newNode->next = head;
      head = newNode;

      stack1.pop();
      stack2.pop();
    }

    while (!stack1.empty()) {
      int sum = stack1.top() + flag;
      if (sum >= 10) {
        sum = sum - 10;
        flag = 1;
      } else {
        sum = sum;
        flag = 0;
      }

      ListNode* newNode = new ListNode(sum);
      newNode->next = head;
      head = newNode;

      stack1.pop();
    }

    while (!stack2.empty()) {
      int sum = stack2.top() + flag;
      if (sum >= 10) {
        sum = sum - 10;
        flag = 1;
      } else {
        sum = sum;
        flag = 0;
      }

      ListNode* newNode = new ListNode(sum);
      newNode->next = head;
      head = newNode;

      stack2.pop();
    }

    if (flag) {
      ListNode* newNode = new ListNode(flag);
      newNode->next = head;
      head = newNode;
    }

    return head;
  }
};

ListNode* l1 = nullptr;
ListNode* l2 = nullptr;

void construct() {
  l1 = new ListNode(7);
  ListNode* n1 = new ListNode(2);
  ListNode* n2 = new ListNode(4);
  ListNode* n3 = new ListNode(3);
  l1->next = n1;
  n1->next = n2;
  n2->next = n3;

  l2 = new ListNode(5);
  ListNode* o1 = new ListNode(6);
  ListNode* o2 = new ListNode(4);
  l2->next = o1;
  o1->next = o2;
}

void construct1() {
  l1 = new ListNode(9);
  ListNode* n1 = new ListNode(1);
  ListNode* n2 = new ListNode(6);
  l1->next = n1;
  n1->next = n2;

  l2 = new ListNode(0);
}

int main() {
  construct1();
  Solution sol;
  ListNode* node = sol.addTwoNumbers(l1, l2);
  while (node) {
    std::cout << node->val << std::endl;
    node = node->next;
  }

  return 0;
}