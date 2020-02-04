#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>

using namespace std;

class Student
{
protected:
    string id;

public:
    Student(string _id = "-1")
    {
        id = _id;
    }
    virtual void signup() = 0;
};

class Graduate : public Student
{
public:
    Graduate(string _id) : Student(_id)
    {
    }
    void signup()
    {
        cout << "Graduate student " << id << endl;
    }
};

class Undergraduate : public Student
{
public:
    Undergraduate(string _id) : Student(_id)
    {
    }
    void signup()
    {
        cout << "Undergraduate student " << id << endl;
    }
};
