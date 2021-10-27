#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 在二叉搜索树中寻找第k大元素
class Solution230 {
  int pos = 0;
  int value = 0;
 public:
  int kthSmallest(TreeNode* root, int k) {
    if (root == nullptr) {
      return 0;
    }

    kthSmallest(root->left, k);
    if (++pos == k) {
      value = root->val;
    }
    kthSmallest(root->right, k);
    return value;
  }
};

// 二叉树变累加树
class Solution538 {
  int sum = 0;
 public:
  TreeNode* convertBST(TreeNode* root) {
    if (root == nullptr) {
      return nullptr;
    }

    convertBST(root->right);
    sum += root->val;
    root->val = sum;
    convertBST(root->left);
    return root;
  }
};

// 验证二叉搜索树是否有效
// Tips：当前value值要大于左子树max，小于右子树min
class Solution98 {
 int max = INT32_MIN;
 public:
  bool isValidBST(TreeNode* root) {
    if (root == nullptr) {
      return true;
    }
    bool isValidLeft = isValidBST(root->left);
    bool isValidMid = false;
    if (root->val >= max) {
      isValidMid = true;
      max = root->val;
    }
    bool isValidRight = isValidBST(root->right);
    return isValidLeft && isValidMid && isValidRight;
  }
};

// 寻找二叉搜索树中的某个节点
class Solution700 {
 public:
  TreeNode* searchBST(TreeNode* root, int val) {
    if (root == nullptr) {
      return nullptr;
    }

    TreeNode* node = root;
    while (node && node->val != val) {
      if (node->val == val) {
        break;
      } else if (node->val > val) {
        node = node->left;
      } else if (node->val < val) {
        node = node->right;
      }
    }
    return node;
  }
};

// 删除二叉搜索树中的某个节点
// 1. 如果没有子节点，直接删除
// 2. 如果仅有一个子节点，直接替换
// 3. 如果有两个子节点，寻找左子树最大 or 右子树最小的节点
class Solution450 {
 public:
  TreeNode* getRightMin(TreeNode* root) {
    while (root->left) {
      root = root->left;
    }
    return root;
  }

  TreeNode* deleteNode(TreeNode* root, int key) {
    if (root == nullptr) {
      return nullptr;
    }

    if (root->val == key) {
      if (!root->left && !root->right) {
        return nullptr;
      } else if (root->left && !root->right) {
        return root->left;
      } else if (root->right && !root->left) {
        return root->right;
      } else if (root->left && root->right) {
        TreeNode* min = getRightMin(root->right);
        root->val = min->val;
        root->right = deleteNode(root->right, min->val);
      }

    } else if (root->val > key) {
      root->left = deleteNode(root->left, key);
    } else if (root->val < key) {
      root->right = deleteNode(root->right, key);
    }

    return root;
  }
};

// 在二叉搜索树中插入指定节点
class Solution701 {
 public:
  TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (root == nullptr) {
      return new TreeNode(val);
    }
    if (root->val > val) {
      root->left = insertIntoBST(root->left, val);
    } else if (root->val < val) {
      root->right = insertIntoBST(root->right, val);
    }

    return root;
  }
};

class Solution96 {
 public:
  std::vector<std::vector<int>> cache;
  int count(int left, int right, vector<vector<int>>& cache) {
    if (left>right) {
      return 1;
    }

    if (cache[left][right] != 0) {
      return cache[left][right];
    }

    int res = 0;
    for (int i=left; i<=right; i++) {
      int leftCount = count(left, i-1, cache);
      int rightCount = count(i+1, right, cache);
      res += leftCount * rightCount;
    }
    cache[left][right] = res;
    return res;
  }

  int numTrees(int n) {
    if (n == 0) {
      return 0;
    }

    if (n == 1) {
      return 1;
    }
    cache.resize(n+1);
    for (auto& c : cache) {
      c.resize(n+1);
    }
    return count(1, n, cache);
  }
};

class Solution95 {
 public:
  vector<TreeNode*> build(int left, int right) {
    if (left > right) {
      return std::vector<TreeNode*>{nullptr};
    }

    vector<TreeNode*> result;
    for (int i=left; i<=right; i++) {
      auto leftTrees = build(left, i-1);
      auto rightTrees = build(i+1, right);
      for (auto& leftTree : leftTrees) {
        for (auto rightTree : rightTrees) {
          TreeNode* root = new TreeNode(i);
          root->left = leftTree;
          root->right = rightTree;
          result.push_back(root);
        }
      }
    }

    return result;
  }
  vector<TreeNode*> generateTrees(int n) {
    if (n == 0) {
      return std::vector<TreeNode*>{nullptr};
    }

    if (n == 1) {
      return std::vector<TreeNode*>{new TreeNode(1)};
    }
    return build(1, n);
  }
};

class Solution1373 {
 public:
  TreeNode* construct() {
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(4);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n4 = new TreeNode(2);
    TreeNode* n5 = new TreeNode(4);
    TreeNode* n6 = new TreeNode(2);
    TreeNode* n7 = new TreeNode(5);
    TreeNode* n8 = new TreeNode(4);
    TreeNode* n9 = new TreeNode(6);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;
    n3->left = n6;
    n3->right = n7;
    n7->left = n8;
    n7->right = n9;

    return n1;
  }

