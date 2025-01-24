#include <iostream>
#include <queue>
#include <string>
#include <climits>
#include <sstream>

using namespace std;

class RBTree;
class RBTreeNode{
private:
    RBTreeNode *leftchild;
    RBTreeNode *rightchild;
    RBTreeNode *parent;
    int key;
    bool color; //black = 0, red = 1
public:
    RBTreeNode(int key);
    RBTreeNode();
    friend class RBTree;
};


class RBTree{
private:
    RBTreeNode *root;
public:
    static RBTreeNode *NIL;
    RBTree():root(){
        NIL = new RBTreeNode();
        root = NIL;
    };
    bool isNil(RBTreeNode *x) {
        return x == NIL;
    }
    void left_rotate(RBTreeNode *i){
        RBTreeNode *y = i->rightchild;
        i->rightchild = y->leftchild;
        if(y->leftchild != NIL)
            y->leftchild->parent = i;
        y->parent = i->parent;
        if(i->parent == NIL)
            root = y;
        else if(i == i->parent->leftchild)
            i->parent->leftchild = y;
        else
            i->parent->rightchild = y;
        y->leftchild = i;
        i->parent = y;
    }
    void right_rotate(RBTreeNode *i){
        RBTreeNode *y = i->leftchild;
        i->leftchild = y->rightchild;
        if(y->rightchild != NIL)
            y->rightchild->parent = i;
        y->parent = i->parent;
        if(i->parent == NIL)
            root = y;
        else if(i == i->parent->rightchild)
            i->parent->rightchild = y;
        else
            i->parent->leftchild = y;
        y->rightchild = i;
        i->parent = y;
    }
    void insert(RBTreeNode *i) {
        RBTreeNode *y = NIL;
        RBTreeNode *x = root;
        while (x != NIL) {
            y = x;
            if (i->key < x->key)
                x = x->leftchild;
            else
                x = x->rightchild;
        }
        i->parent = y;
        if (y == NIL)
            root = i;
        else if (i->key < y->key)
            y->leftchild = i;
        else
            y->rightchild = i;
        i->leftchild = NIL;
        i->rightchild = NIL;
        i->color = true;
        RB_Insert_Fixup(i);
    }

    void RB_Insert_Fixup(RBTreeNode *i){
        while(i->parent->color == true){
            if(i->parent == i->parent->parent->leftchild){
                RBTreeNode *y = i->parent->parent->rightchild;
                if(y->color == true){
                    i->parent->color = false;
                    y->color = false;
                    i->parent->parent->color = true;
                    i = i->parent->parent;
                }
                else{
                    if(i == i->parent->rightchild){
                        i = i->parent;
                        left_rotate(i);
                    }
                    i->parent->color = false;
                    i->parent->parent->color = true;
                    right_rotate(i->parent->parent);
                }
            }
            else{
                RBTreeNode *y = i->parent->parent->leftchild;
                if(y->color == true){
                    i->parent->color = false;
                    y->color = false;
                    i->parent->parent->color = true;
                    i = i->parent->parent;
                }
                else{
                    if(i == i->parent->leftchild){
                        i = i->parent;
                        right_rotate(i);
                    }
                    i->parent->color = false;
                    i->parent->parent->color = true;
                    left_rotate(i->parent->parent);
                }
            }
        }
        root->color = false;
    }
    void RB_Transplant(RBTreeNode *u, RBTreeNode *v){
        if(u->parent == NIL)
            root = v;
        else if(u == u->parent->leftchild)
            u->parent->leftchild = v;
        else
            u->parent->rightchild = v;
        v->parent = u->parent;
    }
    RBTreeNode Tree_Minimum(RBTreeNode *x){
        while(x->leftchild != NIL)
            x = x->leftchild;
        return *x;
    }
    void preorder_traversal(RBTreeNode *x){
        if(isNil(x) == false){
            if(x == root)
                cout << x->key;
            else
                cout << " " << x->key;
            preorder_traversal(x->leftchild);
            preorder_traversal(x->rightchild);
        }
    }
    RBTreeNode* get_root(){
        return root;
    }
    int findMax(RBTreeNode *x, int& res)
    {
        if (x == NIL)
            return 0;

        int l = findMax(x->leftchild, res);
        int r = findMax(x->rightchild, res);
        int max_single = max(max(l, r) + x->key, x->key);
        int max_top = max(max_single, l + r + x->key);
        res = max(res, max_top);
        return max_single;
    }
    
    // Returns maximum path sum in tree with given root
    int findMaxSum(RBTreeNode *x)
    {
        int res = INT_MIN;
        findMax(x, res);
        return res;
    }
};

RBTreeNode* RBTree::NIL = new RBTreeNode();
RBTreeNode::RBTreeNode(int key):key(key), leftchild(RBTree::NIL), rightchild(RBTree::NIL), parent(RBTree::NIL), color(true) {};
RBTreeNode::RBTreeNode():leftchild(RBTree::NIL), rightchild(RBTree::NIL), parent(RBTree::NIL), color(false) {};

int main() {
    RBTree T;
    string input;
    getline(cin, input);
    stringstream ss(input);
    int num;
    while(ss >> num){
        RBTreeNode *node = new RBTreeNode(num);
        T.insert(node);
    }

    T.preorder_traversal(T.get_root());
    cout << endl;

    int max = T.findMaxSum(T.get_root());
    cout << max << endl;

    return 0;
}