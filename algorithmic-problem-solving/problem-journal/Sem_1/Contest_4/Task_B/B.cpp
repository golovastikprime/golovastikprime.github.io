#include <iostream>
#include <string>

class Splay {
  struct BaseNode {
    int size{0};
    BaseNode* parent{this};
    BaseNode* left_child{nullptr};
    BaseNode* right_child{nullptr};

    BaseNode() = default;
    BaseNode(int size) : size(size) {}
    virtual ~BaseNode() = default;
  };

  struct Node : BaseNode {
    Node(int value) : BaseNode(1), value(value) {}

    int value;
  };

  static int Value(BaseNode* v) { return static_cast<Node*>(v)->value; }

  static int Size(BaseNode* v) {
    if (v == nullptr) {
      return 0;
    }
    return v->size;
  }

  void Clear(BaseNode* node) {
    if (node == nullptr) {
      return;
    }

    Clear(node->left_child);
    Clear(node->right_child);
    delete node;
  }

 public:
  Splay() = default;

  ~Splay() { Clear(dummy_root_.left_child); }

  void Insert(int value) {
    BaseNode* current = dummy_root_.left_child;
    BaseNode* parent = &dummy_root_;
    bool was_left = true;

    while (current != nullptr) {
      if (Value(current) == value) {
        SplayRotate(current);
        return;
      }

      parent = current;
      if (Value(current) < value) {
        was_left = false;
        current = current->right_child;
      } else {
        was_left = true;
        current = current->left_child;
      }
    }

    BaseNode* new_node = new Node(value);
    new_node->parent = parent;
    if (was_left) {
      parent->left_child = new_node;
    } else {
      parent->right_child = new_node;
    }
    SplayRotate(new_node);
  }

  // 0-indexed K.
  std::string Kth(int k) {
    BaseNode* current = dummy_root_.left_child;

    if (current == nullptr || k < 0 || k >= current->size) {
      return "none";
    }

    while (current != nullptr) {
      int left_size = Size(current->left_child);

      if (left_size == k) {
        SplayRotate(current);
        return std::to_string(Value(current));
      }

      if (k < left_size) {
        current = current->left_child;
      } else {
        k -= left_size + 1;
        current = current->right_child;
      }
    }

    return "none";
  }

  std::string Next(int value) {
    BaseNode* current = dummy_root_.left_child;
    BaseNode* candidate = nullptr;
    BaseNode* last = current;

    while (current != nullptr) {
      last = current;
      if (Value(current) > value) {
        candidate = current;
        current = current->left_child;
      } else {
        current = current->right_child;
      }
    }

    if (last != nullptr) {
      SplayRotate(last);
    }

    if (candidate == nullptr) {
      return "none";
    }

    return std::to_string(Value(candidate));
  }

  std::string Prev(int value) {
    BaseNode* current = dummy_root_.left_child;
    BaseNode* candidate = nullptr;
    BaseNode* last = current;

    while (current != nullptr) {
      last = current;
      if (Value(current) < value) {
        candidate = current;
        current = current->right_child;
      } else {
        current = current->left_child;
      }
    }

    if (last != nullptr) {
      SplayRotate(last);
    }

    if (candidate == nullptr) {
      return "none";
    }

    return std::to_string(Value(candidate));
  }

  void Delete(int value) {
    auto [v, was_find] = FindNode(value);
    if (v == nullptr) {
      return;
    }

    SplayRotate(v);

    if (!was_find) {
      return;
    }

    BaseNode* left_subtree = v->left_child;
    BaseNode* right_subtree = v->right_child;

    if (left_subtree == nullptr) {
      dummy_root_.left_child = right_subtree;
      if (right_subtree != nullptr) {
        right_subtree->parent = &dummy_root_;
      }
      delete v;
      return;
    }

    BaseNode* max_leftsubtree = MaximumNode(left_subtree);
    left_subtree->parent = &dummy_root_;
    dummy_root_.left_child = left_subtree;
    SplayRotate(max_leftsubtree);

    max_leftsubtree->right_child = right_subtree;
    if (right_subtree != nullptr) {
      right_subtree->parent = max_leftsubtree;
    }

    UpdateSize(max_leftsubtree);
    delete v;
  }

