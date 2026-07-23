#include <iostream>
#include <vector>

const int64_t cMod = 1e7 + 4321;
const int64_t cA = 123;
const int64_t cB = 45;

int Partition(std::vector<int64_t>& nums, int64_t left, int64_t right) {
  int64_t pivot_idx = left + (rand() % (right - left + 1));
  int64_t pivot = nums[pivot_idx];
  std::swap(nums[pivot_idx], nums[right]);

  int j = left;
  for (int i = left; i < right; ++i) {
    if (nums[i] <= pivot) {
      std::swap(nums[i], nums[j]);
      ++j;
    }
  }
  std::swap(nums[right], nums[j]);

  return j;
}

int64_t QuickSelectHelper(std::vector<int64_t>& nums, int64_t left,
                          int64_t right, int64_t k) {
  if (left >= right) {
    return nums[left];
  }

  auto p = Partition(nums, left, right);
  if (p == k) {
    return nums[p];
  }

  if (p > k) {
    return QuickSelectHelper(nums, left, p - 1, k);
  }

  return QuickSelectHelper(nums, p + 1, right, k);
}

int64_t QuickSelect(std::vector<int64_t>& nums, int64_t k) {
  return QuickSelectHelper(nums, 0, nums.size() - 1, k);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int64_t n;
  int64_t k;

  std::cin >> n >> k;

  std::vector<int64_t> nums(n);
  std::cin >> nums[0] >> nums[1];

  for (int i = 2; i < n; ++i) {
    int64_t a = (nums[i - 1] * cA) % cMod;
    int64_t b = (nums[i - 2] * cB) % cMod;
    nums[i] = (a + b) % cMod;
  }

  std::cout << QuickSelect(nums, k - 1);
}
