#include "Tree.h"
int main()
{
    BinaryDimonTree tree;
    tree.insert(0, 0);
    tree.insert(-1, -2);
    tree.insert(3, 4);
    tree.insert(-2, 5);
    tree.insert(2, -2);
    tree.insert(3, 4);
    tree.levelTraverse();
}