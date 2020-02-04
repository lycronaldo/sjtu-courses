#pragma once
#include <string>

using namespace std;

class ListNode
{
public:
    int lineNumber;
    string statement;
    ListNode *next;

    ListNode(int lineNumber, const string &statement);
    ~ListNode();
};