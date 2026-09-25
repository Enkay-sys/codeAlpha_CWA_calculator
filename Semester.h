#ifndef SEMESTER_H
#define SEMESTER_H

#include <string>
#include <vector>
using namespace std;

enum semester{FIRST, SECOND, NONE};

struct Courses{
    string courseName;
    double score;
    int creditHours;
};

class Semester{
    public:
    Semester();

    void setName(const semester&);
    semester getName() const;

    void createourse(const Courses&);
    void clearCourses();
    void reserveCourseNumber(int);

    const vector<Courses>& getCourses() const;
    int getNumberOfOCourses() const;

    // CWA is always calculated from the courses, so it can never go out of sync
    int getTotalCreditHours() const;
    double getTotalPoints() const;      // sum of (score * creditHours)
    double getCWA() const;

    private:
    semester name;
    vector<Courses> courses;
};

#endif
