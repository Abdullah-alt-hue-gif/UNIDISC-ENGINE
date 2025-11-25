#include <iostream>
#include <string>
#include <limits>
#include "Core.h"
#include "Induction.h"
#include "LogicInference.h"
#include "AutomatedProofVerification.h"
#include "AlgorithmicEfficiency.h"
#include "Set.h"
#include "Relation.h"
#include "Function.h"
#include "ConsistencyChecker.h"
#include "StudentGroupCombination.h"
#include "CourseScheduling.h"
#include "UnitTesting.h"

using namespace std;

DataManager* DataManager::instance = nullptr;

class UnidiscSystem {
private:
    DataManager* dm;
    SchedulingModule schedulingModule;
    CombinationsModule combinationsModule;
    InductionModule inductionModule;
    LogicEngine logicEngine;
    SetOperationsModule setOperationsModule;
    RelationsModule relationsModule;
    FunctionsModule functionsModule;
    ProofModule proofModule;
    ConsistencyChecker consistencyChecker;
    EfficiencyModule efficiencyModule;
    UnitTestingModule unitTestingModule;

    void clearScreen() {
        system("cls");
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void printHeader(const string& title) {
        cout << "\n";
        cout << "============================================================" << endl;
        cout << title << endl;
        cout << "============================================================" << endl;
    }

    void loadSampleData() {
        Course cs101("CS101", "Intro to Programming", 3);
        Course cs102("CS102", "Data Structures", 3);
        cs102.addPrerequisite("CS101");
        Course cs201("CS201", "Algorithms", 3);
        cs201.addPrerequisite("CS102");
        Course math101("MATH101", "Calculus I", 4);
        Course math102("MATH102", "Calculus II", 4);
        math102.addPrerequisite("MATH101");

        dm->addCourse(cs101);
        dm->addCourse(cs102);
        dm->addCourse(cs201);
        dm->addCourse(math101);
        dm->addCourse(math102);

        Student s1("S101", "Ali Ahmed");
        s1.completeCourse("CS101", 3);
        s1.enrollCourse("CS102", 3);

        Student s2("S102", "Fatima Khan");
        s2.enrollCourse("CS101", 3);
        s2.enrollCourse("MATH101", 4);

        Student s3("S103", "Hassan Ali");
        s3.completeCourse("CS101", 3);
        s3.completeCourse("CS102", 3);
        s3.enrollCourse("CS201", 3);

        dm->addStudent(s1);
        dm->addStudent(s2);
        dm->addStudent(s3);

        Faculty f1("F101", "Ali", 3);
        f1.assignCourse("CS101");
        Faculty f2("F102", "Ahmad", 3);
        f2.assignCourse("CS102");

        dm->addFaculty(f1);
        dm->addFaculty(f2);

        Room r1("R101", 50, "Lecture");
        Room r2("R102", 30, "Lab");

        dm->addRoom(r1);
        dm->addRoom(r2);

        Lab lab1("LAB1", "CS101", 20);
        Lab lab2("LAB2", "CS102", 20);

        dm->addLab(lab1);
        dm->addLab(lab2);

        cout << "Sample data loaded successfully!" << endl;
    }

    void courseSchedulingMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 1: COURSE & SCHEDULING");
            
            cout << "\n1. View All Courses and Prerequisites" << endl;
            cout << "2. Check Student Eligibility for Course" << endl;
            cout << "3. Predict Available Courses for Student" << endl;
            cout << "4. Generate Valid Course Sequences" << endl;
            cout << "5. Topological Sort (Optimal Course Order)" << endl;
            cout << "6. Enroll Student in Course (with Prerequisite Check)" << endl;
            cout << "7. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1:
                    schedulingModule.viewCoursesAndPrerequisites();
                    pause();
                    break;
                    
                case 2: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    schedulingModule.checkEligibility(studentID, courseID);
                    pause();
                    break;
                }
                
