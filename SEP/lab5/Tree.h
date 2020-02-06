#include <cassert>
#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

// Please do not imitate me.
// Wirte implementation code in header files is unreasonable.

class TreeNode
{
private:
    int x, y;

public:
    TreeNode *left;
    TreeNode *right;

public:
    TreeNode(int _x, int _y)
    {
        x = _x;
        y = _y;
        left = right = nullptr;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    double distance(int x0, int y0)
    {
        return sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
    }
};

class BinaryDimonTree
{
public:
    TreeNode *root;

public:
    BinaryDimonTree()
    {
        root = nullptr;
    }
    ~BinaryDimonTree()
    {
        freeHelper(root);
    }

    // post-order traverse to reclaim the tree
    void freeHelper(TreeNode *&p)
    {
        if (p == nullptr)
            return;
        freeHelper(p->left);
        freeHelper(p->right);
        // cout << p->getX() << p->getY() << endl;
        delete p;
    }

    void insert(int x, int y)
    {
        insertHelper(root, 0, x, y);
    }

    void insertHelper(TreeNode *&p, int level, int x, int y)
    {
        if (p == nullptr)
        {
            p = new TreeNode(x, y);
            return;
        }
        if (level % 2)
        {
            int domination = p->getY();
            if (y >= domination)
            {
                insertHelper(p->right, level + 1, x, y);
            }
            else
            {
                insertHelper(p->left, level + 1, x, y);
            }
        }
        else
        {
            int domination = p->getX();
            if (x >= domination)
            {
                insertHelper(p->right, level + 1, x, y);
            }
            else
            {
                insertHelper(p->left, level + 1, x, y);
            }
        }
    }

    TreeNode *findNearestNode(int x, int y)
    {
        TreeNode *guess = nullptr;
        double bestDist = __DBL_MAX__;
        findNearestHelper(root, guess, bestDist, x, y, 0);
        return guess;
    }

    void findNearestHelper(TreeNode *current, TreeNode *&guess, double &bestDist, int x, int y, int level)
    {
        if (current == nullptr)
        {
            return;
        }
        double currentDist = current->distance(x, y);
        if (currentDist < bestDist)
        {
            bestDist = currentDist;
            guess = current;
        }
        if (level % 2)
        {
            if (y < current->getY())
            {
                findNearestHelper(current->left, guess, bestDist, x, y, level + 1);
                if ((current->getY()) < bestDist)
                    findNearestHelper(current->right, guess, bestDist, x, y, level + 1);
            }
            else
            {
                findNearestHelper(current->right, guess, bestDist, x, y, level + 1);
                if ((y - current->getY()) < bestDist)
                    findNearestHelper(current->left, guess, bestDist, x, y, level + 1);
            }
        }
        else
        {
            if (x < current->getX())
            {
                findNearestHelper(current->left, guess, bestDist, x, y, level + 1);
                if ((current->getX() - x) < bestDist)
                    findNearestHelper(current->right, guess, bestDist, x, y, level + 1);
            }
            else
            {
                findNearestHelper(current->right, guess, bestDist, x, y, level + 1);
                if ((x - current->getX()) < bestDist)
                    findNearestHelper(current->left, guess, bestDist, x, y, level + 1);
            }
        }
    }

    // print nodes to console to check the 'insert' function
    // '#' represents the null nodes
    void levelTraverse()
    {
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty())
        {
            auto p = q.front();
            q.pop();
            if (p != nullptr)
            {
                printf("(%d, %d)\n", p->getX(), p->getY());
                q.push(p->left);
                q.push(p->right);
            }
            else
                printf("#\n");
        }
    }
};
