#ifndef LOGIC_ENGINE_H
#define LOGIC_ENGINE_H

#include "Core.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>


struct LogicRule {
    string id;
    string antecedent; 
    string consequent; 
    string type; 

    LogicRule(string i, string a, string c, string t)
        : id(i), antecedent(a), consequent(c), type(t) {
    }
};

class LogicEngine {
private:
    DataManager* dm;
    vector<LogicRule> rules;
    set<string> facts; 
    map<string, string> conflicts;

    pair<string, vector<string>> parsePredicate(const string& pred) {
        size_t openParen = pred.find('(');
        size_t closeParen = pred.find(')');

        if (openParen == string::npos || closeParen == string::npos) {
            return { "", {} };
        }

        string predName = pred.substr(0, openParen);
        string argsStr = pred.substr(openParen + 1, closeParen - openParen - 1);

        vector<string> args;
        stringstream ss(argsStr);
        string arg;

        while (getline(ss, arg, ',')) {
        
            arg.erase(0, arg.find_first_not_of(" \t"));
            arg.erase(arg.find_last_not_of(" \t") + 1);
            args.push_back(arg);
        }

        return { predName, args };
    }

    bool matchesFact(const string& pattern) {
        return facts.find(pattern) != facts.end();
    }

public:
    LogicEngine() {
        dm = DataManager::getInstance();
    }

    void addCourseRule(const string& courseID, const string& prerequisite) {
        string ruleID = "CR_" + courseID + "_" + prerequisite;
        string antecedent = "enrolled(" + courseID + ")";
        string consequent = "must_complete(" + prerequisite + ")";

        rules.push_back(LogicRule(ruleID, antecedent, consequent, "prerequisite"));

        cout << "Added rule: IF " << antecedent << " THEN " << consequent << endl;
    }

    void addFacultyRule(const string& facultyID, const string& courseID, const string& roomID) {
        string ruleID = "FR_" + facultyID + "_" + courseID;
        string antecedent = "teaches(" + facultyID + ", " + courseID + ")";
        string consequent = "must_use_room(" + courseID + ", " + roomID + ")";

        rules.push_back(LogicRule(ruleID, antecedent, consequent, "faculty"));

        cout << "Added rule: IF " << antecedent << " THEN " << consequent << endl;
    }

    void addGeneralRule(const string& ruleID, const string& antecedent,
        const string& consequent, const string& type = "general") {
        rules.push_back(LogicRule(ruleID, antecedent, consequent, type));
        cout << "Added rule " << ruleID << ": IF " << antecedent
            << " THEN " << consequent << endl;
    }

    void addFact(const string& fact) {
        facts.insert(fact);
        cout << "Added fact: " << fact << endl;
    }

    void removeFact(const string& fact) {
        facts.erase(fact);
    }


    set<string> runInference() {
        cout << "\n--- Running Forward Chaining Inference ---" << endl;

        set<string> derived;
        bool changed = true;
        int iteration = 0;

        while (changed && iteration < 100) { 
            changed = false;
            iteration++;

            for (const LogicRule& rule : rules) {
                if (matchesFact(rule.antecedent) &&
                    facts.find(rule.consequent) == facts.end() &&
                    derived.find(rule.consequent) == derived.end()) {

                    derived.insert(rule.consequent);
                    facts.insert(rule.consequent);
                    changed = true;

                    cout << "Iteration " << iteration << ": Applied rule " << rule.id
                        << " -> Derived: " << rule.consequent << endl;
                }
            }
        }

        if (derived.empty()) {
            cout << "No new facts derived." << endl;
        }
        else {
            cout << "\nTotal facts derived: " << derived.size() << endl;
        }

        return derived;
    }

    void detectConflicts() {
        cout << "\n--- Detecting Conflicts ---" << endl;
        conflicts.clear();

        map<string, set<string>> studentSchedules;

        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;
            string studentID = student.getID();

            for (const string& courseID : student.getEnrolledCourses()) {
                studentSchedules[studentID].insert(courseID);
            }
        }
        for (auto& facultyPair : dm->getAllFaculty()) {
            Faculty& faculty = facultyPair.second;

            if (faculty.getAssignedCourses().size() > faculty.getMaxCourses()) {
                string conflict = "Faculty " + faculty.getID() + " overloaded: " +
                    to_string(faculty.getAssignedCourses().size()) +
                    " courses (max: " + to_string(faculty.getMaxCourses()) + ")";
                conflicts[faculty.getID()] = conflict;
                cout << "CONFLICT: " << conflict << endl;
            }
        }

        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;

            for (const string& courseID : student.getEnrolledCourses()) {
                Course* course = dm->getCourse(courseID);
                if (!course) continue;

                for (const string& prereq : course->getPrerequisites()) {
                    if (student.getCompletedCourses().find(prereq) ==
                        student.getCompletedCourses().end()) {

                        string conflict = "Student " + student.getID() +
                            " enrolled in " + courseID +
                            " without completing prerequisite " + prereq;
                        conflicts[student.getID() + "_" + courseID] = conflict;
                        cout << "CONFLICT: " << conflict << endl;
                    }
                }
            }
        }

        if (conflicts.empty()) {
            cout << "No conflicts detected." << endl;
        }
    }

    void viewRules() {
        cout << "\n=== All Logic Rules ===" << endl;

        if (rules.empty()) {
            cout << "No rules defined." << endl;
            return;
        }

        map<string, vector<LogicRule>> rulesByType;
        for (const LogicRule& rule : rules) {
            rulesByType[rule.type].push_back(rule);
        }

        for (auto& pair : rulesByType) {
            cout << "\n" << pair.first << " rules:" << endl;
            for (const LogicRule& rule : pair.second) {
                cout << "  [" << rule.id << "] IF " << rule.antecedent
                    << " THEN " << rule.consequent << endl;
            }
        }
    }

    void viewFacts() {
        cout << "\n=== Known Facts ===" << endl;

        if (facts.empty()) {
            cout << "No facts in knowledge base." << endl;
            return;
        }

        int count = 1;
        for (const string& fact : facts) {
            cout << count++ << ". " << fact << endl;
        }
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 4: LOGIC & INFERENCE ENGINE DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        
        cout << "Creating rules from system data..." << endl;

        
        for (auto& coursePair : dm->getAllCourses()) {
            Course& course = coursePair.second;
            for (const string& prereq : course.getPrerequisites()) {
                addCourseRule(course.getID(), prereq);
            }
        }

        
        for (auto& facultyPair : dm->getAllFaculty()) {
            Faculty& faculty = facultyPair.second;
            for (const string& courseID : faculty.getAssignedCourses()) {
                
                if (!dm->getAllRooms().empty()) {
                    string roomID = dm->getAllRooms().begin()->first;
                    addFacultyRule(faculty.getID(), courseID, roomID);
                }
            }
        }

        
        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;
            for (const string& courseID : student.getEnrolledCourses()) {
                addFact("enrolled(" + courseID + ")");
            }
        }

        viewRules();
        viewFacts();

        runInference();

        detectConflicts();
    }
};

#endif 