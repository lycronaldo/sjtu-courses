#pragma once

#include <string>
#include "ListNode.h"

using namespace std;

class ListBuffer
{
private:
    //TODO: add a List here for storing the input lines
    ListNode *head, *tail;
    int length;

public:
    ListBuffer();
    ~ListBuffer();

    void writeToFile(const string &filename) const;

    void showLines() const;
    void deleteLine(int line_idx);
    void insertLine(int line_idx, const string &text);
};
