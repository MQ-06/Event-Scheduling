#include "tree.h"

Tree::Tree(Event *e)
    : root_event(e), lchild(nullptr), rchild(nullptr) {}

Tree::Tree()
    : root_event(nullptr), lchild(nullptr), rchild(nullptr) {}

Tree::Tree(Event *e, Tree *left, Tree *right)
    : root_event(e), lchild(left), rchild(right) {}

Tree::~Tree()
{
    delete root_event;
    root_event = nullptr;
    lchild = nullptr;
    rchild = nullptr;
}
