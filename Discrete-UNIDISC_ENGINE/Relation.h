#pragma once

#include "Core.h"
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>

class RelationsModule {
private:
    DataManager* dm;

    typedef pair<string, string> Pair;
    typedef set<Pair> Relation;

    Relation studentCourseRelation;
    Relation facultyCourseRelation;
    Relation courseRoomRelation;
    Relation prerequisiteRelation;

public:
    RelationsModule() {
        dm = DataManager::getInstance();
        buildRelations();
    }

    void buildRelations() {
        studentCourseRelation.clear();
        facultyCourseRelation.clear();
        courseRoomRelation.clear();
        prerequisiteRelation.clear();

        for (auto& pair : dm->getAllStudents()) {
            Student& student = pair.second;
            for (const string& courseID : student.getEnrolledCourses()) {
                studentCourseRelation.insert({ student.getID(), courseID });
            }
        }

        for (auto& pair : dm->getAllFaculty()) {
            Faculty& faculty = pair.second;
            for (const string& courseID : faculty.getAssignedCourses()) {
                facultyCourseRelation.insert({ faculty.getID(), courseID });
            }
        }
        
        for (auto& pair : dm->getAllCourses()) {
            Course& course = pair.second;
            for (const string& prereq : course.getPrerequisites()) {
                prerequisiteRelation.insert({ course.getID(), prereq });
            }
        }
    }

    bool isReflexive(const Relation& R, const set<string>& domain) {
        for (const string& elem : domain) {
            if (R.find({ elem, elem }) == R.end()) {
                return false;
            }
        }
        return true;
    }

    bool isSymmetric(const Relation& R) {
        for (const Pair& p : R) {
            if (R.find({ p.second, p.first }) == R.end()) {
                return false;
            }
        }
        return true;
    }

