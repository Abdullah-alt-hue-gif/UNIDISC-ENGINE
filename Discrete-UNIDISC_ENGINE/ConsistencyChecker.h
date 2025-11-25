#pragma once
#include "Core.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <chrono>

class ConsistencyChecker {
private:
    DataManager* dm;
    vector<string> violations;

public:
    ConsistencyChecker() {
        dm = DataManager::getInstance();
    }

    void detectCourseConflicts() {
        cout << "\n--- Detecting Course Conflicts ---" << endl;
        violations.clear();

        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;
            set<string> enrolled = student.getEnrolledCourses();

            
            map<string, vector<string>> prefixGroups;

            for (const string& courseID : enrolled) {
                if (courseID.length() >= 2) {
                    string prefix = courseID.substr(0, 2);
                    prefixGroups[prefix].push_back(courseID);
                }
            }

            for (auto& prefixPair : prefixGroups) {
                if (prefixPair.second.size() > 3) {
                    string violation = "Student " + student.getID() +
                        " enrolled in too many " + prefixPair.first +
                        " courses: " + to_string(prefixPair.second.size());
                    violations.push_back(violation);
                    cout << "CONFLICT: " << violation << endl;
                }
            }
        }

        if (violations.empty()) {
            cout << "No course conflicts detected" << endl;
        }
    }

    void detectMissingPrerequisites() {
        cout << "\n--- Detecting Missing Prerequisites ---" << endl;
        int violationCount = 0;

        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;

            for (const string& courseID : student.getEnrolledCourses()) {
                Course* course = dm->getCourse(courseID);
                if (!course) continue;

                for (const string& prereq : course->getPrerequisites()) {
                    if (student.getCompletedCourses().find(prereq) ==
                        student.getCompletedCourses().end() &&
                        student.getEnrolledCourses().find(prereq) ==
                        student.getEnrolledCourses().end()) {

                        string violation = "Student " + student.getID() +
                            " enrolled in " + courseID +
                            " without prerequisite " + prereq;
                        violations.push_back(violation);
                        cout << "VIOLATION: " << violation << endl;
                        violationCount++;
                    }
                }
            }
        }

        if (violationCount == 0) {
            cout << "No missing prerequisites" << endl;
        }
    }

    void detectOverload() {
        cout << "\n--- Detecting Student Overload ---" << endl;
        const int MAX_CREDITS = 18;
        int overloadCount = 0;

        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;

            if (student.getCurrentCredits() > MAX_CREDITS) {
                string violation = "Student " + student.getID() +
                    " overloaded: " + to_string(student.getCurrentCredits()) +
                    " credits (max: " + to_string(MAX_CREDITS) + ")";
                violations.push_back(violation);
                cout << "OVERLOAD: " << violation << endl;
                overloadCount++;
            }
        }

        if (overloadCount == 0) {
            cout << "No student overloads" << endl;
        }
    }

    void runAllChecks() {
        violations.clear();
        detectCourseConflicts();
        detectMissingPrerequisites();
        detectOverload();

        cout << "\n=== CONSISTENCY CHECK SUMMARY ===" << endl;
        cout << "Total violations found: " << violations.size() << endl;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 9: CONSISTENCY CHECKER DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        runAllChecks();
    }
};