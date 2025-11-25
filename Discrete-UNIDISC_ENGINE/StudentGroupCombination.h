#pragma once

#include "Core.h"
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>

class CombinationsModule {
private:
    DataManager* dm;

    unsigned long long factorial(int n) {
        if (n <= 1) return 1;
        unsigned long long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }

    unsigned long long combination(int n, int r) {
        if (r > n) return 0;
        if (r == 0 || r == n) return 1;

        if (r > n - r) r = n - r;

        unsigned long long result = 1;
        for (int i = 0; i < r; i++) {
            result *= (n - i);
            result /= (i + 1);
        }
        return result;
    }
    void generateCombinations(
        const vector<string>& items,
        int start,
        int groupSize,
        vector<string>& current,
        vector<vector<string>>& result
    ) {
        if (current.size() == groupSize) {
            result.push_back(current);
            return;
        }

        for (int i = start; i < items.size(); i++) {
            current.push_back(items[i]);
            generateCombinations(items, i + 1, groupSize, current, result);
            current.pop_back();
        }
    }

public:
    CombinationsModule() {
        dm = DataManager::getInstance();
    }

    vector<vector<string>> formProjectGroups(int groupSize) {
        vector<string> studentIDs;
        for (auto& pair : dm->getAllStudents()) {
            studentIDs.push_back(pair.first);
        }

        if (studentIDs.empty() || groupSize <= 0 || groupSize > studentIDs.size()) {
            cout << "Invalid group size or insufficient students." << endl;
            return vector<vector<string>>();
        }

        vector<vector<string>> groups;
        vector<string> current;
        generateCombinations(studentIDs, 0, groupSize, current, groups);

        return groups;
    }

    map<string, vector<string>> assignLabGroups(const string& courseID, int studentsPerLab) {
        map<string, vector<string>> assignments;

        
        vector<string> enrolledStudents;
        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;
            if (student.getEnrolledCourses().find(courseID) != student.getEnrolledCourses().end()) {
                enrolledStudents.push_back(student.getID());
            }
        }

        if (enrolledStudents.empty()) {
            cout << "No students enrolled in course " << courseID << endl;
            return assignments;
        }

        vector<Lab*> courseLabs;
        for (auto& pair : dm->getAllLabs()) {
            Lab& lab = pair.second;
            if (lab.getAssociatedCourse() == courseID) {
                courseLabs.push_back(&lab);
            }
        }

        if (courseLabs.empty()) {
            cout << "No labs available for course " << courseID << endl;
            return assignments;
        }

        
        int labIndex = 0;
        for (const string& studentID : enrolledStudents) {
            Lab* currentLab = courseLabs[labIndex];

            if (currentLab->canEnroll()) {
                currentLab->enrollStudent(studentID);
                assignments[currentLab->getID()].push_back(studentID);

                if (assignments[currentLab->getID()].size() >= studentsPerLab) {
                    labIndex = (labIndex + 1) % courseLabs.size();
                }
            }
            else {
                labIndex = (labIndex + 1) % courseLabs.size();
            }
        }

        return assignments;
    }

    
    map<string, vector<string>> assignElectives(const vector<string>& electiveCourses, int maxPerElective) {
        map<string, vector<string>> assignments;

        
        for (const string& courseID : electiveCourses) {
            assignments[courseID] = vector<string>();
        }

        
        vector<string> studentIDs;
        for (auto& pair : dm->getAllStudents()) {
            studentIDs.push_back(pair.first);
        }

        int courseIndex = 0;
        for (const string& studentID : studentIDs) {
            const string& courseID = electiveCourses[courseIndex];

            if (assignments[courseID].size() < maxPerElective) {
                assignments[courseID].push_back(studentID);
            }

            courseIndex = (courseIndex + 1) % electiveCourses.size();
        }

        return assignments;
    }

    
    unsigned long long calculateTotalCombinations(int n, int r) {
        return combination(n, r);
    }

    
    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 2: STUDENT GROUP COMBINATIONS DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        int totalStudents = dm->getAllStudents().size();

        if (totalStudents == 0) {
            cout << "No students in the system. Please add students first." << endl;
            return;
        }

        cout << "Total Students: " << totalStudents << endl;

        
        if (totalStudents >= 3) {
            cout << "\n--- Project Groups (Size 3) ---" << endl;
            vector<vector<string>> groups = formProjectGroups(3);

            int displayCount = min(10, (int)groups.size());
            for (int i = 0; i < displayCount; i++) {
                cout << "Group " << (i + 1) << ": ";
                for (size_t j = 0; j < groups[i].size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << groups[i][j];
                }
                cout << endl;
            }

            cout << "Total possible groups: " << groups.size() << endl;
            cout << "Mathematical verification: C(" << totalStudents << ", 3) = "
                << calculateTotalCombinations(totalStudents, 3) << endl;
        }

        
        cout << "\n--- Combination Calculations ---" << endl;
        if (totalStudents >= 2) {
            cout << "C(" << totalStudents << ", 2) = "
                << calculateTotalCombinations(totalStudents, 2) << " (pairs)" << endl;
        }
        if (totalStudents >= 4) {
            cout << "C(" << totalStudents << ", 4) = "
                << calculateTotalCombinations(totalStudents, 4) << " (groups of 4)" << endl;
        }
        if (totalStudents >= 5) {
            cout << "C(" << totalStudents << ", 5) = "
                << calculateTotalCombinations(totalStudents, 5) << " (groups of 5)" << endl;
        }
    }
};

