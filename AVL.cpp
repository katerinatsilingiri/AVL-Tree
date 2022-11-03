#include "AVL.hpp"
#include <algorithm>
#include <stdio.h>
#include <string.h>


//Node constructor: create a new Node
AVL::Node::Node(const string &e, Node *parent, Node *left, Node *right) {
    this->element = e;
    this->parent = parent;
    this->left = left;
    this->right = right;
    height = 1;
} 

AVL::Node *AVL::Node::getParent() const {
    return(parent);
}

AVL::Node *AVL::Node::getLeft() const {
    return(left);
}

AVL::Node *AVL::Node::getRight() const {
    return(right);
}

string AVL::Node::getElement() const {
    return(element);
}

int AVL::Node::getHeight() const {
    return(height);
}

void AVL::Node::setLeft(Node *newLeft) {
    left = newLeft;
}

void AVL::Node::setRight(Node *newRight) {
    right = newRight;
}

void AVL::Node::setParent(Node *newParent) {
    parent = newParent;
}

void AVL::Node::setElement(string newElement) {
    element = newElement;
}

bool AVL::Node::isLeft() const {

    if (parent == nullptr) {
        return(false);
    }

    if (parent->getLeft() == this) {
        return(true);
    }

    return(false);

}

bool AVL::Node::isRight() const {

    if (parent == nullptr) {
        return(false);
    }

    if (parent->getRight() == this) {
        return(true);
    }

    return(false);

}

int AVL::Node::rightChildHeight() const {

    if (right == nullptr) {
        return(0);
    }

    return(right->getHeight());
}

int AVL::Node::leftChildHeight() const {

    if (left == nullptr) {
        return(0);
    }

    return(left->getHeight());
}

int AVL::Node::updateHeight() {
    
    int newHeight = max(leftChildHeight(), rightChildHeight()) + 1;
    height = newHeight;
    return(height);
}

bool AVL::Node::isBalanced() {
    int balance = leftChildHeight() - rightChildHeight();
    return((-1 <= balance) && (balance <= 1));
}
             

//Default constructor for the AVL tree
AVL::AVL() {
    size = 0;
    root = nullptr;
 
}

//Copy constructor for the AVL tree (copies the "tree")
AVL::AVL(const AVL &tree){
    Iterator start, end;
    start = tree.begin();
    end = tree.end();

    //iterate through all the nodes of the "tree" and add them at the current tree
    while (start != end) {
        this->add(*start);
        start++;
    }

}

//returns the hightest node after the rotation 
AVL::Node *AVL::reconstructTree(AVL::Node *v, AVL::Node *w, AVL::Node *u) {
    
    //simple right rotation
    if (w->isLeft() && u->isLeft()) {
        if (v != root) {
            if (v->isLeft()) {
                v->getParent()->setLeft(w);
            }
            else {
                v->getParent()->setRight(w);
            }
            w->setParent(v->getParent());
        }
        v->setLeft(w->getRight());
        if (w->getRight() != nullptr) {
            w->getRight()->setParent(v);
        }
        w->setRight(v);
        v->setParent(w);
        if (v == root) {
            root = w;
            w->setParent(nullptr);
        }
        return(w);
    } 

    //simple left rotation 
    if (w->isRight() && u->isRight()) {
        if (v != root) {
            if (v->isRight()) {
                v->getParent()->setRight(w);
            }
            else {
                v->getParent()->setLeft(w);
            }
            w->setParent(v->getParent());
        }
        v->setRight(w->getLeft());
        if (w->getLeft() != nullptr) {
            w->getLeft()->setParent(v);
        }
        w->setLeft(v);
        v->setParent(w);
        if (v == root) {
            root = w;
            w->setParent(nullptr);
        }
        return(w);
    }

    //double left rotation
    if (u->isLeft()) {
        v->setRight(u->getLeft());
        if (u->getLeft() != nullptr) {
            u->getLeft()->setParent(v);
        }
        w->setLeft(u->getRight());
        if (u->getRight() != nullptr) {
            u->getRight()->setParent(w);
        }
        if (v != root) {
            if (v->isRight()) {
                v->getParent()->setRight(u);
            }
            else {
                v->getParent()->setLeft(u);
            }
            u->setParent(v->getParent());
        }
        v->setParent(u);
        w->setParent(u);
        u->setLeft(v);
        u->setRight(w);
        if (v == root) {
            root = u;
            u->setParent(nullptr);
        }
        return(u);
    }
   
    //double right rotation 
    v->setLeft(u->getRight());
    if (u->getRight() != nullptr) {
        u->getRight()->setParent(v);
    }
    w->setRight(u->getLeft());
    if (u->getLeft() != nullptr) {
        u->getLeft()->setParent(w);
    }
    if (v != root) {
        if (v->isLeft()) {
            v->getParent()->setLeft(u);
        }
        else {
            v->getParent()->setRight(u);
        }
        u->setParent(v->getParent());
    }
    v->setParent(u);
    w->setParent(u);
    u->setLeft(w);
    u->setRight(v);
    if (v == root) {
        root = u;
        u->setParent(nullptr);
    }
    return(u);
    
}


