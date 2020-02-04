#include "lab1.h"

template <class T>
class Class
{
private:
    T **students;
    int count;
    int capacity;

public:
    Class(int number)
    {
        students = new T *[number];
        count = 0;
        capacity = number;
    }
    void addStudent(T *student)
    {
        if (count >= capacity)
            throw out_of_range("Too many students in the class");
        students[count++] = student;
    }
    void takeAttendance()
    {
        // TODO: call signup on each student
        for (int i = 0; i < count; i++)
        {
            students[i]->signup();
        }
    }
    ~Class()
    {
        delete[] students;
    }
};

int main()
{
    Class<Graduate> advanced_class(10);
    Class<Undergraduate> basic_class(81);
    Class<Student> common_class(108);
    Graduate *graduate_students[12];
    Undergraduate *undergraduate_students[81];

    for (int i = 0; i < 12; ++i)
    {
        stringstream ss;
        ss << "B18037" << std::setfill('0') << setw(2) << i;
        graduate_students[i] = new Graduate(ss.str());
    }
    for (int i = 0; i < 81; ++i)
    {
        stringstream ss;
        ss << "F18037" << std::setfill('0') << setw(2) << i;
        undergraduate_students[i] = new Undergraduate(ss.str());
    }

    // Add graduate students to advanced_class
    for (int i = 0; i < 12; ++i)
    {
        advanced_class.addStudent(graduate_students[i]);
        common_class.addStudent(graduate_students[i]);
    }
    // TODO: Add undergraduate students for basic_class
    for (int i = 0; i < 81; i++)
    {
        basic_class.addStudent(undergraduate_students[i]);
        common_class.addStudent(undergraduate_students[i]);
    }
    // TODO: Add all graduate students and undergrudate students to common_class

    // TOOD: call takeAttendance on all three classes
    common_class.takeAttendance();
    basic_class.takeAttendance();
    advanced_class.takeAttendance();
    return 0;
}