#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <string>
#include "Degree.h"

using namespace std;

const string DATA_FILE = "student_record.txt";
const int MAX_COURSES_PER_SEMESTER = 21;
const int MAX_CREDIT_HOURS = 10;

// =====================================================================
//  Input helpers - every read is a whole line, so cin never gets stuck
//  and the user is always told what was wrong.
// =====================================================================

string readLine(const string &prompt){
    string line;
    cout << prompt;
    if (!getline(cin, line)){
        cout << "\nInput closed. Exiting.\n";
        exit(0);
    }
    return line;
}

int readInt(const string &prompt, int low, int high){
    while (true){
        istringstream iss(readLine(prompt));
        int value;
        char extra;
        if (iss >> value && !(iss >> extra) && value >= low && value <= high){
            return value;
        }
        cout << "Invalid input. Please enter a whole number between "
             << low << " and " << high << ".\n";
    }
}

double readDouble(const string &prompt, double low, double high){
    while (true){
        istringstream iss(readLine(prompt));
        double value;
        char extra;
        if (iss >> value && !(iss >> extra) && value >= low && value <= high){
            return value;
        }
        cout << "Invalid input. Please enter a number between "
             << low << " and " << high << ".\n";
    }
}

string readCourseName(const string &prompt){
    while (true){
        string line = readLine(prompt);
        size_t start = line.find_first_not_of(" \t");
        if (start != string::npos){
            size_t end = line.find_last_not_of(" \t");
            return line.substr(start, end - start + 1);
        }
        cout << "Invalid course name. It cannot be empty.\n";
    }
}

bool readYesNo(const string &prompt){
    while (true){
        string line = readLine(prompt);
        if (line == "y" || line == "Y") return true;
        if (line == "n" || line == "N") return false;
        cout << "Please enter y or n.\n";
    }
}

// =====================================================================
//  File storage
//  Format:  <number of years>
//           then, for every year, for semester 1 and semester 2:
//               <number of courses>
//               courseName,score,creditHours     (one line per course)
//  CWA values are not stored - they are recalculated from the courses.
// =====================================================================

bool saveData(const Degree &degree){
    ofstream outFile(DATA_FILE);
    if (!outFile){
        cout << "ERROR: could not open \"" << DATA_FILE << "\" for writing. Nothing was saved.\n";
        return false;
    }

    outFile << degree.getNumberOfYears() << "\n";
    for (int y = 0; y < degree.getNumberOfYears(); y++){
        AcademicYear year = degree.getYear(y);
        for (int s = 0; s < 2; s++){
            Semester sem = year.getSemester(s);
            const vector<Courses> &courses = sem.getCourses();

            outFile << courses.size() << "\n";
            for (size_t j = 0; j < courses.size(); j++){
                outFile << courses[j].courseName << ","
                        << courses[j].score << ","
                        << courses[j].creditHours << "\n";
            }
        }
    }

    outFile.close();
    if (outFile.fail()){
        cout << "ERROR: something went wrong while writing \"" << DATA_FILE << "\".\n";
        return false;
    }
    cout << "Record saved.\n";
    return true;
}

// Splits "name,score,credit" from the RIGHT, so course names may contain commas.
bool parseCourseLine(const string &line, Courses &c){
    size_t last = line.rfind(',');
    if (last == string::npos || last == 0) return false;
    size_t prev = line.rfind(',', last - 1);
    if (prev == string::npos || prev == 0) return false;

    string name = line.substr(0, prev);
    string scoreStr = line.substr(prev + 1, last - prev - 1);
    string creditStr = line.substr(last + 1);

    try {
        size_t pos;
        double score = stod(scoreStr, &pos);
        if (pos != scoreStr.size() || score < 0 || score > 100) return false;
        int credit = stoi(creditStr, &pos);
        if (pos != creditStr.size() || credit < 1 || credit > MAX_CREDIT_HOURS) return false;
        c.courseName = name;
        c.score = score;
        c.creditHours = credit;
    } catch (...) {
        return false;
    }
    return true;
}

void loadFailed(){
    cout << "WARNING: \"" << DATA_FILE << "\" could not be read (it may be damaged or in an old format).\n"
         << "Starting fresh. The file will be overwritten the next time you save, "
         << "so copy it somewhere first if you want to keep it.\n";
}

// Loads into a temporary Degree first, so a bad file can never leave you with half-loaded data.
void loadData(Degree &degree){
    ifstream inFile(DATA_FILE);
    if (!inFile){
        cout << "No previous record found. Starting fresh.\n";
        return;
    }

    int numYears;
    if (!(inFile >> numYears)){
        if (inFile.eof()){
            cout << "Record file is empty. Starting fresh.\n";
        } else {
            loadFailed();
        }
        return;
    }
    if (numYears < 1 || numYears > MAX_YEARS){
        loadFailed();
        return;
    }

    Degree loaded;
    for (int y = 0; y < numYears; y++){
        AcademicYear year;
        for (int s = 0; s < 2; s++){
            int numCourses;
            if (!(inFile >> numCourses) || numCourses < 0 || numCourses > MAX_COURSES_PER_SEMESTER){
                loadFailed();
                return;
            }
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');

            Semester sem;
            sem.setName(s == 0 ? FIRST : SECOND);
            for (int j = 0; j < numCourses; j++){
                string line;
                Courses c;
                if (!getline(inFile, line) || !parseCourseLine(line, c)){
                    loadFailed();
                    return;
                }
                sem.createourse(c);
            }
            year.setSemester(s, sem);
        }
        loaded.setYear(y, year);
    }

    degree = loaded;
    cout << "Previous record loaded (" << degree.getNumberOfYears() << " years).\n";
}

