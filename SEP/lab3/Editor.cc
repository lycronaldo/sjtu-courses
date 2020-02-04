#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new ListBuffer();
}

Editor::~Editor()
{
    delete buffer;
    buffer = nullptr;
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try
        {
            dispatchCmd(cmd);
        }
        catch (const char *e)
        {
            cout << "? " << e << endl;
        }
        catch (const out_of_range &oor)
        {
            cout << "? " << oor.what() << endl;
        }
        catch (const range_error &re)
        {
            cout << "? " << re.what() << endl;
        }
    }
}

void Editor::dispatchCmd(const string &cmd)
{
    /*
    TODO: your code here.
          cmd-1: non-negative integer + statement (e.g., 100 let a = b + c)
          cmd-2: delete (e.g., d 100)
          cmd-3: list   (e.g., lisT) 
          cmd-4: save as a file (e.g., save code.sh)
    */
    if (toUpper(cmd) == "LIST")
    {
        buffer->showLines();
    }
    else if (cmd[0] == 'd')
    {
        stringstream ss(cmd);
        char op;
        int lineIdx;
        ss >> op >> lineIdx;
        if (ss.eof())
        {
            buffer->deleteLine(lineIdx);
        }
    }
    else if (cmd[0] == 's')
    {
        stringstream ss(cmd);
        string op;
        ss >> op;
        if (ss.good() && op == "save")
        {
            buffer->writeToFile(cmd.substr(5));
        }
    }
    else if ('0' < cmd[0] && cmd[0] <= '9')
    {
        int number;
        stringstream ss(cmd);
        ss >> number;
        int len = (int)log10(number) + 1;
        if (ss.good())
        {
            buffer->insertLine(number, cmd.substr(len));
        }
    }
    else
    {
        throw "Unknown command";
    }
}

//TODO: add other code you want
string Editor::toUpper(const string &text)
{
    string s = "";
    for (auto x : text)
    {
        if ('a' <= x && x <= 'z')
            x = x - 'a' + 'A';
        s.append(1, x);
    }
    return s;
}