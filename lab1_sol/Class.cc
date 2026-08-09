#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>
#include <iostream>
#include <fstream>

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
    double maxScore = -1.0;
    int scale = students.size();
    for(int i = 0; i < scale; i++){
        double score = students[i].getScore();
        if(score == -1.0) continue;
        if(score > maxScore){
            maxScore = score;
        }
    }
    if(maxScore == -1.0){
        throw "No Valid Score";
    } else {
        return maxScore;
    }
}

double Class::getLowestScore() {
    // TODO implement getLowestScore
    double minScore = 101.0;
    int scale = students.size();
    for(int i = 0; i < scale; i++){
        double score = students[i].getScore();
        if (score == -1.0) continue;
        if(score < minScore){
            minScore = score;
        }
    }
    if(minScore == 101.0){
        throw "No Valid Score";
    } else {
        return minScore;
    }
}

double Class::getAvgScore() {
    // TODO implement getAvgScore
    double avg = 0;
    int scale = students.size(), sumClass = scale;
    for(int i = 0; i < scale; i++){
        double score = students[i].getScore();
        if(score == -1.0){
            sumClass--;
            continue;
        }
        avg += students[i].getScore();
    }
    return avg / sumClass;
}

void Class::saveScore(const std::string& filename) {
    // TODO implement saveScore
    std::ofstream outFile(filename);
    outFile << "Class:" << this->name << '\n';
    outFile << "Point:" << this->point << '\n';
    for(const StudentWrapper& sw : this->students){
        outFile << sw.id << ";" << sw.getScore() <<"\n";
    }
    outFile.close();
}
