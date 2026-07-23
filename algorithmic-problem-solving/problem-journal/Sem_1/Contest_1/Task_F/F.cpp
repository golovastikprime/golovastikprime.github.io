#include <iostream>
#include <vector>

struct Item {
  int data;
  int idx;
};

class Sort {
 public:
  Sort(std::vector<Item>& arr) : f_(arr.size(), 0), arr_ref_(arr) {}

  void Apply() {
    MergeSort(arr_ref_, 0, static_cast<int>(arr_ref_.size()) - 1);
  }

  const std::vector<int>& Get() const { return f_; }

 private:
  void MergeSort(std::vector<Item>& arr, int left, int right) {
    if (left >= right) {
      return;
    }

    int mid = left + ((right - left) / 2);

    MergeSort(arr, left, mid);
    MergeSort(arr, mid + 1, right);
    Merge(arr, left, mid, right);
  }

  void Merge(std::vector<Item>& arr, int left, int mid, int right) {
    int n = mid - left + 1;
    int m = right - mid;

    std::vector<Item> arr_1(n);
    std::vector<Item> arr_2(m);

    for (int i = 0; i < n; ++i) {
      arr_1[i] = arr[left + i];
    }

    for (int j = 0; j < m; ++j) {
      arr_2[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n && j < m) {
      if (arr_1[i].data <= arr_2[j].data) {
        arr[k] = arr_1[i];
        f_[arr_1[i].idx] += j;
        ++i;
      } else {
        arr[k] = arr_2[j];
        ++j;
      }
      ++k;
    }

    while (i < n) {
      arr[k] = arr_1[i];
      f_[arr_1[i].idx] += j;
      ++i;
      ++k;
    }

    while (j < m) {
      arr[k] = arr_2[j];
      ++j;
      ++k;
    }
  }

  std::vector<int> f_;
  std::vector<Item>& arr_ref_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;

  std::vector<Item> arr(n);

  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i].data;
    arr[i].idx = i;
  }

  Sort sr(arr);
  sr.Apply();

  for (int x : sr.Get()) {
    std::cout << x << '\n';
  }

  return 0;
}
