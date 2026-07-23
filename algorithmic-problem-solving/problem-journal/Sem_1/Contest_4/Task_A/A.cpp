#include <iostream>

const long long cMod = 1e9;

class Avl {
  struct BaseNode {
    BaseNode() = default;

    virtual ~BaseNode() = default;

    BaseNode(int height) : height(height) {}

    int height = 0;
    BaseNode* parent = nullptr;
    BaseNode* left_child = nullptr;
    BaseNode* right_child = nullptr;
  };

  struct Node : BaseNode {
    Node(int value) : Node(value, 1) {}
    Node(int value, int height) : BaseNode(height), value(value) {}
    int value;
  };

 public:
  Avl() = default;

  ~Avl() { Clear(); }

  void Insert(int value) {
    BaseNode* node = dummy_root_.left_child;
    BaseNode* parent = &dummy_root_;
    bool was_left = true;

    while (node != nullptr) {
      if (Value(node) == value) {
        return;
      }
      parent = node;
      if (value < Value(node)) {
        was_left = true;
        node = node->left_child;
      } else {
        node = node->right_child;
        was_left = false;
      }
    }

    BaseNode* new_node = new Node(value);
    new_node->parent = parent;
    if (was_left) {
      parent->left_child = new_node;
    } else {
      parent->right_child = new_node;
    }

    BaseNode* v = new_node->parent;

    while (v != &dummy_root_) {
      parent = v->parent;
      was_left = parent->left_child == v;
      BaseNode* new_root = Rebalance(v);
      if (was_left) {
        parent->left_child = new_root;
      } else {
        parent->right_child = new_root;
      }
      new_root->parent = parent;
      v = parent;
    }
  }

  int LowerBound(int x) const {
    BaseNode* v = dummy_root_.left_child;
    BaseNode* candidate = nullptr;

    while (v != nullptr) {
      if (Value(v) >= x) {
        candidate = v;
        v = v->left_child;
      } else {
        v = v->right_child;
      }
    }

    if (candidate == nullptr) {
      return -1;
    }

    return Value(candidate);
  }

 private:
  static int Height(BaseNode* v) {
    if (v == nullptr) {
      return 0;
    }
    return v->height;
  }

  static void UpdateHeight(BaseNode* v) {
    v->height = 1 + std::max(Height(v->left_child), Height(v->right_child));
  }

  static int BalanceFactor(BaseNode* v) {
    return Height(v->left_child) - Height(v->right_child);
  }

  static BaseNode* RotateRight(BaseNode* y) {
    BaseNode* old_parent = y->parent;

    BaseNode* x = y->left_child;
    BaseNode* b = x->right_child;

    y->left_child = b;
    if (b != nullptr) {
      b->parent = y;
    }

    x->right_child = y;
    y->parent = x;
    x->parent = old_parent;

    UpdateHeight(y);
    UpdateHeight(x);

    return x;
  }

  static BaseNode* RotateLeft(BaseNode* x) {
    BaseNode* old_parent = x->parent;

    BaseNode* y = x->right_child;

    x->right_child = y->left_child;
    if (x->right_child != nullptr) {
      x->right_child->parent = x;
    }

    x->parent = y;
    y->left_child = x;
    y->parent = old_parent;

    UpdateHeight(x);
    UpdateHeight(y);

    return y;
  }

  static BaseNode* Rebalance(BaseNode* v) {
    UpdateHeight(v);

    if (BalanceFactor(v) == 2) {
      if (BalanceFactor(v->left_child) < 0) {
        v->left_child = RotateLeft(v->left_child);
      }
      return RotateRight(v);
    }

    if (BalanceFactor(v) == -2) {
      if (BalanceFactor(v->right_child) > 0) {
        v->right_child = RotateRight(v->right_child);
      }
      return RotateLeft(v);
    }

    return v;
  }

  static int Value(BaseNode* v) { return static_cast<Node*>(v)->value; }

  void Clear() {
    auto* root = dummy_root_.left_child;
    Clear(root);
  }

  void Clear(BaseNode* root) {
    if (root == nullptr) {
      return;
    }

    Clear(root->left_child);
    Clear(root->right_child);
    delete root;
  }

  BaseNode dummy_root_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int q;
  std::cin >> q;
  Avl tree;
  long long y = -1;
  bool last_was_insert = true;
  for (int i = 0; i < q; ++i) {
    char op;
    int x;
    std::cin >> op >> x;
    if (op == '+') {
      if (last_was_insert) {
        tree.Insert(x);
      } else {
        long long temp = (y + x) % cMod;
        tree.Insert(temp);
      }
      last_was_insert = true;
    } else if (op == '?') {
      y = tree.LowerBound(x);
      last_was_insert = false;
      std::cout << y << '\n';
    }
  }
}
