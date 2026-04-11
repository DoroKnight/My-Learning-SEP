#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.
    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

// TODO: implement functions which are declared in Student.h.
void Student::addClasses(Class *c){
    classes.push_back(c);
}
double Undergraduate::getGpa(){
    return this->getAvgScore() / 20;
}
double Graduate::getGpa(){
    double sumPoint = 0, sumGpa = 0, size = classes.size(), sumClass = size;
    for(int i = 0; i < size; i++){
        double score = classes[i]->getStudentWrapper(this->id).getScore();
        if(score < 0){
            sumClass --;
            continue;
        } else if(score >= 90){
            sumGpa += classes[i]->point * 4.0;
        } else if(score >= 80){
            sumGpa += classes[i]->point * 3.5;
        } else if(score >= 70){
            sumGpa += classes[i]->point * 3.0;
        } else if(score >= 60){
            sumGpa += classes[i]->point * 2.5;
        } else {
            sumGpa += classes[i]->point * 2.0;
        }
        sumPoint += classes[i]->point;
    }
    return sumGpa / sumPoint;
}
double Undergraduate::getAvgScore(){
    double avg = 0, size = classes.size(), sumPoint = 0;
    for(int i = 0; i < size; i++){
        double score = classes[i]->getStudentWrapper(this->id).getScore();
        if(score < 0){
            continue;
        }
        avg += score * classes[i]->point;
        sumPoint += classes[i]->point;
    }
    if (sumPoint == 0) {
        throw "No Vaild Score";
    }
    return avg / sumPoint;
}
double Graduate::getAvgScore(){
    double avg = 0, sumClass = classes.size(), sumPoint = 0;
    for(int i = 0; i < sumClass; i++){
        double score = classes[i]->getStudentWrapper(this->id).getScore();
        if(score < 0) continue;
        avg += score * classes[i]->point;
        sumPoint += classes[i]->point;
    }
    return avg / sumPoint;
}