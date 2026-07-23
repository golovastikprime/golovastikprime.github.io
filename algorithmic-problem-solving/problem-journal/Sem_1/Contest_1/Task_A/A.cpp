#include <iostream>
#include <vector>

struct Point {
  long long x;
  long long y;

  bool operator<=(const Point& right) const { return this->x <= right.x; }
};

// template <typename T>
namespace Detail {
struct Sort {
  using T = Point;
  using Vec = std::vector<T>;
  using Cvec = const Vec;

  static void MergeSort(Vec& array, long long l, long long r) {
    if (l >= r) {
      return;
    }

    long long m = l + ((r - l) / 2);
    MergeSort(array, l, m);
    MergeSort(array, m + 1, r);
    Merge(array, l, m, r);
  }

  static void Merge(Vec& array, long long l, long long m, long long r) {
    long long n1 = m - l + 1;
    long long n2 = r - m;
    Vec left_array(n1);
    Vec right_array(n2);

    for (long long i = 0; i < n1; ++i) {
      left_array[i] = array[l + i];
    }

    for (long long j = 0; j < n2; ++j) {
      right_array[j] = array[m + 1 + j];
    }

    long long i = 0;
    long long j = 0;
    long long k = l;

    while (i < n1 && j < n2) {
      if (left_array[i] <= right_array[j]) {
        array[k++] = left_array[i++];
      } else {
        array[k++] = right_array[j++];
      }
    }

    while (i < n1) {
      array[k++] = left_array[i++];
    }

    while (j < n2) {
      array[k++] = right_array[j++];
    }
  }
};
}  // namespace Detail

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  long long n;
  std::cin >> n;
  std::vector<Point> a(n);

  for (int i = 0; i < n; ++i) {
    std::cin >> a[i].x;
    std::cin >> a[i].y;
  }

  Detail::Sort::MergeSort(a, 0, n - 1);

  std::vector<Point> ans;
  Point curr = a[0];
  for (long long i = 1; i < n; ++i) {
    Point next = a[i];
    if (next.x <= curr.y) {
      curr.y = std::max(curr.y, next.y);
    } else {
      ans.push_back(curr);
      curr = next;
    }
  }

  ans.push_back(curr);

  std::cout << ans.size() << '\n';
  for (const auto& p : ans) {
    std::cout << p.x << ' ' << p.y << '\n';
  }
}