                case 3: {
                    string studentID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "\n--- Available Courses ---" << endl;
                    vector<string> available = schedulingModule.predictAvailableCourses(studentID);
                    if (available.empty()) {
                        cout << "No courses available at this time." << endl;
                    } else {
                        for (const string& courseID : available) {
                            Course* course = dm->getCourse(courseID);
                            if (course) {
                                cout << courseID << ": " << course->getName() << endl;
                            }
                        }
                    }
                    pause();
                    break;
                }
                
                case 4: {
                    set<string> courses;
                    cout << "Enter number of courses to include: ";
                    int n;
                    cin >> n;
                    cin.ignore();
                    
                    for (int i = 0; i < n; i++) {
                        string courseID;
                        cout << "Enter Course ID " << (i+1) << ": ";
                        getline(cin, courseID);
                        courses.insert(courseID);
                    }
                    
                    vector<vector<string>> sequences = schedulingModule.generateValidSequences(courses, 10);
                    cout << "\n--- Valid Sequences ---" << endl;
                    int displayCount = min(10, (int)sequences.size());
                    for (int i = 0; i < displayCount; i++) {
                        cout << "Sequence " << (i + 1) << ": ";
                        for (size_t j = 0; j < sequences[i].size(); j++) {
                            if (j > 0) cout << " → ";
                            cout << sequences[i][j];
                        }
                        cout << endl;
                    }
                    cout << "Total sequences: " << sequences.size() << endl;
                    pause();
                    break;
                }
                
                case 5: {
                    set<string> courses;
                    cout << "Enter number of courses: ";
                    int n;
                    cin >> n;
                    cin.ignore();
                    
                    for (int i = 0; i < n; i++) {
                        string courseID;
                        cout << "Enter Course ID " << (i+1) << ": ";
                        getline(cin, courseID);
                        courses.insert(courseID);
                    }
                    
                    vector<string> sorted = schedulingModule.topologicalSort(courses);
                    cout << "\n--- Topological Sort (Recommended Order) ---" << endl;
                    for (size_t i = 0; i < sorted.size(); i++) {
                        if (i > 0) cout << " --> ";
                        cout << sorted[i];
                    }
                    cout << endl;
                    pause();
                    break;
                }
                
                case 6: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID to enroll: ";
                    getline(cin, courseID);
                    
                    Student* student = dm->getStudent(studentID);
                    Course* course = dm->getCourse(courseID);
                    
