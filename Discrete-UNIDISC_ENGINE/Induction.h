#pragma once

#include "Core.h"
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <iostream>
#include <functional>
using namespace std;

class InductionModule {
private:
    DataManager* dm;

    set<string> getDirectPrerequisites(const string& courseID) {
        Course* course = dm->getCourse(courseID);
        if (!course) return set<string>();
        return course->getPrerequisites();
    }

    void getAllPrerequisitesRecursive(const string& courseID, set<string>& allPrereqs, set<string>& visited) {
        if (visited.find(courseID) != visited.end()) return;
        visited.insert(courseID);

        Course* course = dm->getCourse(courseID);
        if (!course) return;

        for (const string& prereq : course->getPrerequisites()) {
            allPrereqs.insert(prereq);
            getAllPrerequisitesRecursive(prereq, allPrereqs, visited);
        }
    }

public:
    InductionModule() {
        dm = DataManager::getInstance();
    }

    bool verifyBaseCase(const string& studentID, const string& courseID) {
        Course* course = dm->getCourse(courseID);
        if (!course) {
            cout << "Course not found." << endl;
            return false;
        }

        if (course->getPrerequisites().empty()) {
            cout << "Base Case: Course " << courseID << " has no prerequisites." << endl;
            cout << "Student " << studentID << " can take this course." << endl;
            return true;
        }

        return false;
    }

    bool verifyInductiveStep(const string& studentID, const string& courseID) {
        Student* student = dm->getStudent(studentID);
        Course* course = dm->getCourse(courseID);

        if (!student || !course) {
            cout << "Invalid student or course." << endl;
            return false;
        }

        cout << "\n--- Verifying Inductive Step for " << courseID << " ---" << endl;
        cout << "Hypothesis: If student completed all level-k prerequisites," << endl;
        cout << "            they can take level-(k+1) course " << courseID << endl;

        set<string> prerequisites = course->getPrerequisites();

        if (prerequisites.empty()) {
            cout << "No prerequisites - Base case applies." << endl;
            return true;
        }

        cout << "\nChecking prerequisites:" << endl;
        bool allCompleted = true;

        for (const string& prereq : prerequisites) {
            bool completed = student->getCompletedCourses().find(prereq) !=
                student->getCompletedCourses().end();
            cout << "  " << prereq << ": " << (completed ? "Completed" : "Not completed") << endl;

            if (!completed) allCompleted = false;
        }

        if (allCompleted) {
            cout << "\nConclusion: All prerequisites satisfied. Student can take " << courseID << endl;
            return true;
        }
        else {
            cout << "\nConclusion: Not all prerequisites satisfied." << endl;
            return false;
        }
    }

