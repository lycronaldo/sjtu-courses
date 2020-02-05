#include "termio.h"
#include "UserMovement.h"
#include <sstream>
// Do not delete the following 3 lines
char Termio::buffer[Termio::CANVAS_HEIGHT][Termio::CANVAS_WIDTH + 1];
int Termio::MAX_WIDTH;
int Termio::MAX_HEIGHT;

//gloabl variables
int disks = 3;
int totalStep = 0;
UserMovement userMovement;
Rod rods[3];

void write_hi()
{
    for (int i = 0; i < 5; i++)
    {
        Termio::buffer[i][0] = '|';
        Termio::buffer[i][5] = '|';
        Termio::buffer[i][9] = '|';
        Termio::buffer[i][13] = '*';
    }
    Termio::buffer[3][13] = ' ';
    Termio::buffer[1][9] = ' ';
    for (int i = 1; i < 5; i++)
    {
        Termio::buffer[2][i] = '-';
    }
}

void hanoi(int n, Rod &a, Rod &b, Rod &c)
{
    if (n == 0)
        return;
    hanoi(n - 1, a, c, b);
    c.PushDisk(a.PopDisk());

    Termio::PutDisks(rods[0], rods[1], rods[2]);
    Termio::Draw();
    cout << "Auto Mode:" << (&a - rods) + 1 << "->" << (&c - rods) + 1 << endl;
    Termio::GetLine();

    hanoi(n - 1, b, a, c);
}
void AutoMove()
{
    while (!userMovement.Empty())
    {
        Pair p = userMovement.PopMovement();
        int src = p.src;
        int dst = p.dst;
        rods[src].PushDisk(rods[dst].PopDisk());
        Termio::PutDisks(rods[0], rods[1], rods[2]);
        Termio::Draw();
        Termio::GetLine();
    }
    hanoi(disks, rods[0], rods[1], rods[2]);
    cout << "Auto solving completed." << endl;
    exit(0);
}
void DispatchCmd(string &cmd)
{
    try
    {
        int src, dst;
        stringstream ss(cmd);

        ss >> src >> dst;
        if (ss.eof() && src == 0 && dst == 0)
        {
            AutoMove();
        }
        src--, dst--;
        if (ss.eof() && CHECK_ROD_IDX(src) && CHECK_ROD_IDX(dst))
        {
            int k = rods[src].PopDisk();
            if (rods[dst].PushDisk(k))
            {
                userMovement.PushMovement(src, dst);
                totalStep++;
            }
            else
            {
                rods[src].PushDisk(k);
            }

            Termio::PutDisks(rods[0], rods[1], rods[2]);
            Termio::Draw();

            if (rods[0].GetLength() == 0 && rods[1].GetLength() == 0 && rods[2].GetLength() == disks)
            {
                cout << "Congratulation, you win!" << endl;
                cout << "Totally moved " << totalStep << " times." << endl;
                exit(0);
            }
        }
    }
    catch (const char *e)
    {
        cout << e << endl;
    }
}

int main()
{

    // ---- Example: how to use termio.h ----
    // clear screen
    Termio::Clear();
    // reset buffer
    Termio::ResetBuffer();
    write_hi();
    Termio::Draw();
    // ----------- End of example -----------
    // Your code here

    while (true)
    {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        cin >> disks;
        cin.ignore();
        if (DISKS_MIN <= disks && disks <= DISKS_MAX)
            break;
    }
    for (int i = disks; i >= 1; i--)
        rods[0].PushDisk(i);
    Termio::PutDisks(rods[0], rods[1], rods[2]);
    Termio::Draw();

    string cmd;
    while (1)
    {
        cout << "Move a disk. Format: x y" << endl;
        cout << "You have moved " << totalStep << " times." << endl;
        cout.flush();

        getline(cin, cmd);
        DispatchCmd(cmd);
    }

    return 0;
}
