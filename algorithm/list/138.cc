#include <iostream>
#include <unordered_map>

class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

class Solution {
 public:
  std::unordered_map<Node*, Node*> hash;
  Node* copyRandomList(Node* head) {
    if (head == nullptr) {
      return nullptr;
    }

    if (hash.count(head) == 0) {
      Node* newNode = new Node(head->val);
      hash[head] = newNode;
      hash[head]->next = copyRandomList(head->next);
      hash[head]->random = copyRandomList(head->random);
    }

    return hash[head];
  }
};