                    if (!student || !course) {
                        cout << "Invalid student or course!" << endl;
                    } else if (schedulingModule.checkEligibility(studentID, courseID)) {
                        student->enrollCourse(courseID, course->getCredits());
                        cout << "\nStudent successfully enrolled in " << courseID << endl;
                    } else {
                        cout << "\nEnrollment denied: Prerequisites not met!" << endl;
                    }
                    pause();
                    break;
                }
                
                case 7:
                    schedulingModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    
    void combinationsMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 2: STUDENT GROUP COMBINATIONS");
            
            cout << "\n1. Form Project Groups" << endl;
            cout << "2. Assign Students to Lab Groups" << endl;
            cout << "3. Assign Students to Electives" << endl;
            cout << "4. Calculate Total Combinations" << endl;
            cout << "5. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    int groupSize;
                    cout << "Enter group size: ";
                    cin >> groupSize;
                    cin.ignore();
                    
                    vector<vector<string>> groups = combinationsModule.formProjectGroups(groupSize);
                    cout << "\n--- Project Groups (Size " << groupSize << ") ---" << endl;
                    int displayCount = min(10, (int)groups.size());
                    for (int i = 0; i < displayCount; i++) {
                        cout << "Group " << (i + 1) << ": ";
                        for (size_t j = 0; j < groups[i].size(); j++) {
                            if (j > 0) cout << ", ";
                            cout << groups[i][j];
                        }
                        cout << endl;
                    }
                    cout << "Total groups possible: " << groups.size() << endl;
                    pause();
                    break;
                }
                
                case 2: {
                    string courseID;
                    int studentsPerLab;
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    cout << "Enter students per lab: ";
                    cin >> studentsPerLab;
                    cin.ignore();
                    
                    map<string, vector<string>> assignments = combinationsModule.assignLabGroups(courseID, studentsPerLab);
                    cout << "\n--- Lab Assignments for " << courseID << " ---" << endl;
                    for (auto& pair : assignments) {
                        cout << "Lab " << pair.first << ": ";
                        for (size_t i = 0; i < pair.second.size(); i++) {
                            if (i > 0) cout << ", ";
                            cout << pair.second[i];
                        }
                        cout << " (" << pair.second.size() << " students)" << endl;
                    }
                    pause();
                    break;
                }
                
                case 3: {
                    vector<string> electives;
                    int numElectives, maxPerElective;
                    
                    cout << "Enter number of elective courses: ";
                    cin >> numElectives;
                    cin.ignore();
                    
                    for (int i = 0; i < numElectives; i++) {
                        string courseID;
                        cout << "Enter Elective Course ID " << (i+1) << ": ";
                        getline(cin, courseID);
                        electives.push_back(courseID);
                    }
                    
                    cout << "Enter max students per elective: ";
                    cin >> maxPerElective;
                    cin.ignore();
                    
                    map<string, vector<string>> assignments = combinationsModule.assignElectives(electives, maxPerElective);
                    cout << "\n--- Elective Assignments ---" << endl;
                    for (auto& pair : assignments) {
                        cout << "Course " << pair.first << ": ";
                        for (size_t i = 0; i < pair.second.size(); i++) {
                            if (i > 0) cout << ", ";
                            cout << pair.second[i];
                        }
                        cout << " (" << pair.second.size() << " students)" << endl;
                    }
                    pause();
                    break;
                }
                
                case 4: {
                    int n, r;
                    cout << "Enter n (total items): ";
                    cin >> n;
                    cout << "Enter r (group size): ";
                    cin >> r;
                    cin.ignore();
                    
                    unsigned long long result = combinationsModule.calculateTotalCombinations(n, r);
                    cout << "\nC(" << n << ", " << r << ") = " << result << endl;
                    pause();
                    break;
                }
                
                case 5:
                    combinationsModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }


    void inductionMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 3: MATHEMATICAL INDUCTION");
            
            cout << "\n1. Verify Base Case" << endl;
            cout << "2. Verify Inductive Step" << endl;
            cout << "3. Verify Prerequisite Chain (Full Induction)" << endl;
            cout << "4. Verify Strong Induction" << endl;
            cout << "5. Validate Indirect Prerequisites" << endl;
            cout << "6. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    inductionModule.verifyBaseCase(studentID, courseID);
                    pause();
                    break;
                }
                
                case 2: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    inductionModule.verifyInductiveStep(studentID, courseID);
                    pause();
                    break;
                }
                
                case 3: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    inductionModule.verifyPrerequisiteChain(studentID, courseID);
                    pause();
                    break;
                }
                
                case 4: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    inductionModule.verifyStrongInduction(studentID, courseID);
                    pause();
                    break;
                }
                
                case 5: {
                    string courseID;
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    set<string> prereqs = inductionModule.validateIndirectPrerequisites(courseID);
                    cout << "\n--- All Prerequisites (Direct + Indirect) ---" << endl;
                    for (const string& p : prereqs) {
                        cout << "  - " << p << endl;
                    }
                    cout << "Total prerequisites: " << prereqs.size() << endl;
                    pause();
                    break;
                }
                
                case 6:
                    inductionModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    
    void logicInferenceMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 4: LOGIC & INFERENCE ENGINE");
            
            cout << "\n1. Add Course Prerequisite Rule" << endl;
            cout << "2. Add Faculty Assignment Rule" << endl;
            cout << "3. Add General Rule" << endl;
            cout << "4. Add Fact to Knowledge Base" << endl;
            cout << "5. View All Rules" << endl;
            cout << "6. View All Facts" << endl;
            cout << "7. Run Forward Chaining Inference" << endl;
            cout << "8. Detect Conflicts" << endl;
            cout << "9. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    string courseID, prereq;
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    cout << "Enter Prerequisite: ";
                    getline(cin, prereq);
                    logicEngine.addCourseRule(courseID, prereq);
                    pause();
                    break;
                }
                
                case 2: {
                    string facultyID, courseID, roomID;
                    cout << "Enter Faculty ID: ";
                    getline(cin, facultyID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    cout << "Enter Room ID: ";
                    getline(cin, roomID);
                    logicEngine.addFacultyRule(facultyID, courseID, roomID);
                    pause();
                    break;
                }
                
                case 3: {
                    string ruleID, antecedent, consequent;
                    cout << "Enter Rule ID: ";
                    getline(cin, ruleID);
                    cout << "Enter Antecedent (IF part): ";
                    getline(cin, antecedent);
                    cout << "Enter Consequent (THEN part): ";
                    getline(cin, consequent);
                    logicEngine.addGeneralRule(ruleID, antecedent, consequent);
                    pause();
                    break;
                }
                
                case 4: {
                    string fact;
                    cout << "Enter Fact: ";
                    getline(cin, fact);
                    logicEngine.addFact(fact);
                    pause();
                    break;
                }
                
                case 5:
                    logicEngine.viewRules();
                    pause();
                    break;
                    
                case 6:
                    logicEngine.viewFacts();
                    pause();
                    break;
                    
                case 7:
                    logicEngine.runInference();
                    pause();
                    break;
                    
                case 8:
                    logicEngine.detectConflicts();
                    pause();
                    break;
                    
                case 9:
                    logicEngine.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void setOperationsMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 5: SET OPERATIONS");
            
            cout << "\n1. View Student Set" << endl;
            cout << "2. View Course Set" << endl;
            cout << "3. Find Students in Multiple Courses (Intersection)" << endl;
            cout << "4. Find Students in Any Course (Union)" << endl;
            cout << "5. Find Students in Course A but not B (Difference)" << endl;
            cout << "6. Generate Power Set of Students" << endl;
            cout << "7. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    set<string> students = setOperationsModule.getStudentSet();
                    cout << "\n--- Student Set ---" << endl;
                    cout << "Total Students: " << students.size() << endl;
                    for (const string& s : students) {
                        cout << "  - " << s << endl;
                    }
                    pause();
                    break;
                }
                
                case 2: {
                    set<string> courses = setOperationsModule.getCourseSet();
                    cout << "\n--- Course Set ---" << endl;
                    cout << "Total Courses: " << courses.size() << endl;
                    for (const string& c : courses) {
                        cout << "  - " << c << endl;
                    }
                    pause();
                    break;
                }
                
                case 3: {
                    vector<string> courseIDs;
                    int n;
                    cout << "Enter number of courses: ";
                    cin >> n;
                    cin.ignore();
                    
                    for (int i = 0; i < n; i++) {
                        string courseID;
                        cout << "Enter Course ID " << (i+1) << ": ";
                        getline(cin, courseID);
                        courseIDs.push_back(courseID);
                    }
                    
                    set<string> result = setOperationsModule.studentsInMultipleCourses(courseIDs);
                    cout << "\n--- Students in ALL specified courses (Intersection) ---" << endl;
                    for (const string& s : result) {
                        cout << "  - " << s << endl;
                    }
                    cout << "Total: " << result.size() << " students" << endl;
                    pause();
                    break;
                }
                
                case 4: {
                    vector<string> courseIDs;
                    int n;
                    cout << "Enter number of courses: ";
                    cin >> n;
                    cin.ignore();
                    
                    for (int i = 0; i < n; i++) {
                        string courseID;
                        cout << "Enter Course ID " << (i+1) << ": ";
                        getline(cin, courseID);
                        courseIDs.push_back(courseID);
                    }
                    
                    set<string> result = setOperationsModule.studentsInAnyCourse(courseIDs);
                    cout << "\n--- Students in ANY specified course (Union) ---" << endl;
                    for (const string& s : result) {
                        cout << "  - " << s << endl;
                    }
                    cout << "Total: " << result.size() << " students" << endl;
                    pause();
                    break;
                }
                
                case 5: {
                    string courseA, courseB;
                    cout << "Enter Course A: ";
                    getline(cin, courseA);
                    cout << "Enter Course B: ";
                    getline(cin, courseB);
                    
                    set<string> result = setOperationsModule.studentDifference(courseA, courseB);
                    cout << "\n--- Students in " << courseA << " but NOT in " << courseB << " (Difference) ---" << endl;
                    for (const string& s : result) {
                        cout << "  - " << s << endl;
                    }
                    cout << "Total: " << result.size() << " students" << endl;
                    pause();
                    break;
                }
                
                case 6: {
                    int maxStudents;
                    cout << "Enter max students to include (recommended: <=5): ";
                    cin >> maxStudents;
                    cin.ignore();
                    
                    vector<set<string>> powerSet = setOperationsModule.generatePowerSet(maxStudents);
                    cout << "\n--- Power Set ---" << endl;
                    cout << "Power set size: " << powerSet.size() << " subsets" << endl;
                    cout << "\nFirst 10 subsets:" << endl;
                    int count = 0;
                    for (const set<string>& subset : powerSet) {
                        cout << "{";
                        bool first = true;
                        for (const string& s : subset) {
                            if (!first) cout << ", ";
                            cout << s;
                            first = false;
                        }
                        cout << "}" << endl;
                        if (++count >= 10) break;
                    }
                    pause();
                    break;
                }
                
                case 7:
                    setOperationsModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void relationsMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 6: RELATIONS");
            
            cout << "\n1. View Student-Course Relations" << endl;
            cout << "2. View Faculty-Course Relations" << endl;
            cout << "3. Check Reflexive Property" << endl;
            cout << "4. Check Symmetric Property" << endl;
            cout << "5. Check Transitive Property" << endl;
            cout << "6. Check Equivalence Relation" << endl;
            cout << "7. Compose Relations (Student->Course->Faculty)" << endl;
            cout << "8. Detect Indirect Course Conflicts" << endl;
            cout << "9. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    relationsModule.buildRelations();
                    cout << "\nStudent-Course relation built." << endl;
                    cout << "Use other menu options to analyze properties." << endl;
                    pause();
                    break;
                }
                
                case 8: {
                    cout << "\n--- Detecting Indirect Course Conflicts ---" << endl;
                    relationsModule.buildRelations();
                    map<string, set<string>> studentCourses;
                    for (auto& pair : dm->getAllStudents()) {
                        Student& student = pair.second;
                        for (const string& courseID : student.getEnrolledCourses()) {
                            studentCourses[student.getID()].insert(courseID);
                            Course* course = dm->getCourse(courseID);
                            if (course) {
                                for (const string& prereq : course->getPrerequisites()) {
                                    if (student.getEnrolledCourses().find(prereq) != student.getEnrolledCourses().end()) {
                                        cout << "INDIRECT CONFLICT: Student " << student.getID() 
                                             << " enrolled in both " << courseID 
                                             << " and its prerequisite " << prereq << endl;
                                    }
                                }
                            }
                        }
                    }
                    
                    cout << "\nConflict detection complete." << endl;
                    pause();
                    break;
                }
                
                case 9:
                    relationsModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Feature analysis available in demonstration mode." << endl;
                    pause();
            }
        }
    }

    void functionsMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 7: FUNCTIONS");
            
            cout << "\n1. View Student->Course Mapping" << endl;
            cout << "2. View Course->Faculty Mapping" << endl;
            cout << "3. View Faculty->Room Mapping" << endl;
            cout << "4. Check Injective Property" << endl;
            cout << "5. Check Surjective Property" << endl;
            cout << "6. Check Bijective Property" << endl;
            cout << "7. Compose Functions (Student->Course->Faculty)" << endl;
            cout << "8. Find Inverse Function" << endl;
            cout << "9. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    cout << "\nFunction analysis performed in demonstration mode." << endl;
                    cout << "Building and analyzing functions..." << endl;
                    functionsModule.buildFunctions();
                    pause();
                    break;
                    
                case 9:
                    functionsModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void proofVerificationMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 8: AUTOMATED PROOF & VERIFICATION");
            
            cout << "\n1. Generate Prerequisite Proof" << endl;
            cout << "2. Verify Course Sequence Consistency" << endl;
            cout << "3. Validate Logic Rules" << endl;
            cout << "4. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1: {
                    string studentID, courseID;
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Course ID: ";
                    getline(cin, courseID);
                    proofModule.generatePrerequisiteProof(studentID, courseID);
                    pause();
                    break;
                }
                
                case 2: {
                    vector<string> sequence;
                    int n;
                    cout << "Enter number of courses in sequence: ";
                    cin >> n;
                    cin.ignore();
                    
                    for (int i = 0; i < n; i++) {
                        string courseID;
                        cout << "Enter Course " << (i+1) << ": ";
                        getline(cin, courseID);
                        sequence.push_back(courseID);
                    }
                    
                    proofModule.verifyConsistency(sequence);
                    pause();
                    break;
                }
                
                case 3:
                    proofModule.validateLogicRules();
                    pause();
                    break;
                    
                case 4:
                    proofModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void consistencyCheckerMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 9: CONSISTENCY CHECKER");
            
            cout << "\n1. Detect Course Conflicts" << endl;
            cout << "2. Detect Missing Prerequisites" << endl;
            cout << "3. Detect Student Overload" << endl;
            cout << "4. Run All Consistency Checks" << endl;
            cout << "5. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1:
                    consistencyChecker.detectCourseConflicts();
                    pause();
                    break;
                    
                case 2:
                    consistencyChecker.detectMissingPrerequisites();
                    pause();
                    break;
                    
                case 3:
                    consistencyChecker.detectOverload();
                    pause();
                    break;
                    
                case 4:
                    consistencyChecker.runAllChecks();
                    pause();
                    break;
                    
                case 5:
                    consistencyChecker.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void efficiencyMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULE 10: ALGORITHMIC EFFICIENCY");
            
            cout << "\n1. Benchmark Prerequisite Path Computation" << endl;
            cout << "2. Benchmark Set Operations" << endl;
            cout << "3. Demonstrate Recursion Optimization" << endl;
            cout << "4. Run Full Demonstration" << endl;
            cout << "0. Back to Main Menu" << endl;
            
            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1:
                    efficiencyModule.benchmarkPrerequisitePaths();
                    pause();
                    break;
                    
                case 2:
                    efficiencyModule.benchmarkSetOperations();
                    pause();
                    break;
                    
                case 3:
                    efficiencyModule.demonstrateRecursionOptimization();
                    pause();
                    break;
                    
                case 4:
                    efficiencyModule.demonstrate();
                    pause();
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
                    pause();
            }
        }
    }

    void dataManagementMenu() {
        while (true) {
            clearScreen();
            printHeader("DATA MANAGEMENT");

            cout << "\n1. Add Student" << endl;
            cout << "2. Add Course" << endl;
            cout << "3. Add Faculty" << endl;
            cout << "4. Add Room" << endl;
            cout << "5. Add Lab" << endl;
            cout << "6. View All Students" << endl;
            cout << "7. View All Courses" << endl;
            cout << "8. View All Faculty" << endl;
            cout << "9. Load Sample Data" << endl;
            cout << "0. Back to Main Menu" << endl;

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 0) break;

            switch (choice) {
            case 1: {
                string id, name;
                cout << "Enter Student ID: ";
                getline(cin, id);
                cout << "Enter Student Name: ";
                getline(cin, name);

                Student student(id, name);
                dm->addStudent(student);
                cout << "Student added successfully!" << endl;
                pause();
                break;
            }
            case 2: {
                string id, name;
                int credits;
                cout << "Enter Course ID: ";
                getline(cin, id);
                cout << "Enter Course Name: ";
                getline(cin, name);
                cout << "Enter Credits: ";
                cin >> credits;
                cin.ignore();

                Course course(id, name, credits);

                cout << "Add prerequisites? (y/n): ";
                char choice;
                cin >> choice;
                cin.ignore();

                if (choice == 'y' || choice == 'Y') {
                    while (true) {
                        string prereq;
                        cout << "Enter prerequisite course ID (or 'done'): ";
                        getline(cin, prereq);
                        if (prereq == "done") break;
                        course.addPrerequisite(prereq);
                    }
                }

                dm->addCourse(course);
                cout << "Course added successfully!" << endl;
                pause();
                break;
            }
            case 3: {
                string id, name;
                int maxCourses;
                cout << "Enter Faculty ID: ";
                getline(cin, id);
                cout << "Enter Faculty Name: ";
                getline(cin, name);
                cout << "Enter Max Courses: ";
                cin >> maxCourses;
                cin.ignore();

                Faculty faculty(id, name, maxCourses);
                dm->addFaculty(faculty);
                cout << "Faculty added successfully!" << endl;
                pause();
                break;
            }
            case 4: {
                string id, type;
                int capacity;
                cout << "Enter Room ID: ";
                getline(cin, id);
                cout << "Enter Capacity: ";
                cin >> capacity;
                cin.ignore();
                cout << "Enter Type (Lecture/Lab/Seminar): ";
                getline(cin, type);

                Room room(id, capacity, type);
                dm->addRoom(room);
                cout << "Room added successfully!" << endl;
                pause();
                break;
            }
            case 5: {
                string id, courseID;
                int capacity;
                cout << "Enter Lab ID: ";
                getline(cin, id);
                cout << "Enter Associated Course ID: ";
                getline(cin, courseID);
                cout << "Enter Capacity: ";
                cin >> capacity;
                cin.ignore();

                Lab lab(id, courseID, capacity);
                dm->addLab(lab);
                cout << "Lab added successfully!" << endl;
                pause();
                break;
            }
            case 6: {
                cout << "\n=== All Students ===" << endl;
                for (auto& pair : dm->getAllStudents()) {
                    Student& s = pair.second;
                    cout << s.getID() << ": " << s.getName()
                        << " (Credits: " << s.getCurrentCredits() << ")" << endl;
                }
                pause();
                break;
            }
            case 7: {
                cout << "\n=== All Courses ===" << endl;
                for (auto& pair : dm->getAllCourses()) {
                    Course& c = pair.second;
                    cout << c.getID() << ": " << c.getName()
                        << " (" << c.getCredits() << " credits)" << endl;
                }
                pause();
                break;
            }
            case 8: {
                cout << "\n=== All Faculty ===" << endl;
                for (auto& pair : dm->getAllFaculty()) {
                    Faculty& f = pair.second;
                    cout << f.getID() << ": " << f.getName()
                        << " (Courses: " << f.getAssignedCourses().size() << ")" << endl;
                }
                pause();
                break;
            }
            case 9:
                loadSampleData();
                pause();
                break;
            default:
                cout << "Invalid choice!" << endl;
                pause();
            }
        }
    }

    void modulesMenu() {
        while (true) {
            clearScreen();
            printHeader("MODULES MENU");

            cout << "\n1.  Course & Scheduling Module" << endl;
            cout << "2.  Student Group Combination Module" << endl;
            cout << "3.  Induction & Strong Induction Module" << endl;
            cout << "4.  Logic & Inference Engine" << endl;
            cout << "5.  Set Operations Module" << endl;
            cout << "6.  Relations Module" << endl;
            cout << "7.  Functions Module" << endl;
            cout << "8.  Automated Proof & Verification" << endl;
            cout << "9.  Consistency Checker" << endl;
            cout << "10. Algorithmic Efficiency & Benchmarking" << endl;
            cout << "11. Run All Module Demonstrations" << endl;
            cout << "0.  Back to Main Menu" << endl;

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 0) break;

            switch (choice) {
            case 1:
                courseSchedulingMenu();
                break;
            case 2:
                combinationsMenu();
                break;
            case 3:
                inductionMenu();
                break;
            case 4:
                logicInferenceMenu();
                break;
            case 5:
                setOperationsMenu();
                break;
            case 6:
                relationsMenu();
                break;
            case 7:
                functionsMenu();
                break;
            case 8:
                proofVerificationMenu();
                break;
            case 9:
                consistencyCheckerMenu();
                break;
            case 10:
                efficiencyMenu();
                break;
            case 11: 
                clearScreen();
                cout << "Running all module demonstrations...\n" << endl; 
                schedulingModule.demonstrate();
                pause();
                combinationsModule.demonstrate();
                pause();
                inductionModule.demonstrate();
                pause();
                logicEngine.demonstrate();
                pause();
                setOperationsModule.demonstrate();
                pause();
                relationsModule.demonstrate();
                pause();
                functionsModule.demonstrate();
                pause();
                proofModule.demonstrate();
                pause();
                consistencyChecker.demonstrate();
                pause();
                efficiencyModule.demonstrate();
                pause();
                break;
            default:
                cout << "Invalid choice!" << endl;
                pause();
            }
        }
    }

