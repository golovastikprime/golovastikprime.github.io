#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

class Segment {
 public:
  using Type = long long;

  Segment() = default;

  ~Segment() = default;

  explicit Segment(Type left, Type right) : left_(left), right_(right) {}

  Segment(const Segment&) = default;

  Type GetLeft() const { return left_; }

  Type GetRight() const { return right_; }

  friend bool operator==(const Segment& lhs, const Segment& rhs) {
    return ((lhs.GetLeft() == rhs.GetLeft()) &&
            (lhs.GetRight() == rhs.GetRight()));
  }

  friend bool operator<(const Segment& lhs, const Segment& rhs) {
    return lhs.Less(rhs);
  }

  bool Less(const Segment& rhs) const {
    if (GetLeft() == rhs.GetLeft()) {
      return GetRight() > rhs.GetRight();
    }

    return GetLeft() < rhs.GetLeft();
  }

 private:
  Type left_;
  Type right_;
};

struct Item {
  Item(const Segment& seg, int count) : seg(seg), count(count) {}
  Segment seg;
  int count{1};
};

class SegmentTree {
 public:
  SegmentTree(const std::vector<long long>& rights) {
    N_ = 1;
    while (N_ < rights.size()) {
      N_ *= 2;
    }

    tree_.assign(N_ * 2, 0);
  }

  long long Range(int l, int r) const {
    l += N_;
    r += N_;
    long long ans = 0;
    while (l <= r) {
      if ((l & 1) != 0) {
        ans += tree_[l++];
      }

      if ((r & 1) == 0) {
        ans += tree_[r--];
      }
      l = Parent(l);
      r = Parent(r);
    }

    return ans;
  }

  void Add(int p, int c) {
    int v = N_ + p;
    tree_[v] += c;
    for (; v > 1; v /= 2) {
      tree_[v / 2] = tree_[v] + tree_[v ^ 1];
    }
  }

 private:
  static int LeftChild(int v) { return 2 * v; }
  static int RightChild(int v) { return (2 * v) + 1; }
  static int Parent(int v) { return v >> 1; }
  std::vector<int> tree_;
  std::size_t N_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;

  std::vector<Segment> segmnets(n);
  std::vector<long long> rights(n);

  for (int i = 0; i < n; ++i) {
    long long l;
    long long r;
    std::cin >> l >> r;
    segmnets[i] = Segment(l, r);
    rights[i] = r;
  }

  std::sort(segmnets.begin(), segmnets.end());
  std::sort(rights.begin(), rights.end());

  std::vector<Item> seg_equiv;
  std::vector<long long> rights_equiv;
  seg_equiv.reserve(n);

  int i = 0;
  int cur_count = 1;
  int s = 0;
  // int cur_count_rights = 1;
  for (int j = 1; j < n; ++j) {
    if (segmnets[i] == segmnets[j]) {
      ++cur_count;
    } else {
      seg_equiv.emplace_back(segmnets[i], cur_count);
      cur_count = 1;
      i = j;
    }

    if (rights[s] == rights[j]) {
      //++cur_count_rights;
    } else {
      rights_equiv.emplace_back(rights[s]);
      // ur_count_rights = 1;
      s = j;
    }
  }

  rights_equiv.emplace_back(rights[s]);
  seg_equiv.emplace_back(segmnets[i], cur_count);

  auto pos = [&rights_equiv](int r) {
    int left = 0;
    int right = rights_equiv.size() - 1;

    while (left <= right) {
      int mid = left + ((right - left) / 2);

      if (rights_equiv[mid] == r) {
        return mid;
      }

      if (rights_equiv[mid] < r) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }

    return -1;
  };

  SegmentTree st(rights_equiv);
  std::size_t k = rights_equiv.size();
  long long answer = 0;
  for (auto& [seg, c] : seg_equiv) {
    int p = pos(seg.GetRight());
    answer += st.Range(p, k - 1) * c;
    st.Add(p, c);
  }

  std::cout << answer;
}
