#include "Rod.h"
Rod::Rod()
{
    diskStack = new int[DISKS_MAX + 1];
    top = 0;
}

Rod::~Rod()
{
    delete[] diskStack;
}

bool Rod::PushDisk(int order)
{
    if (top == 0 || diskStack[top - 1] > order)
    {
        return diskStack[top++] = order;
    }
    return false;
}

int Rod::PopDisk()
{
    if (top <= 0)
        throw "Rod stack is empty.";
    return diskStack[--top];
}

int Rod::GetLength()
{
    return top;
}
