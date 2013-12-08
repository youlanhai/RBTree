// RBTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RBTree.h"

#include <iostream>

void traverse_rbtree(int v)
{
    std::cout << v << " ";
}

void traverse_rbtree_debug(rbtree::Node<int> *p)
{
    std::cout <<"("<< p->value << ", " << p->color<<") ";
}

int _tmain(int argc, _TCHAR* argv[])
{
    rbtree::RBTree<int> tree;

    tree.insert(11);
    tree.insert(2);
    tree.insert(14);
    tree.insert(1);
    tree.insert(7);
    tree.insert(16);
    tree.insert(5);
    tree.insert(8);
    tree.insert(4);
    tree.insert(15);

    std::cout << "depth:" << tree.maxDepth() << std::endl;

    tree.traverseDebug(traverse_rbtree_debug);
    std::cout << std::endl;

	return 0;
}