AVL::Node *AVL::rebalanceChild(AVL::Node *v) {

    if (v->leftChildHeight() > v->rightChildHeight()) {
        return(v->getLeft());
    }
    else if (v->leftChildHeight() < v->rightChildHeight()) {
        return(v->getRight());
    }
    //if both children have the same height
    else if (v->isLeft()) {
        return(v->getLeft());
    }
    else {
        return(v->getRight());
    }

}

//rebalance the tree (starting from Node v and going up)
void AVL::rebalanceTree(AVL::Node *v) {

    AVL::Node *u;
    AVL::Node *w;

    while (v != nullptr) {
       
        v->updateHeight();
      
        if (!v->isBalanced()) {
            w = rebalanceChild(v);
            u = rebalanceChild(w);
            v = reconstructTree(v,w,u);

            v->getLeft()->updateHeight();
            v->getRight()->updateHeight();
            v->updateHeight();
        }
        v = v->getParent();
    }

}

//recursive function used in contains and add
//if the "strToSearch" exists in the tree, the function returns it
//if the "strToSearch" doesn't exist in the tree, returns the node where it should be
AVL::Node *AVL::searchNode(string strToSearch, AVL::Node *node) {

    if (strToSearch.compare(node->getElement().c_str()) < 0) {
        if (node->getLeft() == nullptr) {
            return(node);
        }
        else {
            return(searchNode(strToSearch, node->getLeft()));
        }
    }
    else if (strToSearch.compare(node->getElement().c_str()) == 0) {
        return(node);
    }
    else {
        if (node->getRight() == nullptr) {
            return(node);
        } 
        else {
            return(searchNode(strToSearch, node->getRight()));
        }
    }

}

//function used in add
AVL::Node *AVL::insertNode(string strToAdd ) {
    
    //if the tree is empty, create a new Node and add it as root
    if (root == nullptr) {
        AVL::Node *newNode = new AVL::Node(strToAdd, nullptr, nullptr, nullptr);
        root = newNode;
        return(root);
    }

    //using function searchNode to check if the "strToAdd" aldready exists in the tree
    //if not, the newNode will be the child of the helpNode (left or right, depending on the name)
    AVL::Node *helpNode = searchNode(strToAdd, root);

    if (helpNode->getElement().compare(strToAdd) == 0) {
        return(nullptr);
    }
    else {
        if (strToAdd.compare(helpNode->getElement().c_str()) < 0) {
            AVL::Node *newNode = new AVL::Node(strToAdd, helpNode, nullptr, nullptr);
            helpNode->setLeft(newNode);
            return(helpNode->getLeft());
        }
        else {
            AVL::Node *newNode = new AVL::Node(strToAdd, helpNode, nullptr, nullptr);
            helpNode->setRight(newNode);
            return(helpNode->getRight());
        }
    }

}

