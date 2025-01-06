#include <iostream>
#include <vector>
#include <string>
#include "event.h"
using namespace std;

class Tree
{
    Event *event;
    Tree *lchild;
    Tree *rchild;

    public:
    Tree(Event *e):event(e),lchild(nullptr),rchild(nullptr);
};