  bool Find(int value) {
    auto [v, was_find] = FindNode(value);
    if (v != nullptr) {
      SplayRotate(v);
    }

    return was_find;
  }

 private:
  BaseNode dummy_root_;

  static void UpdateSize(BaseNode* v) {
    v->size = 1 + Size(v->left_child) + Size(v->right_child);
  }

  static void RotateLeft(BaseNode* v) {
    BaseNode* p = v->parent;
    BaseNode* b = v->left_child;

    v->left_child = p;
    p->parent = v;
    p->right_child = b;
    if (b != nullptr) {
      b->parent = p;
    }
  }

  static void RotateRight(BaseNode* v) {
    BaseNode* p = v->parent;
    BaseNode* b = v->right_child;

    v->right_child = p;
    p->parent = v;
    p->left_child = b;
    if (b != nullptr) {
      b->parent = p;
    }
  }

  static void Rotate(BaseNode* v) {
    BaseNode* p = v->parent;
    BaseNode* old_parent = p->parent;
    bool was_left = (old_parent->left_child == p);

    if (p->left_child == v) {
      RotateRight(v);
    } else if (p->right_child == v) {
      RotateLeft(v);
    }

    v->parent = old_parent;
    if (was_left) {
      old_parent->left_child = v;
    } else {
      old_parent->right_child = v;
    }

    UpdateSize(p);
    UpdateSize(v);
  }

  void SplayRotate(BaseNode* v) {
    while (v->parent != &dummy_root_) {
      BaseNode* p = v->parent;
      BaseNode* g = p->parent;
      bool is_zig_zig = (g->left_child == p && p->left_child == v) ||
                        (g->right_child == p && p->right_child == v);
      if (g == &dummy_root_) {
        Rotate(v);
      } else if (is_zig_zig) {
        Rotate(p);
        Rotate(v);
      } else {
        Rotate(v);
        Rotate(v);
      }
    }
  }

  static BaseNode* MaximumNode(BaseNode* v) {
    if (v->right_child == nullptr) {
      return v;
    }

    while (v->right_child != nullptr) {
      v = v->right_child;
    }

    return v;
  }

  std::pair<BaseNode*, bool> FindNode(int value) const {
    BaseNode* current = dummy_root_.left_child;
    BaseNode* last = nullptr;
    while (current != nullptr) {
      last = current;
      if (Value(current) == value) {
        return {current, true};
      }

      if (Value(current) < value) {
        current = current->right_child;
      } else {
        current = current->left_child;
      }
    }

    return {last, false};
  }
};

enum Tokens { Insert, Delete, Exists, Next, Prev, Kth };

Tokens Parse(const std::string& line) {
  if (line == "insert") {
    return Insert;
  }

  if (line == "delete") {
    return Delete;
  }

  if (line == "exists") {
    return Exists;
  }

  if (line == "next") {
    return Next;
  }

  if (line == "prev") {
    return Prev;
  }

  return Kth;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string command;
  int argument;
  Splay tree;
  while (std::cin >> command >> argument) {
    Tokens token = Parse(command);

    switch (token) {
      case Insert: {
        tree.Insert(argument);
      } break;

      case Delete: {
        tree.Delete(argument);
      } break;

      case Exists: {
        if (tree.Find(argument)) {
          std::cout << "true\n";
        } else {
          std::cout << "false\n";
        }
      } break;

      case Next: {
        std::cout << tree.Next(argument) << '\n';
      } break;

      case Prev: {
        std::cout << tree.Prev(argument) << '\n';
      } break;

      case Kth: {
        std::cout << tree.Kth(argument) << '\n';
      } break;
    }
  }
}