//function used in rmv and deletes the specific "node"
void AVL::removeThisNode(AVL::Node *node) {

    //if the "node" is a leaf just delete it
    if ((node->getLeft() != nullptr) && (node->getRight() != nullptr)) {
        return;
    }

    //if the "node" is the root of the tree
    if (node == root) {
        AVL::Node *childOfNode = nullptr;
        if (node->getLeft() != nullptr) {
            childOfNode = node->getLeft();
        }
        else {
            childOfNode = node->getRight();
        }
        root = childOfNode;
        if (root != nullptr) {
            root->setParent(nullptr);
        }
      
    }
    else {
        AVL::Node *parentOfNode = node->getParent();
        AVL::Node *childOfNode = nullptr;
       if (node->getLeft() != nullptr) {
            childOfNode = node->getLeft();
        }
        else {
            childOfNode = node->getRight();
        }
        if (node->isLeft()) {
            parentOfNode->setLeft(childOfNode);
        }
        else {
            parentOfNode->setRight(childOfNode);
        }

        if (childOfNode != nullptr) {
            childOfNode->setParent(parentOfNode);
        }

    }

    
    node->setLeft(nullptr);
    node->setRight(nullptr);
    node->setParent(nullptr);
    return;

}

//used in rmv
AVL::Node *AVL::deleteNode(string strToRemove) {

    if (size == 0) {
        return(nullptr);
    }

    //using function searcNode to check if the "strToRemove" exists in the tree
    //if it doesn't, no operation is made
    //if it does, helpNode is the node that must be deleted
    AVL::Node *helpNode = searchNode(strToRemove, root);

    if (helpNode->getElement().compare(strToRemove)!=0) {
        return(nullptr);
    }
    else {
        AVL::Node *returnNode;
        if(helpNode->getLeft() == nullptr || helpNode->getRight() == nullptr){
            if (helpNode->getLeft() != nullptr) {
                returnNode = helpNode->getLeft();
            }
            else {
                if (helpNode->getRight() != nullptr) {
                    returnNode = helpNode->getRight();
                }
                else {
                    returnNode = helpNode->getParent();
                }
            }
			removeThisNode(helpNode);
            return(returnNode);		
		}
        else {
            AVL::Node *iterate = helpNode->getRight();
            AVL::Node *temp;
            AVL::Node *parentOfHelpNode;
            
            temp = iterate->getLeft();

            while (temp != nullptr) {
                iterate = temp;
                temp = iterate->getLeft();
            }

            AVL::Node *change;
            change = iterate;
            iterate = helpNode;
            helpNode = change;

            parentOfHelpNode = iterate->getParent();
            removeThisNode(iterate);
            return(parentOfHelpNode);
        }
    }
}

bool AVL::contains(string strToCheck) {

    //searchNode will return the "strToCheck" node or the last node of the search (the node where
    //the "strToCheck" should be if it was in the tree)
    AVL::Node *checkNode = searchNode(strToCheck, root);
    
    if (checkNode->getElement().compare(strToCheck) == 0) {
        return(true);
    } 

    return(false);


}

bool AVL::add(string elementToAdd) {
  
    AVL::Node *nodeToInsert = insertNode(elementToAdd);

    //if the function "insertNode" returns null, the insert of the node "elementToAdd" 
    //was not successful
    if (nodeToInsert == nullptr) {
        return(false);
    }

    rebalanceTree(nodeToInsert);
    size++;

    return(true);

}

bool AVL::rmv(string elementToRmv) {

    AVL::Node *NodeToDelete = deleteNode(elementToRmv);

    //if the function "deleteNode" returns null, the removal of the node "elementToRmv" 
    //was not successful
    if (NodeToDelete == nullptr) {
        return true;
    }

    rebalanceTree(NodeToDelete);
    size--;

    return false;

}

void AVL::print2DotFile(char *filename) {


} 

void AVL::pre_order(std::ostream& out) {
 
    AVL::Iterator curr, end;
    curr = this->begin();
    end = this->end();

    while (curr != end) {
        out << *curr << " ";
        curr++;
    }

}