public:
    UnidiscSystem() {
        dm = DataManager::getInstance();
    }

    void run() {
        while (true) {
            clearScreen();
            printHeader("UNIDISC ENGINE - FAST University Management System");

            cout << "================ MAIN MENU ================" << endl;
            cout << "1. Data Management" << endl;
            cout << "2. Access Module Menus" << endl;
            cout << "3. Quick Start (Load Sample Data & Run Demos)" << endl;
            cout << "4. Unit Testing & Benchmarking" << endl;
            cout << "0. Exit" << endl;
            cout << "===========================================" << endl;

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                dataManagementMenu();
                break;
            case 2:
                modulesMenu();
                break;
            case 3:
                clearScreen();
                printHeader("QUICK START");
                loadSampleData();
                cout << "\nRunning all module demonstrations...\n" << endl;
                pause();

                schedulingModule.demonstrate();
                pause();
                combinationsModule.demonstrate();
                pause();
                inductionModule.demonstrate();
                pause();
                logicEngine.demonstrate();
                pause();
                setOperationsModule.demonstrate();
                pause();
                relationsModule.demonstrate();
                pause();
                functionsModule.demonstrate();
                pause();
                proofModule.demonstrate();
                pause();
                consistencyChecker.demonstrate();
                pause();
                efficiencyModule.demonstrate();
                pause();
                break;
            case 4:
                clearScreen();
                printHeader("UNIT TESTING & BENCHMARKING");
                unitTestingModule.runAllTests();
                pause();
                break;
            case 0:
                cout << "\nThank you for using UNIDISC ENGINE!" << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pause();
            }
        }
    }
};

int main() {

    cout << "============================================================" << endl;
    cout << "UNIDISC ENGINE" << endl;
    cout << "FAST University Management System" << endl;
    cout << "Discrete Mathematics Project - 2024" << endl;
    cout << "============================================================" << endl;

    cout << "\nInitializing system..." << endl;

    UnidiscSystem system;
    system.run();

    return 0;
}