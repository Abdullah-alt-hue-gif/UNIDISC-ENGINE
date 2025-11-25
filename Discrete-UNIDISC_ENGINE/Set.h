#pragma once

#include "Core.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>

class SetOperationsModule {
private:
    DataManager* dm;

    template<typename T>
    set<T> setUnion(const set<T>& a, const set<T>& b) {
        set<T> result = a;
        result.insert(b.begin(), b.end());
        return result;
    }

    template<typename T>
    set<T> setIntersection(const set<T>& a, const set<T>& b) {
        set<T> result;
        set_intersection(a.begin(), a.end(), b.begin(), b.end(),
            inserter(result, result.begin()));
        return result;
    }

    template<typename T>
    set<T> setDifference(const set<T>& a, const set<T>& b) {
        set<T> result;
        set_difference(a.begin(), a.end(), b.begin(), b.end(),
            inserter(result, result.begin()));
        return result;
    }

    template<typename T>
    vector<set<T>> powerSet(const set<T>& s) {
        vector<T> vec(s.begin(), s.end());
        vector<set<T>> result;

        int n = vec.size();
        int powerSetSize = pow(2, n);

        for (int i = 0; i < powerSetSize && i < 1000; i++) { 
            set<T> subset;
            for (int j = 0; j < n; j++) {
                if (i & (1 << j)) {
                    subset.insert(vec[j]);
                }
            }
            result.push_back(subset);
        }

        return result;
    }

public:
    SetOperationsModule() {
        dm = DataManager::getInstance();
    }

    set<string> getStudentSet() {
        set<string> students;
        for (auto& pair : dm->getAllStudents()) {
            students.insert(pair.first);
        }
        return students;
    }

    set<string> getCourseSet() {
        set<string> courses;
        for (auto& pair : dm->getAllCourses()) {
            courses.insert(pair.first);
        }
        return courses;
    }

    set<string> studentsInMultipleCourses(const vector<string>& courseIDs) {
        if (courseIDs.empty()) return set<string>();

        set<string> result;
        bool first = true;

        for (const string& courseID : courseIDs) {
            set<string> enrolled;

            for (auto& pair : dm->getAllStudents()) {
                Student& student = pair.second;
                if (student.getEnrolledCourses().find(courseID) !=
                    student.getEnrolledCourses().end()) {
                    enrolled.insert(student.getID());
                }
            }

            if (first) {
                result = enrolled;
                first = false;
            }
            else {
                result = setIntersection(result, enrolled);
            }
        }

        return result;
    }

    set<string> studentsInAnyCourse(const vector<string>& courseIDs) {
        set<string> result;

        for (const string& courseID : courseIDs) {
            for (auto& pair : dm->getAllStudents()) {
                Student& student = pair.second;
                if (student.getEnrolledCourses().find(courseID) !=
                    student.getEnrolledCourses().end()) {
                    result.insert(student.getID());
                }
            }
        }

        return result;
    }

    set<string> studentDifference(const string& courseA, const string& courseB) {
        set<string> studentsA, studentsB;

        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;
            if (student.getEnrolledCourses().find(courseA) !=
                student.getEnrolledCourses().end()) {
                studentsA.insert(student.getID());
            }
            if (student.getEnrolledCourses().find(courseB) !=
                student.getEnrolledCourses().end()) {
                studentsB.insert(student.getID());
            }
        }

        return setDifference(studentsA, studentsB);
    }

    vector<set<string>> generatePowerSet(int maxStudents = 5) {
        set<string> students = getStudentSet();

        if (students.size() > maxStudents) {
            set<string> limited;
            int count = 0;
            for (const string& s : students) {
                limited.insert(s);
                if (++count >= maxStudents) break;
            }
            students = limited;
        }

        return powerSet(students);
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 5: SET OPERATIONS DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        set<string> students = getStudentSet();
        set<string> courses = getCourseSet();

        cout << "Total Students: " << students.size() << endl;
        cout << "Total Courses: " << courses.size() << endl;

        if (courses.size() >= 2) {
            auto it = courses.begin();
            string course1 = *it++;
            string course2 = *it;

            cout << "\n--- Set Operations Example ---" << endl;
            cout << "Course A: " << course1 << endl;
            cout << "Course B: " << course2 << endl;

            set<string> inBoth = studentsInMultipleCourses({ course1, course2 });
            set<string> inEither = studentsInAnyCourse({ course1, course2 });
            set<string> onlyInA = studentDifference(course1, course2);

            cout << "\nIntersection (in both): " << inBoth.size() << " students" << endl;
            cout << "Union (in either): " << inEither.size() << " students" << endl;
            cout << "Difference (only in A): " << onlyInA.size() << " students" << endl;
        }

        if (students.size() > 0 && students.size() <= 5) {
            cout << "\n--- Power Set Example ---" << endl;
            vector<set<string>> ps = generatePowerSet(students.size());
            cout << "Power set size: " << ps.size() << " subsets" << endl;
            cout << "Expected: 2^" << students.size() << " = "
                << (int)pow(2, students.size()) << endl;
        }
    }
};
