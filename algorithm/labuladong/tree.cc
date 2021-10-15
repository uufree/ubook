#include <iostream>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class SolutionoOffer27 {
 public:
  TreeNode *mirrorTree(TreeNode *root) {
    if (root == nullptr) {
      return nullptr;
    }

    TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    mirrorTree(root->left);
    mirrorTree(root->right);

    return root;
  }
};

class Node {
 public:
  int val;
  Node *left;
  Node *right;
  Node *next;

  Node() : val(0), left(NULL), right(NULL), next(NULL) {}
  Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}
  Node(int _val, Node *_left, Node *_right, Node *_next)
      : val(_val), left(_left), right(_right), next(_next) {}
};

class Solution116 {
 public:
  void connectTowNode(Node* left, Node *right) {
    if (!left || !right) {
      return;
    }

    left->next = right;
    connectTowNode(left->left, left->right);
    connectTowNode(right->left, right->right);
    connectTowNode(left->right, right->left);
  }

  Node *connect(Node *root) {
    if (root == nullptr) {
      return root;
    }
    connectTowNode(root->left, root->right);
    return root;
  }
};

class Solution114 {
 public:
  void flatten(TreeNode* root) {
    if (root == nullptr) {
      return;
    }

    flatten(root->left);
    flatten(root->right);

    root->right = root->left;
    root->left = nullptr;
  }
};