#include <cmath>
#include "global.h"
class Pair
{
public:
    int src, dst;
    Pair(int s = -1, int d = -1)
    {
        src = s;
        dst = d;
    }
};

class UserMovement
{
private:
    Pair *pairStack;
    int top;
    static const int MAX_SIZE = (2 << DISKS_MAX);

public:
    UserMovement();
    ~UserMovement();
    void PushMovement(int src, int dst);
    Pair PopMovement();
    bool Empty();
};

UserMovement::UserMovement()
{
    pairStack = new Pair[MAX_SIZE];
    top = 0;
}

UserMovement::~UserMovement()
{
    delete[] pairStack;
}

void UserMovement::PushMovement(int src, int dst)
{
    pairStack[top].src = src;
    pairStack[top].dst = dst;
    top++;
}

Pair UserMovement::PopMovement()
{
    if (top <= 0)
        throw "UserMovement stack is empty.";
    return pairStack[--top];
}

bool UserMovement::Empty()
{
    return top == 0;
}