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

// 二叉树反转
// Tips：前序遍历
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

// 填充每个节点的下一个右侧指针
// Tips：单个参数节点无法完成任务时，可将任务拓展为多个参数
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

// 将二叉树拓展为单链表
class Solution114 {
 public:
  void flatten(TreeNode* root) {
    if (root == nullptr) {
      return;
    }

    flatten(root->left);
    flatten(root->right);

    TreeNode* left = root->left;
    TreeNode* right = root->right;

    root->left = nullptr;
    root->right = left;

    TreeNode* temp = root;
    while (temp->right) {
      temp = temp->right;
    }
    temp->right = right;
  }
};

// 根据一个不含重复率元素的数组构架最大二叉树
class Solution654 {
 public:
  TreeNode* build(const std::vector<int>& nums, int low, int hight) {
    if (low > hight) {
      return nullptr;
    }

    int max = -9999999;
    int index = -1;
    for (int i=low; i<=hight; i++) {
      if (nums[i] > max) {
        index = i;
        max = nums[i];
      }
    }

    TreeNode* node = new TreeNode(max);
    node->left = build(nums, low, index-1);
    node->right = build(nums, index+1, hight);
    return node;
  }

  TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
    if (nums.size() == 0) {
      return nullptr;
    }

    return build(nums, 0, nums.size()-1);
  }
};

// 以前序遍历和中序遍历构建一颗二叉树
class Solution105 {
 public:
  TreeNode* build(const std::vector<int>& pre, int preStart, int preEnd,
                  const std::vector<int>& post, int postStart, int postEnd) {
    if (preStart>preEnd) {
      return nullptr;
    }

    int rootValue = pre[preStart];
    int index = -1;
    for (int i=postStart; i<=postEnd; i++) {
      if (post[i] == rootValue) {
        index = i;
        break;
      }
    }

    int leftSize = index - postStart;

    TreeNode* root = new TreeNode(rootValue);
    root->left = build(pre, preStart+1, preStart+leftSize, post, postStart, index-1);
    root->right = build(pre, preStart+leftSize+1, preEnd, post, index+1, postEnd);
    return root;
  }
  TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    if (preorder.size() == 0) {
      return nullptr;
    }
    return build(preorder, 0, preorder.size()-1, inorder, 0, inorder.size()-1);
  }
};

// 以中序遍历和后序遍历构建一颗二叉树
class Solution106 {
 public:
  TreeNode* build(const std::vector<int>& inorder, int inStart, int inEnd,
                  const std::vector<int>& postOrder, int postStart, int postEnd) {
    if (inStart > inEnd) {
      return nullptr;
    }

    int rootValue = postOrder[postEnd];
    int index = -1;
    for (int i=inStart; i<=inEnd; i++) {
      if (inorder[i] == rootValue) {
        index = i;
        break;
      }
    }

    int leftLength = index - inStart;

    TreeNode* root = new TreeNode(rootValue);
    root->left = build(inorder, inStart, index-1, postOrder, postStart, postStart+leftLength-1);
    root->right = build(postOrder, index+1, inEnd, postOrder, postStart+leftLength, postEnd-1);
    return root;
  }

  TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    if (inorder.size() == 0) {
      return nullptr;
    }

    return build(inorder, 0, inorder.size()-1, postorder, 0, postorder.size()-1);
  }
};

// 寻找重复子树
class Solution652 {
 public:
  std::vector<TreeNode*> list;
  std::map<std::string, int> result;
 public:
  std::string treverse(TreeNode* root) {
    if (root == nullptr) {
      return "#";
    }

    std::string subLeft = treverse(root->left);
    std::string subRight = treverse(root->right);
    std::string merge = subLeft + "," + subRight + "," + std::to_string(root->val);
    if (result.find(merge) == result.end()) {
      result[merge] = 1;
    } else {
      if (result[merge] == 1) {
        list.push_back(root);
      }
      ++result[merge];
    }

    return merge;
  }

  vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
    if (root == nullptr) {
      return std::vector<TreeNode*>{};
    }
    treverse(root);
    return list;
  }
};

int main() {
  return 0;
}