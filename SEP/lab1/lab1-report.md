## lab1 report

### 1. 代码实现
复习了一下CPP中抽象类和继承的用法。三个要求实现的类，如下：
```cpp
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
```

### 2. 问题回答
+ 怎么触发这个 `exception` ?
  ```cpp
    void addStudent(T *student)
    {
        if (count >= capacity)
            throw out_of_range("Too many students in the class");
        students[count++] = student;
    }
  ```
  把声明 `Class` 时候的 `capacity` 改为小于添加的人数 `12`：
  ```cpp
  Class<Graduate> advanced_class(10);
  ```

  运行结果：
  ```bash
  sin@ubuntu:~/workspace/sjtu-courses/SEP$ g++ lab1.cc -o a.exe && ./a.exe 
  terminate called after throwing an instance of 'std::out_of_range'
  what():  Too many students in the class
  ```

+ 简述在生生成学生生信息时所涉及到的stringstream的各种用用法。  
  略。