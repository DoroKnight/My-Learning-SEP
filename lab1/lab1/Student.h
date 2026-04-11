#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    std::string name;
    std::string year;
    Degree degree;
protected:
    std::vector<Class*> classes;
public:
    const std::string id;
    std::string toString() const;
    Student(const std::string& ID, const std::string& name, const std::string& year, const std::string degree):id(ID){
        this->name = name;
        this->year = year;
        if(degree == "U") this->degree = undergraduate;
        else if(degree == "G") this->degree = graduate;
    }
    virtual ~Student() = default;
    virtual double getGpa() = 0;
    virtual double getAvgScore() = 0;
    void addClasses(Class* c);
};

// TODO: implement class Graduate.
class Graduate: public Student{
public:
    Graduate(const std::string& ID, const std::string& name, const std::string& year, const std::string degree="G")
    :Student(ID, name, year, degree){};
    double getGpa();
    double getAvgScore();
};
// TODO: implement class Undergraduate.
class Undergraduate: public Student{
public:
    Undergraduate(const std::string& ID, const std::string& name, const std::string& year, const std::string degree="U")
    :Student(ID, name, year, degree){};
    double getGpa();
    double getAvgScore();
};

class StudentWrapper {
private:
    const Student &student;
    double score = -1.0; // Invalid Score
public:
    const std::string id;
    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student): id(id), student(student){};

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Wrong Score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
