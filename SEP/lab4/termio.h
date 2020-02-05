#pragma once
/* some code from Libconio 1.0 (C) 2004 FSL A.C. */

#ifdef _WIN32
#include <conio.h>
#include <stdio.h>
#include <string.h>
static void clrscr()
{
    system("cls");
}
#else
#include <termios.h>
#include <stdio.h>
#include <cstring>
#include "global.h"
#include "Rod.h"
#define ESC 033 /* Escape char */

static int getch()
{
    struct termios t;
    int c;

    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO + ~ICANON;
    tcsetattr(0, TCSANOW, &t);
    fflush(stdout);
    c = getchar();
    t.c_lflag |= ICANON + ECHO;
    tcsetattr(0, TCSANOW, &t);
    return c;
}

static void clrscr()
{
    printf("%c[2J", ESC);
    printf("%c[%d;%dH", ESC, 0, 0);
}
#endif

#include <iostream>
using namespace std;

class Termio
{
public:
    static const int CANVAS_WIDTH = 64;
    static const int CANVAS_HEIGHT = 14;
    static int MAX_WIDTH, MAX_HEIGHT;
    static char buffer[CANVAS_HEIGHT][CANVAS_WIDTH + 1];

    // add by sinkinben
    static const int ROD_COLPOS1 = 5;
    static const int ROD_COLPOS2 = 20;
    static const int ROD_COLPOS3 = 35;

    static char GetChar()
    {
        return getch();
    }

    static void GetLine()
    {
        string s;
        getline(cin, s);
    }

    static void Clear()
    {
        clrscr();
    }

    static void Draw()
    {
        Clear();
        for (int i = 0; i < CANVAS_HEIGHT; i++)
        {
            buffer[i][CANVAS_WIDTH] = '\0';
            cout << buffer[i] << endl;
        }
    }

    static void ResetBuffer()
    {
        for (int i = 0; i < CANVAS_HEIGHT; i++)
            memset(buffer[i], ' ', CANVAS_WIDTH);
    }

    static int GetDiskWidth(int order)
    {
        return 2 * order + 1;
    }

    static void InitializeRod(Rod &r, int height, int whichRod)
    {
        int rlen = r.GetLength();
        int row = height - 2;
        for (int i = 0; i < rlen; i++)
        {
            int k = GetDiskWidth(r.diskStack[i]);
            buffer[row][whichRod] = '*';
            for (int j = 1; j <= ((k - 1) / 2); j++)
                buffer[row][whichRod - j] = buffer[row][whichRod + j] = '*';
            row -= 2;
        }
    }
    static void PutDisks(Rod &a, Rod &b, Rod &c)
    {
        ResetBuffer();
        buffer[1][ROD_COLPOS1] = buffer[1][ROD_COLPOS2] = buffer[1][ROD_COLPOS3] = '|';
        int alen = a.GetLength();
        int blen = b.GetLength();
        int clen = c.GetLength();

        int height = 3 + DISKS_MAX * 2;
        for (int i = 2; i < height; i++)
            buffer[i][ROD_COLPOS1] = buffer[i][ROD_COLPOS2] = buffer[i][ROD_COLPOS3] = '|';
        memset(buffer[height], '-', ROD_COLPOS3 + ROD_COLPOS1);
        buffer[height][ROD_COLPOS1] = buffer[height][ROD_COLPOS2] = buffer[height][ROD_COLPOS3] = '|';

        InitializeRod(a, height, ROD_COLPOS1);
        InitializeRod(b, height, ROD_COLPOS2);
        InitializeRod(c, height, ROD_COLPOS3);
    }
};