    bool isTransitive(const Relation& R) {
        for (const Pair& p1 : R) {
            for (const Pair& p2 : R) {
                if (p1.second == p2.first) {
                    if (R.find({ p1.first, p2.second }) == R.end()) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool isEquivalenceRelation(const Relation& R, const set<string>& domain) {
        return isReflexive(R, domain) && isSymmetric(R) && isTransitive(R);
    }

    Relation composeRelations(const Relation& R1, const Relation& R2) {
        Relation result;

        for (const Pair& p1 : R1) {
            for (const Pair& p2 : R2) {
                if (p1.second == p2.first) {
                    result.insert({ p1.first, p2.second });
                }
            }
        }

        return result;
    }

    bool isPartialOrder(const Relation& R, const set<string>& domain) {
     
        if (!isReflexive(R, domain)) return false;

        for (const Pair& p : R) {
            if (p.first != p.second && R.find({ p.second, p.first }) != R.end()) {
                return false;
            }
        }

        if (!isTransitive(R)) return false;

        return true;
    }
    
    void detectIndirectConflicts() {
        cout << "\n=== DETECTING INDIRECT COURSE CONFLICTS ===" << endl;
        cout << "Using relation composition to find hidden conflicts...\n" << endl;
        
        buildRelations();
        
        int conflictCount = 0;
        cout << "--- Type 1: Prerequisite Conflicts ---" << endl;
        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;
            set<string> enrolled = student.getEnrolledCourses();
            
            for (const string& courseID : enrolled) {
                Course* course = dm->getCourse(courseID);
                if (!course) continue;
                
                for (const string& prereq : course->getPrerequisites()) {
                    if (enrolled.find(prereq) != enrolled.end()) {
                        cout << "CONFLICT: Student " << student.getID() 
                             << " enrolled in " << courseID 
                             << " and its prerequisite " << prereq 
                             << " simultaneously!" << endl;
                        conflictCount++;
                    }
                }
            }
        }
        
        cout << "\n--- Type 2: Common Prerequisite Conflicts ---" << endl;
        map<string, set<string>> prereqToAdvanced;
        
        for (auto& coursePair : dm->getAllCourses()) {
            Course& course = coursePair.second;
            for (const string& prereq : course.getPrerequisites()) {
                prereqToAdvanced[prereq].insert(course.getID());
            }
        }
        
        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;
            set<string> enrolled = student.getEnrolledCourses();
            
            for (const auto& prereqPair : prereqToAdvanced) {
                const string& prereq = prereqPair.first;
                const set<string>& advancedCourses = prereqPair.second;
                
                set<string> studentAdvanced;
                for (const string& adv : advancedCourses) {
                    if (enrolled.find(adv) != enrolled.end()) {
                        studentAdvanced.insert(adv);
                    }
                }
                
                if (studentAdvanced.size() > 2) {
                    cout << "INDIRECT CONFLICT: Student " << student.getID() 
                         << " enrolled in " << studentAdvanced.size() 
                         << " courses requiring " << prereq << ": ";
                    for (const string& c : studentAdvanced) {
                        cout << c << " ";
                    }
                    cout << endl;
                    conflictCount++;
                }
            }
        }
        
        cout << "\n--- Type 3: Faculty Assignment Conflicts ---" << endl;
        Relation studentToFaculty = composeRelations(studentCourseRelation, facultyCourseRelation);
        
        map<string, set<string>> studentFacultyMap;
        for (const Pair& p : studentToFaculty) {
            studentFacultyMap[p.first].insert(p.second);
        }
        
        for (const auto& sfPair : studentFacultyMap) {
            if (sfPair.second.size() > 3) {
                cout << "Student " << sfPair.first 
                     << " has courses from " << sfPair.second.size() 
                     << " different faculty (possible scheduling conflict)" << endl;
                conflictCount++;
            }
        }
        
        cout << "\n--- Type 4: Transitive Prerequisite Violations ---" << endl;
        
        Relation transitiveClosure = prerequisiteRelation;
        bool changed = true;
        int iterations = 0;
        const int MAX_ITER = 100;
        
        while (changed && iterations < MAX_ITER) {
            changed = false;
            iterations++;
            Relation newPairs = transitiveClosure;
            
            for (const Pair& p1 : transitiveClosure) {
                for (const Pair& p2 : transitiveClosure) {
                    if (p1.second == p2.first) {
                        Pair newPair = {p1.first, p2.second};
                        if (transitiveClosure.find(newPair) == transitiveClosure.end()) {
                            newPairs.insert(newPair);
                            changed = true;
                        }
                    }
                }
            }
            transitiveClosure = newPairs;
        }
        
        for (auto& studentPair : dm->getAllStudents()) {
            Student& student = studentPair.second;
            
            for (const string& enrolledCourse : student.getEnrolledCourses()) {
                set<string> allPrereqs;
                for (const Pair& p : transitiveClosure) {
                    if (p.first == enrolledCourse) {
                        allPrereqs.insert(p.second);
                    }
                }
                
                // Check if all are completed
                for (const string& prereq : allPrereqs) {
                    if (student.getCompletedCourses().find(prereq) == student.getCompletedCourses().end()) {
                        cout << "TRANSITIVE VIOLATION: Student " << student.getID()
                             << " enrolled in " << enrolledCourse
                             << " without completing indirect prerequisite " << prereq << endl;
                        conflictCount++;
                    }
                }
            }
        }
        
        cout << "\n========================================" << endl;
        if (conflictCount == 0) {
            cout << "No indirect conflicts detected!" << endl;
        } else {
            cout << "Total indirect conflicts found: " << conflictCount << endl;
        }
        cout << "========================================" << endl;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 6: RELATIONS DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        buildRelations();

        cout << "Student-Course Relation: " << studentCourseRelation.size() << " pairs" << endl;
        cout << "Faculty-Course Relation: " << facultyCourseRelation.size() << " pairs" << endl;
        cout << "Prerequisite Relation: " << prerequisiteRelation.size() << " pairs" << endl;

        cout << "\n--- Sample Student-Course Pairs ---" << endl;
        int count = 0;
        for (const Pair& p : studentCourseRelation) {
            cout << "(" << p.first << ", " << p.second << ")" << endl;
            if (++count >= 5) break;
        }

        cout << "\n--- Relation Properties ---" << endl;
        set<string> students;
        for (auto& pair : dm->getAllStudents()) {
            students.insert(pair.first);
        }

        cout << "Student-Course Relation:" << endl;
        cout << "  Reflexive: " << (isReflexive(studentCourseRelation, students) ? "Yes" : "No") << endl;
        cout << "  Symmetric: " << (isSymmetric(studentCourseRelation) ? "Yes" : "No") << endl;
        cout << "  Transitive: " << (isTransitive(studentCourseRelation) ? "Yes" : "No") << endl;
        
        cout << "\nPrerequisite Relation:" << endl;
        set<string> courses;
        for (auto& pair : dm->getAllCourses()) {
            courses.insert(pair.first);
        }
        cout << "  Is Partial Order: " << (isPartialOrder(prerequisiteRelation, courses) ? "Yes (with reflexive pairs)" : "No") << endl;

        if (!studentCourseRelation.empty() && !facultyCourseRelation.empty()) {
            Relation composed = composeRelations(studentCourseRelation, facultyCourseRelation);
            cout << "\n--- Composition: Student -> Course -> Faculty ---" << endl;
            cout << "Composed relation size: " << composed.size() << " pairs" << endl;
            
            cout << "Sample composed pairs:" << endl;
            int count2 = 0;
            for (const Pair& p : composed) {
                cout << "  Student " << p.first << " -> Faculty " << p.second << endl;
                if (++count2 >= 5) break;
            }
        }
        
        detectIndirectConflicts();
    }
};