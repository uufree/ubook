#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <unistd.h>

using namespace std;

// define
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* construct(const std::vector<int>& vec) {
  ListNode* node = new ListNode();
  ListNode* p = node;
  for (auto& v : vec) {
    p->next = new ListNode(v);
    p = p->next;
  }
  return node->next;
}

void print(ListNode* head) {
  while (head) {
    std::cout << head->val << std::endl;
    head = head->next;
  }
}

// 合并两个有序list
// Tips：使用dummy节点，可以有效降低写代码的复杂度
class Solution21 {
 public:
  ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr) {
      return l2;
    }

    if (l2 == nullptr) {
      return l1;
    }

    ListNode* dummy = new ListNode(0);
    ListNode* p1 = dummy;
    while (l1 && l2) {
      if (l1->val <= l2->val) {
        p1->next = l1;
        l1 = l1->next;
      } else {
        p1->next = l2;
        l2 = l2->next;
      }
      p1 = p1->next;
    }

    if (l1 == nullptr) {
      p1->next = l2;
    } else if (l2 == nullptr) {
      p1->next = l1;
    }

    return dummy->next;
  }
};

// 合并K个有序list
// Tips: 使用dummy节点，可以有效降低复杂度
// Tips: 自定义堆排序函数
struct ListNodeOp {
  bool operator()(ListNode* a, ListNode* b) {
    return a->val >= b->val;
  }
};
class Solution23 {
 public:
  ListNode* mergeKLists(vector<ListNode*>& lists) {
    std::priority_queue<ListNode*, std::vector<ListNode*>, ListNodeOp> pq;
    for (auto& list : lists) {
      if (list) {
        pq.push(list);
      }
    }

    ListNode* dummy = new ListNode();
    ListNode* p1 = dummy;
    while (!pq.empty()) {
      p1->next = pq.top();
      pq.pop();

      ListNode* l1 = p1->next;
      if (l1->next) {
        pq.push(l1->next);
      }
      p1 = p1->next;
    }

    return dummy->next;
  }
};

// 单链表的倒数第k个节点
// Tips: 双指针解法。数学公式：n - (n-k) = k
class SolutionOffer22 {
 public:
  ListNode* getKthFromEnd(ListNode* head, int k) {
    if (head == nullptr) {
      return nullptr;
    }

    ListNode* p1 = head;
    for (int i=0; i<k; i++) {
      p1 = p1->next;
    }

    ListNode* p2 = head;
    while (p1) {
      p1 = p1->next;
      p2 = p2->next;
    }

    return p2;
  }
};

// 删除单链表中的倒数第k个节点
// Tips: 如果遍历数量要增大，记得使用dummy节点
// Tips: 寻找倒数第K个节点的变种
class Solution19 {
 public:
  ListNode *removeNthFromEnd(ListNode *head, int n) {
    if (head == nullptr) {
      return head;
    }

    ListNode* dummy = new ListNode(0);
    dummy->next = head;

    ListNode* p1 = dummy;
    ListNode* p2 = dummy;
    for (int i=0; i<n+1; i++) {
      p1 = p1->next;
    }

    while (p1) {
      p1 = p1->next;
      p2 = p2->next;
    }

    if (p2->next) {
      p2->next = p2->next->next;
    }
    return dummy->next;
  }
};

// 寻找链表的中点
// Tips: 快慢指针
// Tips: 将判断条件写到while循环中
class Solution876 {
 public:
  ListNode* middleNode(ListNode* head) {
    if (head == nullptr) {
      return head;
    }

    if (head->next == nullptr) {
      return head;
    }

    ListNode* fast = head;
    ListNode* slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
  }
};

// 链表中是否包含环
// Tips: 快慢指针相遇即有环
// Tips: 将判断条件写到while循环中
class Solution141 {
 public:
  bool hasCycle(ListNode *head) {
    if (!head || !head->next) {
      return false;
    }

    ListNode* fast = head;
    ListNode* slow = head;
    while (fast && fast->next) {
      fast = fast->next->next;
      slow = slow->next;

      if (fast == slow) {
        return true;
      }
    }
    return false;
  }
};

