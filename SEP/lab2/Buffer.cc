#include <fstream>
#include <iostream>
#include <iomanip>
#include "Buffer.h"

Buffer::Buffer()
{
    currentLine = maxLine = 0;
}
Buffer::~Buffer()
{
}
void Buffer::moveToPrevLine()
{
    if (currentLine <= 1)
        throw out_of_range("Line number out of range");
    currentLine--;
}
string Buffer::moveToLine(int lineNum)
{
    if (lineNum <= 0 || lineNum > maxLine)
        throw out_of_range("Line number out of range");
    currentLine = lineNum;
    return lines[lineNum];
}

void Buffer::writeToFile(const string &filename) const
{
    // TODO: write lines to the file and output the number of bytes written.
    ofstream file(filename);
    int bytes = 0;
    for (int i = 1; i <= maxLine; i++)
    {
        file << lines[i] << endl;
        bytes += lines[i].size();
    }
    file.close();
    cout << bytes << " bytes are written." << endl;
}

void Buffer::showLines(int start, int end) const
{
    // TODO: print lines according to the requirement.
    start = max(1, start);
    end = min(end, maxLine);
    for (int i = start; i <= end; i++)
    {
        cout << left << setfill(' ') << setw(6) << i << lines[i] << endl;
    }
}
void Buffer::deleteLines(int start, int end)
{
    if (start <= 0 || end > maxLine || start > end)
        throw range_error("Delete range error");
    for (int i = end + 1; i <= maxLine; ++i)
    {
        lines[i - end - 1 + start] = lines[i];
    }
    maxLine -= end - start + 1;
}

void Buffer::insertLine(const string &text)
{
    // TODO: insert a line to current line.
    if (currentLine >= 1)
        currentLine--;
    appendLine(text);
}

void Buffer::appendLine(const string &text)
{
    if (maxLine >= MAX_LINE)
        throw out_of_range("move beyond the file line limit");
    for (int i = maxLine; i > currentLine; i--)
    {
        lines[i + 1] = lines[i];
    }
    lines[currentLine + 1] = text;
    currentLine++;
    maxLine++;
}