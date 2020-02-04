#include "ListNode.h"

ListNode::ListNode(int lineNumber, const string &statement)
{
    this->lineNumber = lineNumber;
    this->statement = statement;
    this->next = nullptr;
}

ListNode::~ListNode()
{
}