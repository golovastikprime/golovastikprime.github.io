#include <cstdint>
#include <iostream>
#include <vector>

void LSD(std::vector<uint64_t>& nums) {
  const int cKBase = 256;
  const int cKBytes = 8;

  std::vector<uint64_t> buffer(nums.size());

  for (int byte = 0; byte < cKBytes; ++byte) {
    int count[cKBase]{};
    int shift = cKBytes * byte;
    for (uint64_t num : nums) {
      uint64_t digit = (num >> shift) & (cKBase - 1);
      ++count[digit];
    }

    int pos[cKBase]{};
    pos[0] = 0;

    for (int i = 1; i < cKBase; ++i) {
      pos[i] = pos[i - 1] + count[i - 1];
    }

    for (uint64_t num : nums) {
      uint64_t digit = (num >> shift) & (cKBase - 1);
      buffer[pos[digit]] = num;
      ++pos[digit];
    }
    nums.swap(buffer);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n;
  std::cin >> n;

  std::vector<uint64_t> nums(n, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> nums[i];
  }

  LSD(nums);

  for (auto x : nums) {
    std::cout << x << '\n';
  }

  return 0;
}
