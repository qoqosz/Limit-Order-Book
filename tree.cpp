#include <map>
#include <iostream>
#include <chrono>
/*
 * https://gist.github.com/harish-r/097688ac7f48bcbadfa5
 **/

template <class T> class AVLTree 
{
  /**
   * Implementation taken from: 
   *  https://gist.github.com/harish-r/097688ac7f48bcbadfa5
   **/
  struct Node
  {
    T data;
    Node* left;
    Node* right;
    int height;
  };
  
  Node* root;

  void make_empty(Node* t)
  {
    if (t == NULL)
      return;

    make_empty(t->right);
    make_empty(t->left);
    delete t;
  }

  Node* insert(T x, Node* t)
  {
    if (t == NULL)
    {
      t = new Node;
      t->data = x;
      t->height = 0;
      t->left = t->right = NULL;
    }
    else if (x < t->data)
    {
      t->left = insert(x, t->left);

      if (height(t->left) - height(t->right) == 2)
      {
        if (x < t->left->data)
          t = single_right_rotate(t);
        else
          t = double_right_rotate(t);
      }
    }
    else if (x > t->data)
    {
      t->right = insert(x, t->right);

      if (height(t->left) - height(t->right) == -2)
      {
        if (x > t->right->data)
          t = single_left_rotate(t);
        else
          t = double_left_rotate(t);
      }
    }

    t->height = std::max(height(t->left), height(t->right)) + 1;

    return t;
  }
 
  Node* single_right_rotate(Node* &t)
  {
    Node* u = t->left;
    t->left = u->right;
    u->right = t;
    t->height = std::max(height(t->left), height(t->right)) + 1;
    u->height = std::max(height(u->left), t->height) + 1;

    return u;
  }

  Node* single_left_rotate(Node* &t)
  {
    Node* u = t->right;
    t->right = u->left;
    u->left = t;
    t->height = std::max(height(t->left), height(t->right)) + 1;
    u->height = std::max(height(t->right), t->height) + 1;

    return u;
  }

  Node* double_left_rotate(Node* &t)
  {
    t->right = single_right_rotate(t->right);

    return single_left_rotate(t);
  }

  Node* double_right_rotate(Node* &t)
  {
    t->left = single_left_rotate(t->left);

    return single_right_rotate(t);
  }

  Node* min(Node* t)
  {
    if (t == NULL)
      return NULL;
    else if (t->left == NULL)
      return t;
    else
      return min(t->left);
  }

  Node* max(Node* t)
  {
    if (t == NULL)
      return NULL;
    else if (t->right == NULL)
      return t;
    else
      return max(t->right);
  }

  Node* remove(T x, Node* t)
  {
    Node* tmp;

    if (t == NULL) // Element not found
      return NULL;

    // Searching for element
    else if (x < t->data)
      t->left = remove(x, t->left);
    else if (x > t->data)
      t->right = remove(x, t->right);

    // Element found
    else if (t->left && t->right)
    {
      tmp = min(t->right);
      t->data = tmp->data;
      t->right = remove(t->data, t->right);
    }
    else
    {
      tmp = t;

      if (t->left == NULL)
        t = t->right;
      else if (t->right == NULL)
        t = t->left;

      delete tmp;
    }

    if (t == NULL)
      return t;

    t->height = std::max(height(t->left), height(t->right)) + 1;

    // If node is unbalanced
    // If left node is deleted, right case
    if (height(t->left) - height(t->right) == 2)
    {
      // right right case
      if (height(t->left->left) - height(t->left->right) == 1)
        return single_left_rotate(t);

      return double_left_rotate(t);
    }
    else if (height(t->left) - height(t->right) == -2)
    {
      if (height(t->right->right) - height(t->right->left) == 1)
        return single_right_rotate(t);
      
      return double_right_rotate(t);
    }

    return t;
  }

  int height(Node* t)
  {
    return (t == NULL ? -1 : t->height);
  }

  int get_balance(Node* t)
  {
    if (t == NULL)
      return 0;
  
    return height(t->left) - height(t->right);
  }

  void inorder(Node* t)
  {
    if (t == NULL)
      return;

    inorder(t->left);
    std::cout << t->data << " ";
    inorder(t->right);
  }

public:
  AVLTree()
  {
    root = NULL;
  }

  ~AVLTree()
  {
    if (root != NULL)
      make_empty(root);
  }

  void insert(T x)
  {
    root = insert(x, root);
  }

  void remove(T x)
  {
    root = remove(x, root);
  }

  void display()
  {
    inorder(root);
    std::cout << "\n";
  }
};




int main() {
  AVLTree<int> t;

  auto t0 = std::chrono::steady_clock::now();

  t.insert(20);
  t.insert(25);
  t.insert(15);
  t.insert(10);
  t.insert(30);
  t.insert(5);
  t.insert(35);
  t.insert(67);
  t.insert(43);
  t.insert(21);
  t.insert(10);
  t.insert(89);
  t.insert(38);
  t.insert(69);
  t.remove(5);
  t.remove(35);
  t.remove(65);
  t.remove(89);
  t.remove(43);
  t.remove(88);
  t.remove(20);
  t.remove(38);

  // What would be the perf when plugged to python?

  auto t1 = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = t1 - t0;

  t.display();

  std::cout << diff.count() / 22.0 << '\n';

  // int a;
  // std::cin >> a;
}