// 链表中环的起点
// Tips: 快慢指针
// Tips: 将判断条件写到while循环中
// Tips: 先检测相遇点，再同步执行
// Tips: 当两个指针相遇时，fast走了2n步，slow走了n步，即环的长度为n。
// 假设环的起点距离相遇点的距离为m：
// 从slow来看：头节点距离环的起点为n-m
// 从fast来看：从相遇点再走n-m步，就到了环的起点
// 因此，将slow或者fast重置为head，之后两个指针同步走，再次相遇的点就是环的起点
class SolutionOffer022 {
 public:
  ListNode *detectCycle(ListNode *head) {
    if (!head || !head->next) {
      return nullptr;
    }

    bool meeting = false;
    ListNode* fast = head;
    ListNode* slow = head;
    while (fast && fast->next) {
      fast = fast->next->next;
      slow = slow->next;

      if (fast == slow) {
        meeting = true;
        break;
      }
    }

    if (!meeting) {
      return nullptr;
    }

    slow = head;
    while (fast) {
      if (fast == slow) {
        return fast;
      }

      slow = slow->next;
      fast = fast->next;
    }

    return nullptr;
  }
};

// 寻找相交链表的起点
// Tips: 相交链表A、B的前半部长度可能不一致，如果A、B的前半部都遍历了，那么相遇的点就是相交链表的起点
class Solution160 {
 public:
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode* p1 = headA;
    ListNode* p2 = headB;
    while (p1 != p2) {
      if (p1) {
        p1 = p1->next;
      } else {
        p1 = headB;
      }

      if (p2) {
        p2 = p2->next;
      } else {
        p2 = headA;
      }
    }
    return p1;
  }
};

// 反转链表
class Solution206 {
 public:
  ListNode* reverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode* p = head;
    ListNode* prev = nullptr;
    ListNode* cur = p;
    while (cur) {
      ListNode* next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }

    return prev;
  }
};

// 反转链表，递归写法
class Solution206_recursion {
 public:
  ListNode* reverse(ListNode* node) {
    if (node->next == nullptr) {
      return node;
    }

    ListNode* last = reverse(node->next);
    node->next->next = node;
    node->next = nullptr;
    return last;
  }
  ListNode* reverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    return reverse(head);
  }
};

// 反转链表的前n个节点
class Solution206_pro_recursion {
 public:
  ListNode* second = nullptr;

  ListNode* reverse(ListNode* node, int n) {
    if (node->next == nullptr || n == 1) {
      second = node->next;
      return node;
    }

    ListNode* last = reverse(node->next, n-1);
    node->next->next = node;
    node->next = second;
    return last;
  }
  ListNode* reverseList(ListNode* head, int n) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    if (n == 1 || n == 0) {
      return head;
    }

    return reverse(head, n);
  }
};

// 反转链表的中间几个节点
class Solution206_promax_recursion {
 public:
  ListNode* second = nullptr;
  ListNode* reverseN(ListNode* node, int n) {
    if (node->next == nullptr || n == 1) {
      second = node->next;
      return node;
    }

    ListNode* last = reverseN(node->next, n-1);
    node->next->next = node;
    node->next = second;
    return last;
  }

  ListNode* reverseList(ListNode* head, int start, int end) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    if (start == 1) {
      return reverseN(head, end);
    }
    head->next = reverseList(head->next, start-1, end-1);
    return head;
  }
};

// K个一组的反转链表
class Solution25 {
 public:
  ListNode* reverse(ListNode* start, ListNode* end) {
    if (start == end) {
      return start;
    }

    ListNode* last = reverse(start->next, end);
    start->next->next = start;
    start->next = nullptr;
    return last;
  }

  ListNode* reverseKGroup(ListNode* head, int k) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode* start = head;
    ListNode* end = head;
    for (int i=0; i<k-1; i++) {
      if (end == nullptr) {
        return head;
      }
      end = end->next;
    }

    if (end == nullptr) {
      return head;
    }

    ListNode* newStart = end->next;
    ListNode* newHead = reverse(start, end);
    start->next = reverseKGroup(newStart, k);
    return newHead;
  }
};

int main() {
  ListNode* l1 = construct({1,2,3,4,5});

  Solution25 sol;
  ListNode* res = sol.reverseKGroup(l1, 3);
  print(res);

  return 0;
}
