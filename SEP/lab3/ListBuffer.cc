#include <fstream>
#include <iostream>
#include <iomanip>
#include "ListBuffer.h"

//TODO: your code here
//implement the functions in ListBuffer

ListBuffer::ListBuffer()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

ListBuffer::~ListBuffer()
{
    auto p = head;
    while (p)
    {
        p = p->next;
        delete head;
    }
    head = tail = nullptr;
}

void ListBuffer::showLines() const
{
    auto p = head;
    while (p != nullptr)
    {
        cout << left << setfill(' ') << setw(6) << p->lineNumber << p->statement << endl;
        p = p->next;
    }
    cout << "----------------" << endl;
}

void ListBuffer::deleteLine(int lineIdx)
{
    if (length == 0)
        return;
    if (head->lineNumber == lineIdx)
    {
        auto p = head;
        head = head->next;
        delete p;
        length--;
        return;
    }
    auto pre = head, cur = head->next;
    while (cur != nullptr)
    {
        if (cur->lineNumber == lineIdx)
        {
            if (cur == tail)
                tail = pre;
            pre->next = cur->next;
            delete cur;
            length--;
            break;
        }
        pre = cur;
        cur = cur->next;
    }
}

void ListBuffer::insertLine(int lineIdx, const string &text)
{
    if (length == 0)
    {
        head = new ListNode(lineIdx, text);
        tail = head;
        length++;
        return;
    }
    if (lineIdx < head->lineNumber)
    {
        ListNode *q = new ListNode(lineIdx, text);
        q->next = head;
        head = q;
        length++;
        return;
    }
    auto p = head;
    while (p != nullptr)
    {
        if (p->lineNumber == lineIdx)
        {
            p->statement = text;
        }
        else if (p->lineNumber < lineIdx)
        {
            if (p->next && p->next->lineNumber > lineIdx)
            {
                ListNode *q = new ListNode(lineIdx, text);
                q->next = p->next;
                p->next = q;
                length++;
            }
            else if (p == tail)
            {
                p->next = new ListNode(lineIdx, text);
                tail = p->next;
                length++;
            }
        }
        p = p->next;
    }
}

void ListBuffer::writeToFile(const string &filename) const
{
    ofstream file(filename);
    auto p = head;
    while (p != nullptr)
    {
        file << p->statement << endl;
        p = p->next;
    }
    file.close();
}
