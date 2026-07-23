#include <iostream>
#include <string>
#include <vector>

struct HeapItem {
  int64_t value;
  int query_id;
};

class MinHeap {
 public:
  explicit MinHeap(int query_count) : position_(query_count, -1) {}

  int Size() const { return static_cast<int>(buffer_.size()); }

  bool Empty() const { return buffer_.empty(); }

  const HeapItem& GetMin() const { return buffer_[0]; }

  void Insert(int64_t value, int query_id) {
    int vertex = Size();

    buffer_.push_back({value, query_id});
    position_[query_id] = vertex;

    SiftUp(vertex);
  }

  HeapItem ExtractMin() {
    HeapItem result = buffer_[0];
    int last = Size() - 1;

    if (last == 0) {
      buffer_.pop_back();
      position_[result.query_id] = -1;
      return result;
    }

    SwapItems(0, last);

    buffer_.pop_back();
    position_[result.query_id] = -1;

    SiftDown(0);

    return result;
  }

  void DecreaseKey(int query_id, int64_t delta) {
    int vertex = position_[query_id];

    buffer_[vertex].value -= delta;

    SiftUp(vertex);
  }

 private:
  static int Parent(int vertex) { return (vertex - 1) / 2; }

  static int Left(int vertex) { return (2 * vertex) + 1; }

  static int Right(int vertex) { return (2 * vertex) + 2; }

  static bool Less(const HeapItem& lhs, const HeapItem& rhs) {
    if (lhs.value != rhs.value) {
      return lhs.value < rhs.value;
    }

    return lhs.query_id < rhs.query_id;
  }

  void SwapItems(int first, int second) {
    HeapItem temporary = buffer_[first];
    buffer_[first] = buffer_[second];
    buffer_[second] = temporary;

    position_[buffer_[first].query_id] = first;
    position_[buffer_[second].query_id] = second;
  }

  void SiftUp(int vertex) {
    while (vertex > 0) {
      int parent = Parent(vertex);

      if (!Less(buffer_[vertex], buffer_[parent])) {
        return;
      }

      SwapItems(vertex, parent);
      vertex = parent;
    }
  }

  void SiftDown(int vertex) {
    while (true) {
      int best = vertex;
      int left = Left(vertex);
      int right = Right(vertex);

      if (left < Size() && Less(buffer_[left], buffer_[best])) {
        best = left;
      }

      if (right < Size() && Less(buffer_[right], buffer_[best])) {
        best = right;
      }

      if (best == vertex) {
        return;
      }

      SwapItems(vertex, best);
      vertex = best;
    }
  }

  std::vector<HeapItem> buffer_;

  std::vector<int> position_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int q;
  std::cin >> q;

  MinHeap heap(q);

  for (int query_id = 0; query_id < q; ++query_id) {
    std::string command;
    std::cin >> command;

    if (command[0] == 'i') {
      int64_t value;
      std::cin >> value;

      heap.Insert(value, query_id);
    } else if (command[0] == 'g') {
      std::cout << heap.GetMin().value << '\n';
    } else if (command[0] == 'e') {
      heap.ExtractMin();
    } else if (command[0] == 'd') {
      int insertion_query;
      int64_t delta;

      std::cin >> insertion_query >> delta;

      heap.DecreaseKey(insertion_query - 1, delta);
    }
  }

  return 0;
}