  TreeNode* construct1() {
    TreeNode* n1 = new TreeNode(4);
    TreeNode* n2 = new TreeNode(3);
    TreeNode* n3 = new TreeNode(1);
    TreeNode* n4 = new TreeNode(2);

    n1->left = n2;
    n2->left = n3;
    n2->right = n4;
    return n1;
  }

  TreeNode* construct2() {
    TreeNode* n1 = new TreeNode(-4);
    TreeNode* n2 = new TreeNode(-2);
    TreeNode* n3 = new TreeNode(-5);

    n1->left = n2;
    n1->right = n3;
    return n1;
  }

  TreeNode* construct3() {
    TreeNode* n1 = new TreeNode(5);
    TreeNode* n2 = new TreeNode(4);
    TreeNode* n3 = new TreeNode(8);
    TreeNode* n4 = new TreeNode(3);
    TreeNode* n5 = new TreeNode(6);
    TreeNode* n6 = new TreeNode(3);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n3->left = n5;
    n3->right = n6;
    return n1;
  }

  void print(TreeNode* root) {
    if (root == nullptr) {
      return;
    }
    std::cout << root->val << std::endl;
    print(root->left);
    print(root->right);
  }

  int getLeftMax(TreeNode* root) {
    if (!root) {
      return 0;
    }
    while (root->right) {
      root = root->right;
    }
    return root->val;
  }

  int getRightMin(TreeNode* root) {
    if (!root) {
      return 0;
    }
    while (root->left) {
      root = root->left;
    }
    return root->val;
  }

  int64_t max = INT64_MIN;
  bool treverse(TreeNode* root, int &sum) {
    if (root == nullptr) {
      return true;
    }

    int leftSum = 0;
    int rightSum = 0;
    bool leftIsBST = treverse(root->left, leftSum);
    bool rightIsBST = treverse(root->right, rightSum);
//    std::cout << root->val << "    " << int(leftIsBST && rightIsBST)  << "  " << leftSum << "  " << rightSum << std::endl;

//    if (root->val == 5) {
//      std::cout << "left: " << leftIsBST << std::endl;
//      std::cout << "left: " << leftSum << std::endl;
//      std::cout << "right: " << rightIsBST << std::endl;
//      std::cout << "right: " << rightSum << std::endl;
//    }

    if (leftIsBST && rightIsBST) {
      int leftMax = getLeftMax(root->left);
      int rightMin = getRightMin(root->right);
      // is BST
      if (root->left && root->right) {
        if (root->val > leftMax && root->val < rightMin) {
          int curSum = leftSum + rightSum + root->val;
          if (curSum > max) {
            max = curSum;
          }
          sum = curSum;
          return true;
        }
      }

      if (root->left && !root->right) {
        if (root->val > leftMax) {
          int curSum = leftSum + root->val;
          if (curSum > max) {
            max = curSum;
          }
          sum = curSum;
          return true;
        }
      }


      if (root->right && !root->left) {
        if (root->val < rightMin) {
          int curSum = rightSum + root->val;
          if (curSum > max) {
            max = curSum;
          }
          sum = curSum;
          return true;
        }
      }

      if (!root->left && !root->right) {
        int curSum = root->val;
        if (curSum > max) {
          max = curSum;
        }
        sum = curSum;
        return true;
      }
    }
    return false;
  }

  int maxSumBST(TreeNode* root) {
    if (root == nullptr) {
      return 0;
    }
    int sum = 0;
    treverse(root, sum);
    return max>=0 ? max : 0;
  }
};

class Codec297 {
 public:
  TreeNode* construct() {
    TreeNode* n1 = new TreeNode(5);
    TreeNode* n2 = new TreeNode(4);
    TreeNode* n3 = new TreeNode(8);
    TreeNode* n4 = new TreeNode(3);
    TreeNode* n5 = new TreeNode(6);
    TreeNode* n6 = new TreeNode(3);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n3->left = n5;
    n3->right = n6;
    return n1;
  }

  const std::string NullPos = "#";
  const std::string Sep = ",";

  // Encodes a tree to a single string.
  string serialize(TreeNode* root) {
    std::string res;
    if (root == nullptr) {
      res += NullPos;
      res += Sep;
      return res;
    }

    res += std::to_string(root->val) + Sep;
    res += serialize(root->left);
    res += serialize(root->right);
    return res;
  }

  // Decodes your encoded data to tree.
  TreeNode* deserialize(string data) {
    std::vector<TreeNode*> nodes;
    // cpp 缺少split，写起来太蛋疼了
  }
};

// 公共父节点
class Solution236 {
 public:
  TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr) {
      return nullptr;
    }

    if (root == p || root == q) {
      return root;
    }

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left != nullptr && right != nullptr) {
      return root;
    }

    if (left == nullptr && right == nullptr) {
      return nullptr;
    }

    return left == nullptr ? right : left;
  }
};

int main() {
  Codec297 sol;
  TreeNode* root = sol.construct();
  std::cout << sol.serialize(root) << std::endl;
  return 0;
}