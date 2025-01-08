#include <iostream>
#include <vector>
#include <string>
#include "EventNode.h"
using namespace std;

class Tree
{
public:
    Event *root_event;
    Tree *lchild;
    Tree *rchild;

    Tree(Event *e);
    Tree();
    Tree(Event *e, Tree *lchild, Tree *child);
    ~Tree();
};