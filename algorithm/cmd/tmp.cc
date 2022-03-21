#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <queue>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
  return 0;
}

using namespace std;

// "abcabcbb"

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct Op {
  bool operator()(const ListNode* lhs, const ListNode* rhs) {
    return lhs->val >= rhs->val;
  }
};

int partition(vector<int>& nums, int left, int right) {
  int val = nums[left];
  while (left < right) {
    while (left < right && nums[right] >= val) {
      --right;
    }
    nums[left] = nums[right];

    while (left < right && nums[left] <= val) {
      ++left;
    }
    nums[right] = nums[left];
  }

  nums[left] = val;

  return left;
}

int quickSort(vector<int>& nums, int low, int high) {
  if (low >= high) {
    return 0;
  }

  int mid = partition(nums, low, high);
  quickSort(nums, low, mid-1);
  quickSort(nums, mid+1, high);
  return 0;
}

int merge(vector<int>& nums, int left, int mid, int right) {
  int len = (right - left) + 1;
  int* tmp = new int[len];

  int leftStart = left;
  int leftEnd = mid;
  int rightStart = mid+1;
  int rightEnd = right;

  int tmpIndex = 0;
  while (leftStart <= leftEnd && rightStart <= rightEnd) {
    if (nums[leftStart] <= nums[rightStart]) {
      tmp[tmpIndex] = nums[leftStart];
      ++tmpIndex;
      ++leftStart;
    } else {
      tmp[tmpIndex] = nums[rightStart];
      ++tmpIndex;
      ++rightStart;
    }
  }

  if (leftStart > leftEnd) {
    while (rightStart <= rightEnd) {
      tmp[tmpIndex] = nums[rightStart];
      ++rightStart;
      ++tmpIndex;
    }
  }

  if (rightStart > rightEnd) {
    while (leftStart <= leftEnd) {
      tmp[tmpIndex] = nums[leftStart];
      ++leftStart;
      ++tmpIndex;
    }
  }

  for (int i=left; i<=right; i++) {
    nums[left] = tmp[i-left];
  }

  delete []tmp;

  return 0;
}

void mergeSort(vector<int>& nums, int left, int right) {
  if (left >= right) {
    return;
  }

  int mid = left + (right - left) / 2;
  mergeSort(nums, left, mid);
  mergeSort(nums, mid+1, right);
  merge(nums, left, mid, right);
}

int sort(vector<int>& nums) {
  int len = nums.size();

  if (nums.empty()) {
    return 0;
  }

  for (int i=0; i<len; i++) {
    for (int j=1; j<len-i; j++) {
      if (nums[j] <= nums[j-1]) {
        swap(nums[j], nums[j-1]);
      }
    }
  }

  return 0;
}

int heapSort() {

}

void print(const vector<int>& nums) {
  for (auto num : nums) {
    std::cout << num << std::endl;
  }
}

int main() {
  vector<int> input{2,0,1};
  quickSort(input, 0, input.size()-1);
  print(input);
  return 0;
}