//used to get access when deleting the elements of the first tree 
//for the operator=
AVL::Node *AVL::Iterator::returnCurr() {
    return(curr);

}

AVL& AVL::operator=(const AVL& avl) {

    //remove the nodes from the left tree
    AVL::Iterator start, end;
    
    start = this->begin();
    end = this->end();
    string removeElement;

    while (start != end) {
        this->rmv(start.returnCurr()->getElement());
        start++;
    }

    this->size = 0; 
    this->root = nullptr;


    //Iterator for the right tree to traverse it 
    start = avl.begin();
    end = avl.end();

    while(start != end) {
        this->add(*start);
        start++;
    }

    return(*this);

}

AVL AVL::operator+(const AVL& avl) {
    //new tree
    AVL newTree = AVL();

    //traverse first the left tree
    AVL::Iterator curr, end;
    curr = this->begin();
    end = this->end();

    while (curr != end) {
        newTree.add(*curr);
        curr++;
    }

    //traverse the right tree
    curr = avl.begin();
    end = avl.end();

    while (curr != end) {
        newTree.add(*curr);
        curr++;
    }
    
    return(newTree);
}
  
AVL& AVL::operator+=(const AVL& avl) {
    //traverse the right tree
    AVL::Iterator curr, end;
    curr = avl.begin();
    end = avl.end();

    while (curr != end) {
        this->add(*curr);
        curr++;
    }

    return(*this);

}

AVL& AVL::operator+=(const string& e) {
    this->add(e);
    return(*this);
}

AVL& AVL::operator-=(const string& e) {
    this->rmv(e);
    return(*this);
}

AVL AVL::operator+(const string& e) {
    //create new tree
    AVL newTree = AVL();

    //traverse this tree
    Iterator curr, end;
    curr = this->begin();
    end = this->end();

    while (curr != end) {
        newTree.add(*curr);
        curr++;
    }

    //add the e
    newTree.add(e);

    return(newTree);
}
  
AVL AVL::operator-(const string& e) {
   //create new tree
    AVL newTree = AVL();

    //traverse this tree
    Iterator curr, end;
    curr = this->begin();
    end = this->end();

    while (curr != end) {
        newTree.add(*curr);
        curr++;
    }

    //remove the e 
    newTree.rmv(e);

    return(newTree);

}

AVL::Iterator::Iterator() {
    curr = nullptr;
}

AVL::Iterator::Iterator(AVL::Node *root) {
    //create stack 
    curr = nullptr;
    if (root != nullptr) {
       stackNode.push(root); 
       traverse();
    }
}

AVL::Iterator AVL::begin() const {
    Iterator it(root);
    return(it);
}

AVL::Iterator AVL::end() const {
    Iterator it;
    return(it);
}

void AVL::Iterator::traverse() {

    if (stackNode.empty() == false) {
        curr = stackNode.top();
        stackNode.pop();
        if (this->curr == nullptr) {
            return;
        }
        if (curr->getRight() != nullptr) {
            stackNode.push(curr->getRight());
        }

        if (curr->getLeft() != nullptr) {
            stackNode.push(curr->getLeft());
        }
        
    }
    else {
        curr = nullptr;
    }

}

AVL::Iterator& AVL::Iterator::operator++() {

    traverse();
    return(*this);

}

AVL::Iterator AVL::Iterator::operator++(int a) {
    AVL::Iterator returnIter(*this);
    traverse();
    return(returnIter);

}

string AVL::Iterator::operator*() {
    return(curr->getElement());
}

bool AVL::Iterator::operator!=(AVL::Iterator it) {

    if (curr != it.curr) {
        return(true);
    }

    return(false);
}

bool AVL::Iterator::operator==(AVL::Iterator it) {

     if (curr == it.curr) {
        return(true);
    }
    return(false);

}

std::ostream& operator<<(std::ostream& out, const AVL& tree) {
    
    AVL::Iterator start, end, current;
    start= tree.begin();
    end = tree.end();

    for (current = start; current != end; current++) {
        out << *current << " ";
    }

    return(out);

}
