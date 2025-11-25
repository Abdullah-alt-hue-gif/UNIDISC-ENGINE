#pragma once

#include "Core.h"
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>

class FunctionsModule {
private:
    DataManager* dm;

    map<string, string> studentToCourse;
    map<string, string> courseToFaculty;
    map<string, string> facultyToRoom;

public:
    FunctionsModule() {
        dm = DataManager::getInstance();
        buildFunctions();
    }

    void buildFunctions() {
        studentToCourse.clear();
        courseToFaculty.clear();
        facultyToRoom.clear();

    
        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;
            if (!student.getEnrolledCourses().empty()) {
                studentToCourse[student.getID()] = *student.getEnrolledCourses().begin();
            }
        }

        for (auto& pair : dm->getAllFaculty()) {
            Faculty& faculty = pair.second;
            for (const string& courseID : faculty.getAssignedCourses()) {
                courseToFaculty[courseID] = faculty.getID();
            }
        }

        if (!dm->getAllRooms().empty()) {
            string firstRoom = dm->getAllRooms().begin()->first;
            for (auto& pair : dm->getAllFaculty()) {
                facultyToRoom[pair.first] = firstRoom;
            }
        }
    }

    bool isInjective(const map<string, string>& func) {
        set<string> images;
        for (const auto& pair : func) {
            if (images.find(pair.second) != images.end()) {
                return false;
            }
            images.insert(pair.second);
        }
        return true;
    }

    bool isSurjective(const map<string, string>& func, const set<string>& codomain) {
        set<string> images;
        for (const auto& pair : func) {
            images.insert(pair.second);
        }
        return images == codomain;
    }

    
    bool isBijective(const map<string, string>& func, const set<string>& codomain) {
        return isInjective(func) && isSurjective(func, codomain);
    }

    map<string, string> composeFunctions(const map<string, string>& f, const map<string, string>& g) {
        map<string, string> result;

        for (const auto& pair : g) {
            string x = pair.first;
            string y = pair.second;

            if (f.find(y) != f.end()) {
                result[x] = f.at(y);
            }
        }

        return result;
    }
    map<string, string> inverseFunction(const map<string, string>& func) {
        map<string, string> inverse;

        if (!isInjective(func)) {
            cout << "Function is not injective - inverse doesn't exist" << endl;
            return inverse;
        }

        for (const auto& pair : func) {
            inverse[pair.second] = pair.first;
        }

        return inverse;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 7: FUNCTIONS DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        buildFunctions();

        cout << "Student --> Course mapping: " << studentToCourse.size() << " mappings" << endl;
        cout << "Course --> Faculty mapping: " << courseToFaculty.size() << " mappings" << endl;
        cout << "Faculty --> Room mapping: " << facultyToRoom.size() << " mappings" << endl;

        cout << "\n--- Function Properties ---" << endl;

        cout << "Student --> Course:" << endl;
        cout << "  Injective: " << (isInjective(studentToCourse) ? "Yes" : "No") << endl;

        cout << "\nCourse --> Faculty:" << endl;
        cout << "  Injective: " << (isInjective(courseToFaculty) ? "Yes" : "No") << endl;

        map<string, string> composed = composeFunctions(courseToFaculty, studentToCourse);
        cout << "\n--- Function Composition ---" << endl;
        cout << "Student --> Course --> Faculty: " << composed.size() << " mappings" << endl;

     
        cout << "\nSample composed mappings (Student --> Faculty):" << endl;
        int count = 0;
        for (const auto& pair : composed) {
            cout << "  " << pair.first << " --> " << pair.second << endl;
            if (++count >= 5) break;
        }

        if (isInjective(courseToFaculty)) {
            map<string, string> inverse = inverseFunction(courseToFaculty);
            cout << "\n--- Inverse Function (Faculty --> Course) ---" << endl;
            cout << "Inverse size: " << inverse.size() << " mappings" << endl;
        }
    }
};