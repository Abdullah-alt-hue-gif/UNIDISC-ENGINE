#pragma once

#include "Core.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <chrono>

using namespace std::chrono;

class ProofModule {
private:
    DataManager* dm;

    void printProofStep(int step, const string& statement) {
        cout << "  Step " << step << ": " << statement << endl;
    }

public:
    ProofModule() {
        dm = DataManager::getInstance();
    }

    // Generate step-by-step proof for prerequisite satisfaction
    bool generatePrerequisiteProof(const string& studentID, const string& courseID) {
        cout << "\n=== FORMAL PROOF ===" << endl;
        cout << "Theorem: Student " << studentID << " can enroll in " << courseID << endl;
        cout << "\nProof:" << endl;

        Student* student = dm->getStudent(studentID);
        Course* course = dm->getCourse(courseID);

        if (!student || !course) {
            printProofStep(1, "Invalid student or course - Proof fails ✗");
            return false;
        }

        int step = 1;
        printProofStep(step++, "Let S = " + studentID + " (student)");
        printProofStep(step++, "Let C = " + courseID + " (target course)");

        set<string> prerequisites = course->getPrerequisites();

        if (prerequisites.empty()) {
            printProofStep(step++, "Prerequisites(C) = {}(empty set)");
            printProofStep(step++, "By definition, any student can enroll in courses with no prerequisites");
            cout << "\nHence, Theorem is proven. QED" << endl;
            return true;
        }

        printProofStep(step++, "Prerequisites(C) = {" +
            [&]() {
                string result;
                bool first = true;
                for (const string& p : prerequisites) {
                    if (!first) result += ", ";
                    result += p;
                    first = false;
                }
                return result;
            }() + "}");

        printProofStep(step++, "Must verify: ∀p in Prerequisites(C), p in Completed(S)");

        set<string> completed = student->getCompletedCourses();

        bool allSatisfied = true;
        for (const string& prereq : prerequisites) {
            bool hasCompleted = completed.find(prereq) != completed.end();

            printProofStep(step++, prereq + (hasCompleted ? " in Completed(S)" : " not in Completed(S)"));

            if (!hasCompleted) allSatisfied = false;
        }

        if (allSatisfied) {
            printProofStep(step++, "All prerequisites satisfied");
            printProofStep(step++, "By enrollment policy, student can enroll");
            cout << "\nHence, Theorem is proven. QED" << endl;
            return true;
        }
        else {
            printProofStep(step++, "Not all prerequisites satisfied");
            cout << "\nHence, Theorem is disproven." << endl;
            return false;
        }
    }
    bool verifyConsistency(const vector<string>& courseSequence) {
        cout << "\n=== CONSISTENCY VERIFICATION ===" << endl;
        cout << "Verifying sequence: ";
        for (size_t i = 0; i < courseSequence.size(); i++) {
            if (i > 0) cout << " → ";
            cout << courseSequence[i];
        }
        cout << "\n" << endl;

        set<string> completed;

        for (size_t i = 0; i < courseSequence.size(); i++) {
            const string& courseID = courseSequence[i];
            Course* course = dm->getCourse(courseID);

            if (!course) {
                cout << "Position " << (i + 1) << ": Course " << courseID << " not found" << endl;
                return false;
            }

            cout << "Position " << (i + 1) << ": " << courseID << endl;

            for (const string& prereq : course->getPrerequisites()) {
                if (completed.find(prereq) == completed.end()) {
                    cout << "  ERROR: Prerequisite " << prereq << " not completed" << endl;
                    return false;
                }
                cout << "  Prerequisite " << prereq << " satisfied" << endl;
            }

            completed.insert(courseID);
        }

        cout << "\nSequence is consistent" << endl;
        return true;
    }

    bool validateLogicRules() {
        cout << "\n=== LOGIC RULES VALIDATION ===" << endl;

        bool valid = true;
        int ruleCount = 0;

        for (auto& pair : dm->getAllCourses()) {
            Course& course = pair.second;
            set<string> visited;

            function<bool(const string&)> hasCycle = [&](const string& cid) -> bool {
                if (visited.find(cid) != visited.end()) return true;
                visited.insert(cid);

                Course* c = dm->getCourse(cid);
                if (!c) return false;

                for (const string& prereq : c->getPrerequisites()) {
                    if (hasCycle(prereq)) return true;
                }

                visited.erase(cid);
                return false;
                };

            if (hasCycle(course.getID())) {
                cout << "CYCLE DETECTED in prerequisites for " << course.getID()  << endl;
                valid = false;
            }
            else {
                ruleCount++;
            }
        }

        cout << "Validated " << ruleCount << " prerequisite rules" << endl;
        cout << "Result: " << (valid ? "All rules valid" : "Some rules invalid") << endl;

        return valid;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 8: AUTOMATED PROOF & VERIFICATION DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        if (!dm->getAllStudents().empty() && !dm->getAllCourses().empty()) {
            string studentID = dm->getAllStudents().begin()->first;
            string courseID = dm->getAllCourses().begin()->first;

            generatePrerequisiteProof(studentID, courseID);
        }

        validateLogicRules();
    }
};