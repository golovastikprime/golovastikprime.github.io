#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct QueueItem {
  int value;
  char oper;

  QueueItem(char oper, int value) : value(value), oper(oper) {}
};

class SegmentTree {
  struct Item {
    int value;
    int count{0};

    Item(int value, int count) : value(value), count(count) {}

    static Item Identity() { return Item(0, 0); }
  };

 public:
  SegmentTree(const std::vector<long long>& coords)
      : coords_(coords), counts_(coords.size(), 0) {
    while (N_ < (int)coords.size()) {
      N_ *= 2;
    }

    tree_.assign(N_ * 2, 0);
  }

  void Insert(int index) {
    int v;
    v = index + N_;

    counts_[index]++;
    tree_[v] = coords_[index] * counts_[index];

    for (; v > 1; v /= 2) {
      tree_[v / 2] = tree_[v] + tree_[v ^ 1];
    }
  }

  long long Sum(int l, int r) {
    l += N_;
    r += N_;
    long long ans = 0;
    while (l <= r) {
      if ((l & 1) != 0) {
        ans += tree_[l];
        ++l;
      }

      if ((r & 1) == 0) {
        ans += tree_[r];
        --r;
      }
      l = Parent(l);
      r = Parent(r);
    }

    return ans;
  }

 private:
  static int LeftChild(int v) { return 2 * v; }

  static int RightChild(int v) { return (2 * v) + 1; }

  static int Parent(int v) { return v >> 1; }

  std::vector<long long> tree_;
  const std::vector<long long>& coords_;
  std::vector<long long> counts_;
  int N_{1};
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::queue<QueueItem> queue;

  int queries;
  std::cin >> queries;

  std::vector<int> coords;
  coords.reserve(queries);

  for (int i = 0; i < queries; ++i) {
    char oper;
    int value;
    std::cin >> oper >> value;
    queue.emplace(oper, value);
    if (oper == '+') {
      coords.push_back(value);
    }
  }

  std::sort(coords.begin(), coords.end());

  std::vector<long long> coords_equiv;
  coords_equiv.reserve(coords.size());

  int s = 0;
  for (int j = 1; j < (int)coords.size(); ++j) {
    if (coords[s] != coords[j]) {
      coords_equiv.emplace_back(coords[s]);
      s = j;
    }
  }

  if (!coords.empty()) {
    coords_equiv.emplace_back(coords[s]);
  }

  auto pos = [&coords_equiv](int x) {
    int left = 0;
    int right = coords_equiv.size() - 1;
    while (left <= right) {
      int mid = left + ((right - left) / 2);

      if (coords_equiv[mid] == x) {
        return mid;
      }

      if (coords_equiv[mid] < x) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }

    return -1;
  };

  auto lower_bound = [&coords_equiv](int x) {
    int left = 0;
    int right = coords_equiv.size() - 1;
    int ans = coords_equiv.size();
    while (left <= right) {
      int mid = left + ((right - left) / 2);

      if (coords_equiv[mid] > x) {
        ans = mid;
        right = mid - 1;
      } else {
        left = mid + 1;
      }
    }
    return ans;
  };

  SegmentTree st(coords_equiv);
  while (!queue.empty()) {
    auto [x, oper] = queue.front();
    queue.pop();

    switch (oper) {
      case '+': {
        int index = pos(x);

        st.Insert(index);
      } break;

      case '?': {
        int lb = lower_bound(x);
        std::cout << st.Sum(0, lb - 1) << '\n';
      } break;
    }
  }
}
