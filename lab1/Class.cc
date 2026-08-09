#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>

std::string Class::toString() const {
    std::ostringstream oss;
    oss << "Class Information:"
        << "\n\tname: " << name
        << "\n\tpoint: " << point
        << std::endl;
    return oss.str();
}

void Class::addStudent(const Student& st) {
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper& Class::getStudentWrapper(const std::string& studentId) {
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No Match Student!";
}

double Class::getHighestScore() {
    // TODO implement getHighestScore
    return 0.0;
}

double Class::getLowestScore() {
    // TODO implement getLowestScore
    return 0.0;
}

double Class::getAvgScore() {
    // TODO implement getAvgScore
    return 0.0;
}

void Class::saveScore(const std::string& filename) {
    // TODO implement saveScore
}
