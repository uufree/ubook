/*************************************************************************
	> File Name: patch.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年10月29日 星期五 07时37分58秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution107 {
 public:
  vector<vector<int>> levelOrderBottom(TreeNode *root) {
    vector<vector<int>> result;
    vector<vector<TreeNode *>> nodes;
    if (root == nullptr) {
      return result;
    }

    nodes.push_back({root});
    while (!nodes.back().empty()) {
      vector<int> temp;
      vector<TreeNode *> tempNodes;
      for (auto node : nodes.back()) {
        temp.push_back(node->val);
        if (node->left) {
          tempNodes.push_back(node->left);
        }

        if (node->right) {
          tempNodes.push_back(node->right);
        }
      }
      result.push_back(temp);
      nodes.push_back(tempNodes);
    }

    reverse(result.begin(), result.end());
    return result;
  }
};

class Solution108 {
 public:
  TreeNode *construct() {
    TreeNode *n1 = new TreeNode(-10);
    TreeNode *n2 = new TreeNode(-3);
    TreeNode *n3 = new TreeNode(0);
    TreeNode *n4 = new TreeNode(5);
    TreeNode *n5 = new TreeNode(9);

    n3->left = n2;
    n3->right = n5;
    n2->left = n1;
    n5->left = n4;

    return n3;
  }

  void print(TreeNode *root) {
    if (root == nullptr) {
      return;
    }

    print(root->left);
    std::cout << root->val << std::endl;
    print(root->right);
  }

  TreeNode *build(vector<int> &nums, int left, int right) {
    if (left > right) {
      return nullptr;
    }

    int mid = (left + right) / 2;
    TreeNode *root = new TreeNode(nums[mid]);
    root->left = build(nums, left, mid - 1);
    root->right = build(nums, mid + 1, right);
    return root;
  }

  TreeNode *sortedArrayToBST(vector<int> &nums) {
    if (nums.empty()) {
      return nullptr;
    }

    return build(nums, 0, nums.size() - 1);
  }
};

class Solution109 {
 public:
  TreeNode *build(vector<int> &nums, int left, int right) {
    if (left > right) {
      return nullptr;
    }

    int mid = (left + right) / 2;
    TreeNode *root = new TreeNode(nums[mid]);
    root->left = build(nums, left, mid - 1);
    root->right = build(nums, mid + 1, right);
    return root;
  }

  TreeNode *sortedListToBST(ListNode *head) {
    std::vector<int> vec;
    while (head) {
      vec.push_back(head->val);
      head = head->next;
    }

    if (vec.empty()) {
      return nullptr;
    }

    return build(vec, 0, vec.size() - 1);
  }
};

class Solution110 {
 public:
  int height(TreeNode *root) {
    if (root == nullptr) {
      return 0;
    }

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    if (leftHeight == -1 || rightHeight == -1) {
      return -1;
    }

    if (abs(leftHeight - rightHeight) > 1) {
      return -1;
    } else {
      return 1 + max(leftHeight, rightHeight);
    }
  }

  bool isBalanced(TreeNode *root) {
    if (root == nullptr) {
      return true;
    }

    return height(root) == -1 ? false : true;
  }
};

class Solution111 {
 public:
  int minDepth(TreeNode *root) {
    if (root == nullptr) {
      return 0;
    }

    if (!root->left && !root->right) {
      return 1;
    }

    int min_depth = -1;
    if (root->left) {
      min_depth = minDepth(root->left);
    } else {
      min_depth = minDepth(root->right);
    }

    return 1 + min_depth;
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

class Solution117 {
 public:
  Node *connect(Node *root) {
    if (root == nullptr) {
      return root;
    }

    std::vector<Node*> queue;
    if (root->left) {
      queue.push_back(root->left);
    }

    if (root->right) {
      queue.push_back(root->right);
    }

    while (!queue.empty()) {
      vector<Node*> nextLevelQueue;
      Node* last = queue.front();
      if (last->left) {
        nextLevelQueue.push_back(last->left);
      }

      if (last->right) {
        nextLevelQueue.push_back(last->right);
      }

      for (int i=1; i<queue.size(); i++) {
        last->next = queue[i];
        last = queue[i];

        if (queue[i]->left) {
          nextLevelQueue.push_back(queue[i]->left);
        }
        if (queue[i]->right) {
          nextLevelQueue.push_back(queue[i]->right);
        }
      }

      swap(queue, nextLevelQueue);
    }
    return root;
  }
};

class Solution129 {
 public:
  int traverse(TreeNode* root, int prevSum) {
    if (root == nullptr) {
      return 0;
    }

    int sum = prevSum * 10 + root->val;
    if (!root->left && !root->right) {
      return sum;
    } else {
      sum = traverse(root->left,sum) + traverse(root->right, sum);
    }
    return sum;
  }

  int sumNumbers(TreeNode* root) {
    return traverse(root, 0);
  }
};
int main() {
  Solution129 sol;
  TreeNode* root = sol.construct();
  sol.sumNumbers(root);
  return 0;
}