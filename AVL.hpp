#ifndef __AVL_HPP_
#define __AVL_HPP_

#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

class AVL {
private:
  class Node {
    Node *parent, *left, *right;
    int height;
    string element;
  
  public:
    Node(const string& e, Node *parent, Node *left, Node *right);
    Node*  getParent() const;
    Node*  getLeft() const;
    Node*  getRight() const;
    string getElement() const;
    int    getHeight() const;
    
    void setLeft(Node *);
    void setRight(Node *);
    void setParent(Node *);
    void setElement(string e);
    
    bool isLeft() const;
    bool isRight() const;
    int  rightChildHeight() const;
    int  leftChildHeight() const;
    int  updateHeight();
    bool isBalanced();
}; private:

    int size;
    Node* root;

public:
    
    class Iterator {
    
    public:
      Iterator();
      Iterator(Node *);
      Iterator& operator++();
      Iterator operator++(int a);
      string operator*();
      bool operator!=(Iterator it);
      bool operator==(Iterator it);
      AVL::Node *returnCurr();

      void traverse();

      private:
      Node *curr;
      stack<Node *> stackNode;

    };

  Iterator begin() const;
  Iterator end() const;
  
  static const int MAX_HEIGHT_DIFF = 1;
  AVL();
  AVL(const AVL& );
  bool contains(string e);
  bool add(string e);
  bool rmv(string e);
  void print2DotFile(char *filename);
  void pre_order(std::ostream& out);


  AVL::Node* searchNode(string str, Node *node);
  AVL::Node* insertNode(string str);
  void rebalanceTree(Node *v);
  AVL::Node* rebalanceChild(Node *v);
  AVL::Node *reconstructTree(Node *v, Node *w, Node *u);
  AVL::Node *deleteNode(string str);
  void printPreorder(AVL* tree, Node* node, std::ostream& out);
  void removeThisNode(Node *nodeToDel);

  
  friend std::ostream& operator<<(std::ostream& out, const AVL& tree);
  AVL& operator  =(const AVL& avl);
  AVL  operator  +(const AVL& avl);
  AVL& operator +=(const AVL& avl);
  AVL& operator +=(const string& e);
  AVL& operator -=(const string& e);
  AVL  operator  +(const string& e);
  AVL  operator  -(const string& e);
}; 

#endif