#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "Core.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class SchedulingModule {
private:
    DataManager* dm;
    map<string, vector<vector<string>>> memoizedSequences;
    
    void getAllPrerequisites(const string& courseID, set<string>& allPrereqs) {
        Course* course = dm->getCourse(courseID);
        if (!course) return;
        
        for (const string& prereq : course->getPrerequisites()) {
            if (allPrereqs.find(prereq) == allPrereqs.end()) {
                allPrereqs.insert(prereq);
                getAllPrerequisites(prereq, allPrereqs);
            }
        }
    }

    void generateSequencesRecursive(
        const set<string>& remaining,
        const set<string>& completed,
        vector<string>& currentSequence,
        vector<vector<string>>& allSequences,
        int maxDepth
    ) {
        if (remaining.empty() || maxDepth == 0) {
            if (!currentSequence.empty()) {
                allSequences.push_back(currentSequence);
            }
            return;
        }
        
        vector<string> eligible;
        for (const string& courseID : remaining) {
            Course* course = dm->getCourse(courseID);
            if (!course) continue;
            
            bool canTake = true;
            for (const string& prereq : course->getPrerequisites()) {
                if (completed.find(prereq) == completed.end()) {
                    canTake = false;
                    break;
                }
            }
            
            if (canTake) {
                eligible.push_back(courseID);
            }
        }
        
        for (const string& courseID : eligible) {
            set<string> newRemaining = remaining;
            newRemaining.erase(courseID);
            
            set<string> newCompleted = completed;
            newCompleted.insert(courseID);
            
            vector<string> newSequence = currentSequence;
            newSequence.push_back(courseID);
            
            generateSequencesRecursive(newRemaining, newCompleted, newSequence, allSequences, maxDepth - 1);
        }
    }
    