    bool verifyPrerequisiteChain(const string& studentID, const string& courseID) {
        Student* student = dm->getStudent(studentID);
        Course* course = dm->getCourse(courseID);

        if (!student || !course) return false;

        cout << "\n=== MATHEMATICAL INDUCTION PROOF ===" << endl;
        cout << "Proving: Student " << studentID << " can take course " << courseID << endl;

        set<string> allPrereqs;
        set<string> visited;
        getAllPrerequisitesRecursive(courseID, allPrereqs, visited);

        if (allPrereqs.empty()) {
            cout << "\nBase Case: Course has no prerequisites." << endl;
            cout << "Proof complete: Student can take the course." << endl;
            return true;
        }

        map<int, set<string>> levelMap;
        map<string, int> courseLevel;

        function<int(const string&)> computeLevel = [&](const string& cid) -> int {
            if (courseLevel.find(cid) != courseLevel.end()) {
                return courseLevel[cid];
            }

            Course* c = dm->getCourse(cid);
            if (!c || c->getPrerequisites().empty()) {
                courseLevel[cid] = 0;
                return 0;
            }

            int maxLevel = 0;
            for (const string& prereq : c->getPrerequisites()) {
                maxLevel = max(maxLevel, computeLevel(prereq) + 1);
            }

            courseLevel[cid] = maxLevel;
            return maxLevel;
            };

        for (const string& prereq : allPrereqs) {
            int level = computeLevel(prereq);
            levelMap[level].insert(prereq);
        }

        int targetLevel = computeLevel(courseID);

        cout << "\n--- Induction Proof ---" << endl;

        cout << "\nBase Case (Level 0): Courses with no prerequisites" << endl;
        if (levelMap[0].empty()) {
            cout << "  No level-0 courses in chain." << endl;
        }
        else {
            for (const string& prereq : levelMap[0]) {
                bool completed = student->getCompletedCourses().find(prereq) !=
                    student->getCompletedCourses().end();
                cout << "  " << prereq << ": " << (completed ? "Completed" : "Not completed") << endl;

                if (!completed) {
                    cout << "\nProof fails: Base case prerequisite not completed." << endl;
                    return false;
                }
            }
        }

        for (int k = 0; k < targetLevel; k++) {
            cout << "\nInductive Step (Level " << k << " --> " << (k + 1) << "):" << endl;
            cout << "  Hypothesis: All level-" << k << " courses completed." << endl;
            cout << "  To prove: Can complete level-" << (k + 1) << " courses." << endl;

            if (levelMap[k + 1].empty()) continue;

            for (const string& prereq : levelMap[k + 1]) {
                Course* prereqCourse = dm->getCourse(prereq);
                if (!prereqCourse) continue;

                bool canTake = true;
                for (const string& subPrereq : prereqCourse->getPrerequisites()) {
                    if (student->getCompletedCourses().find(subPrereq) ==
                        student->getCompletedCourses().end()) {
                        canTake = false;
                        break;
                    }
                }

                bool completed = student->getCompletedCourses().find(prereq) !=
                    student->getCompletedCourses().end();

                cout << "  " << prereq << ": Prerequisites "
                    << (canTake ? "satisfied" : "not satisfied")
                    << ", " << (completed ? "Completed" : "Not completed") << endl;

                if (!completed) {
                    cout << "\nProof fails at level " << (k + 1) << endl;
                    return false;
                }
            }
        }

        cout << "\n=== PROOF COMPLETE ===" << endl;
        cout << "By mathematical induction, all prerequisites are satisfied." << endl;
        cout << "Student can take course " << courseID << endl;

        return true;
    }

    bool verifyStrongInduction(const string& studentID, const string& courseID) {
        cout << "\n=== STRONG INDUCTION VERIFICATION ===" << endl;
        cout << "Course: " << courseID << endl;

        Student* student = dm->getStudent(studentID);
        if (!student) return false;

        set<string> allPrereqs;
        set<string> visited;
        getAllPrerequisitesRecursive(courseID, allPrereqs, visited);

        cout << "\nAll prerequisites (direct and indirect):" << endl;

        bool allSatisfied = true;
        for (const string& prereq : allPrereqs) {
            bool completed = student->getCompletedCourses().find(prereq) !=
                student->getCompletedCourses().end();
            cout << "  " << prereq << ": " << (completed ? "T" : "F") << endl;

            if (!completed) allSatisfied = false;
        }

        cout << "\nStrong Induction Conclusion:" << endl;
        if (allSatisfied || allPrereqs.empty()) {
            cout << "All prerequisites satisfied. Student can take " << courseID  << endl;
            return true;
        }
        else {
            cout << "Some prerequisites not satisfied." << endl;
            return false;
        }
    }

    set<string> validateIndirectPrerequisites(const string& courseID) {
        set<string> allPrereqs;
        set<string> visited;
        getAllPrerequisitesRecursive(courseID, allPrereqs, visited);
        return allPrereqs;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 3: INDUCTION & STRONG INDUCTION DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        if (dm->getAllStudents().empty() || dm->getAllCourses().empty()) {
            cout << "Need students and courses to demonstrate." << endl;
            return;
        }

        string studentID = dm->getAllStudents().begin()->first;
        Student* student = dm->getStudent(studentID);

        cout << "Demonstrating with Student: " << studentID
            << " (" << student->getName() << ")" << endl;

        string targetCourse;
        for (auto& pair : dm->getAllCourses()) {
            if (!pair.second.getPrerequisites().empty()) {
                targetCourse = pair.first;
                break;
            }
        }

        if (targetCourse.empty()) {
            cout << "\nNo courses with prerequisites found for demonstration." << endl;
            return;
        }

        verifyPrerequisiteChain(studentID, targetCourse);

        verifyStrongInduction(studentID, targetCourse);
    }
};

