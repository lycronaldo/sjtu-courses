#include "global.h"

using namespace std;

class Rod
{
public:
    int *diskStack;
    int top;
    Rod(/* args */);
    ~Rod();
    bool PushDisk(int diskOrder);
    int PopDisk();
    int GetLength();
};