public:
    SchedulingModule() {
        dm = DataManager::getInstance();
    }
    
    void viewCoursesAndPrerequisites() {
        cout << "\n=== All Courses and Prerequisites ===" << endl;
        
        if (dm->getAllCourses().empty()) {
            cout << "No courses in the system." << endl;
            return;
        }
        
        for (auto& pair : dm->getAllCourses()) {
            Course& course = pair.second;
            cout << "\nCourse: " << course.getID() << " - " << course.getName() 
                 << " (" << course.getCredits() << " credits)" << endl;
            
            if (!course.getPrerequisites().empty()) {
                cout << "Prerequisites: ";
                bool first = true;
                for (const string& prereq : course.getPrerequisites()) {
                    if (!first) cout << ", ";
                    cout << prereq;
                    first = false;
                }
                cout << endl;
            } else {
                cout << "Prerequisites: None" << endl;
            }
        }
    }
    
    vector<vector<string>> generateValidSequences(const set<string>& courseIDs, int maxLength = 10) {
        vector<vector<string>> allSequences;
        vector<string> currentSequence;
        set<string> completed;
        
        generateSequencesRecursive(courseIDs, completed, currentSequence, allSequences, maxLength);
        
        return allSequences;
    }
    
    bool checkEligibility(const string& studentID, const string& courseID) {
        Student* student = dm->getStudent(studentID);
        Course* course = dm->getCourse(courseID);
        
        if (!student || !course) {
            cout << "Invalid student or course ID" << endl;
            return false;
        }
        
        cout << "\n=== Checking Eligibility ===" << endl;
        cout << "Student: " << student->getName() << " (" << studentID << ")" << endl;
        cout << "Course: " << course->getName() << " (" << courseID << ")" << endl;
        
        if (student->getEnrolledCourses().find(courseID) != student->getEnrolledCourses().end()) {
            cout << "\nStudent is already enrolled in this course!" << endl;
            return false;
        }
        
        if (student->getCompletedCourses().find(courseID) != student->getCompletedCourses().end()) {
            cout << "\nStudent has already completed this course!" << endl;
            return false;
        }
        
        cout << "\n--- Prerequisite Check ---" << endl;
        bool eligible = true;
        
        if (course->getPrerequisites().empty()) {
            cout << "No prerequisites required" << endl;
        } else {
        
        if (eligible) {
            cout << "\nStudent IS ELIGIBLE for " << courseID << endl;
        } else {
            for (const string& prereq : course->getPrerequisites()) {
                if (student->getCompletedCourses().find(prereq) == student->getCompletedCourses().end()) {
                    cout << "Missing prerequisite: " << prereq << endl;
                    eligible = false;
                } else {
                    cout << "Completed prerequisite: " << prereq << endl;
                }
            }
        }
            cout << "\nStudent IS NOT ELIGIBLE for " << courseID << endl;
            cout << "   Student must complete all prerequisites first!" << endl;
        }
        
        return eligible;
    }
    
    vector<string> predictAvailableCourses(const string& studentID) {
        Student* student = dm->getStudent(studentID);
        if (!student) {
            cout << "Student not found." << endl;
            return vector<string>();
        }
        
        vector<string> available;
        
        for (auto& pair : dm->getAllCourses()) {
            const string& courseID = pair.first;
            Course& course = pair.second;
            
            if (student->getEnrolledCourses().find(courseID) != student->getEnrolledCourses().end() ||
                student->getCompletedCourses().find(courseID) != student->getCompletedCourses().end()) {
                continue;
            }
            
            bool canTake = true;
            for (const string& prereq : course.getPrerequisites()) {
                if (student->getCompletedCourses().find(prereq) == student->getCompletedCourses().end()) {
                    canTake = false;
                    break;
                }
            }
            
            if (canTake) {
                available.push_back(courseID);
            }
        }
        
        return available;
    }
    
    vector<string> topologicalSort(const set<string>& courseIDs) {
        map<string, int> inDegree;
        map<string, vector<string>> adjList;
        
        for (const string& courseID : courseIDs) {
            inDegree[courseID] = 0;
            adjList[courseID] = vector<string>();
        }
        
        for (const string& courseID : courseIDs) {
            Course* course = dm->getCourse(courseID);
            if (!course) continue;
            
            for (const string& prereq : course->getPrerequisites()) {
                if (courseIDs.find(prereq) != courseIDs.end()) {
                    adjList[prereq].push_back(courseID);
                    inDegree[courseID]++;
                }
            }
        }
        
        vector<string> result;
        vector<string> queue;
        
        for (auto& pair : inDegree) {
            if (pair.second == 0) {
                queue.push_back(pair.first);
            }
        }
        
        while (!queue.empty()) {
            string current = queue.back();
            queue.pop_back();
            result.push_back(current);
            
            for (const string& next : adjList[current]) {
                inDegree[next]--;
                if (inDegree[next] == 0) {
                    queue.push_back(next);
                }
            }
        }
        
        if (result.size() != courseIDs.size()) {
            cout << "Warning: Circular dependency detected!" << endl;
        }
        
        return result;
    }
    
    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 1: COURSE & SCHEDULING DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;
        
        viewCoursesAndPrerequisites();
        
        if (!dm->getAllStudents().empty() && !dm->getAllCourses().empty()) {
            string studentID = dm->getAllStudents().begin()->first;
            
            if (dm->getAllCourses().size() > 0) {
                string courseID = dm->getAllCourses().begin()->first;
                checkEligibility(studentID, courseID);
            }
            
            cout << "\n--- Available Courses for Student " << studentID << " ---" << endl;
            vector<string> available = predictAvailableCourses(studentID);
            
            if (available.empty()) {
                cout << "No available courses at this time." << endl;
            } else {
                for (const string& courseID : available) {
                    Course* course = dm->getCourse(courseID);
                    if (course) {
                        cout << "✓ " << courseID << ": " << course->getName() << endl;
                    }
                }
            }
            
            if (dm->getAllCourses().size() >= 3) {
                set<string> sampleCourses;
                int count = 0;
                for (auto& pair : dm->getAllCourses()) {
                    sampleCourses.insert(pair.first);
                    if (++count >= 4) break;
                }
                
                cout << "\n--- Sample Valid Course Sequences ---" << endl;
                cout << "Generating sequences for: ";
                bool first = true;
                for (const string& c : sampleCourses) {
                    if (!first) cout << ", ";
                    cout << c;
                    first = false;
                }
                cout << endl;
                
                vector<vector<string>> sequences = generateValidSequences(sampleCourses, 5);
                
                int displayCount = min(5, (int)sequences.size());
                for (int i = 0; i < displayCount; i++) {
                    cout << "Sequence " << (i + 1) << ": ";
                    for (size_t j = 0; j < sequences[i].size(); j++) {
                        if (j > 0) cout << " -> ";
                        cout << sequences[i][j];
                    }
                    cout << endl;
                }
                
                if (sequences.empty()) {
                    cout << "No valid sequences found (check for circular dependencies)" << endl;
                } else {
                    cout << "Total sequences generated: " << sequences.size() << endl;
                }
                
                cout << "\n--- Topological Sort (Optimal Order) ---" << endl;
                vector<string> sortedCourses = topologicalSort(sampleCourses);
                cout << "Recommended order: ";
                for (size_t i = 0; i < sortedCourses.size(); i++) {
                    if (i > 0) cout << " --> ";
                    cout << sortedCourses[i];
                }
                cout << endl;
            }
        } else {
            cout << "\nPlease add students and courses first to see full demonstration." << endl;
        }
        
        cout << "\n========================================" << endl;
    }
};

#endif