// =====================================================================
//  Menu option 1: enter (or replace) the courses of one semester
// =====================================================================

void calculateCWA(Degree &degree){
    int yearNumber = readInt("Which year? (1-" + to_string(MAX_YEARS) + "): ", 1, MAX_YEARS);
    int semNumber = readInt("Which semester? (1 = First, 2 = Second): ", 1, 2);
    int yearIndex = yearNumber - 1;
    int semIndex = semNumber - 1;

    AcademicYear year = degree.getYear(yearIndex);
    Semester existing = year.getSemester(semIndex);

    // Re-entering a semester REPLACES it (the old code silently piled new courses on top).
    if (existing.getNumberOfOCourses() > 0){
        cout << "Year " << yearNumber << ", Semester " << semNumber << " already has "
             << existing.getNumberOfOCourses() << " course(s).\n";
        if (!readYesNo("Replace them? (y/n): ")){
            cout << "Cancelled. Nothing changed.\n";
            return;
        }
    }

    int numCourses = readInt("How many courses this semester? (1-" +
                             to_string(MAX_COURSES_PER_SEMESTER) + "): ",
                             1, MAX_COURSES_PER_SEMESTER);

    Semester s;
    s.setName(semIndex == 0 ? FIRST : SECOND);
    s.reserveCourseNumber(numCourses);

    for (int i = 0; i < numCourses; i++){
        Courses c;
        cout << "\n--- Course " << (i + 1) << " ---\n";
        c.courseName = readCourseName("Enter course name: ");
        c.score = readDouble("Score (0-100): ", 0, 100);
        c.creditHours = readInt("Credit hours (1-" + to_string(MAX_CREDIT_HOURS) + "): ",
                                1, MAX_CREDIT_HOURS);
        s.createourse(c);
    }

    year.setSemester(semIndex, s);
    degree.setYear(yearIndex, year);

    cout << fixed << setprecision(2);
    cout << "\nYear " << yearNumber << ", Semester " << semNumber
         << " CWA: " << s.getCWA() << "\n";
    cout << "Year " << yearNumber << " CWA so far: " << year.calculateYearCWA() << "\n";
    cout << "Cumulative CWA (all years): " << degree.calculateCumulativeCWA() << "\n";

    saveData(degree);
}

// =====================================================================
//  Menu option 2: print transcript for every year that has data
// =====================================================================

const int COL_NAME = 30;
const int COL_SCORE = 10;
const int COL_CREDIT = 12;
const int TABLE_WIDTH = COL_NAME + COL_SCORE + COL_CREDIT;

void printDivider(char ch = '-'){
    cout << string(TABLE_WIDTH, ch) << "\n";
}

// Truncates a course name so it never breaks the column alignment.
string fitName(const string &name){
    if ((int)name.size() <= COL_NAME - 1) return name;
    return name.substr(0, COL_NAME - 4) + "...";
}

void printSemesterTable(const Semester &s){
    const vector<Courses> &courses = s.getCourses();
    if (courses.empty()){
        cout << "  (No courses recorded)\n";
        return;
    }

    printDivider();
    cout << left  << setw(COL_NAME)   << "Course"
         << right << setw(COL_SCORE)  << "Score"
         << right << setw(COL_CREDIT) << "Credit Hrs" << "\n";
    printDivider();

    for (size_t j = 0; j < courses.size(); j++){
        cout << left  << setw(COL_NAME)   << fitName(courses[j].courseName)
             << right << setw(COL_SCORE)  << courses[j].score
             << right << setw(COL_CREDIT) << courses[j].creditHours << "\n";
    }
    printDivider();
    cout << left << setw(COL_NAME + COL_SCORE) << "Semester CWA"
         << right << setw(COL_CREDIT) << s.getCWA() << "\n";
}

void printTranscript(const Degree &degree){
    cout << fixed << setprecision(2);
    cout << "\n";
    printDivider('=');
    cout << string(TABLE_WIDTH / 2 - 5, ' ') << "TRANSCRIPT\n";
    printDivider('=');

    bool anyData = false;
    for (int y = 0; y < degree.getNumberOfYears(); y++){
        AcademicYear year = degree.getYear(y);
        if (!year.hasCourses()) continue;
        anyData = true;

        cout << "\nYEAR " << (y + 1) << "\n";
        for (int i = 0; i < 2; i++){
            Semester s = year.getSemester(i);
            cout << "\nSemester " << (i + 1) << ":\n";
            printSemesterTable(s);
        }
        cout << "\n";
        printDivider();
        cout << left << setw(COL_NAME + COL_SCORE) << ("YEAR " + to_string(y + 1) + " CWA")
             << right << setw(COL_CREDIT) << year.calculateYearCWA() << "\n";
        printDivider('=');
    }

    if (!anyData){
        cout << "\nNo courses recorded yet.\n";
    } else {
        cout << "\n";
        cout << left << setw(COL_NAME + COL_SCORE) << "CUMULATIVE CWA (ALL YEARS)"
             << right << setw(COL_CREDIT) << degree.calculateCumulativeCWA() << "\n";
        printDivider('=');
    }
}

// =====================================================================
//  Main menu loop
// =====================================================================

int main(){
    Degree degree;
    loadData(degree);

    int choice;
    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Enter courses / calculate CWA\n";
        cout << "2. Print Transcript\n";
        cout << "3. Exit\n";
        choice = readInt("Enter choice: ", 1, 3);

        switch (choice){
            case 1:
                calculateCWA(degree);
                break;
            case 2:
                printTranscript(degree);
                break;
            case 3:
                cout << "Goodbye!\n";
                break;
        }
    } while (choice != 3);

    return 0;
}