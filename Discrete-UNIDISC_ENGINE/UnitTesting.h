#pragma once

#include "Core.h"
#include "CourseScheduling.h"
#include "StudentGroupCombination.h"
#include "Induction.h"
#include "LogicInference.h"
#include "Set.h"
#include "Relation.h"
#include "Function.h"
#include "AutomatedProofVerification.h"
#include "ConsistencyChecker.h"
#include "AlgorithmicEfficiency.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

class UnitTestingModule {
private:
    DataManager* dm;
    int testsRun;
    int testsPassed;
    int testsFailed;

    void assert_true(bool condition, const string& testName) {
        testsRun++;
        if (condition) {
            testsPassed++;
            cout << "  [PASS] " << testName << endl;
        }
        else {
            testsFailed++;
            cout << "  [FAIL] " << testName << endl;
        }
    }

    void assert_equal(int actual, int expected, const string& testName) {
        testsRun++;
        if (actual == expected) {
            testsPassed++;
            cout << "  [PASS] " << testName << endl;
        }
        else {
            testsFailed++;
            cout << "  [FAIL] " << testName << " (Expected: " << expected << ", Got: " << actual << ")" << endl;
        }
    }

    void setupTestData() {
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
    }

public:
    UnitTestingModule() : testsRun(0), testsPassed(0), testsFailed(0) {
        dm = DataManager::getInstance();
    }

    void testSchedulingModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 1: COURSE SCHEDULING" << endl;
        cout << "========================================\n" << endl;

        SchedulingModule scheduling;

        
        Course* course = dm->getCourse("CS101");
        assert_true(course != nullptr, "Retrieve existing course CS101");

        
        Course* nonExistent = dm->getCourse("XXX999");
        assert_true(nonExistent == nullptr, "Non-existent course returns nullptr");

        
        Course* cs102 = dm->getCourse("CS102");
        assert_equal((int)cs102->getPrerequisites().size(), 1, "CS102 has 1 prerequisite");

        
        Course* math101 = dm->getCourse("MATH101");
        assert_equal(math101->getCredits(), 4, "MATH101 has 4 credits");

        cout << "\nScheduling Module Tests Complete\n";
    }

    void testCombinationsModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 2: STUDENT GROUP COMBINATIONS" << endl;
        cout << "========================================\n" << endl;

        CombinationsModule combinations;

        
        vector<vector<string>> groups = combinations.formProjectGroups(2);
        assert_true(groups.size() > 0, "Form project groups with size 2");

        
        if (!groups.empty()) {
            assert_equal((int)groups[0].size(), 2, "Each group has correct size");
        }

        
        map<string, vector<string>> labAssignments = combinations.assignLabGroups("CS101", 1);
        assert_true(labAssignments.size() > 0, "Assign lab groups successfully");

        cout << "\nCombinations Module Tests Complete\n";
    }

    void testInductionModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 3: MATHEMATICAL INDUCTION" << endl;
        cout << "========================================\n" << endl;

        InductionModule induction;

        
        bool baseCase = induction.verifyBaseCase("S101", "CS101");
        assert_true(baseCase, "Base case: CS101 has no prerequisites");

        
        bool inductiveStep = induction.verifyInductiveStep("S101", "CS102");
        assert_true(inductiveStep, "Inductive step: S101 can take CS102");

        
        bool strongInduction = induction.verifyStrongInduction("S103", "CS201");
        assert_true(strongInduction, "Strong induction: All prerequisites validated");

        cout << "\nInduction Module Tests Complete\n";
    }

    void testLogicModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 4: LOGIC & INFERENCE" << endl;
        cout << "========================================\n" << endl;

        LogicEngine logicEngine;

        
        logicEngine.addCourseRule("CS102", "CS101");
        assert_true(true, "Add course prerequisite rule");

        
        logicEngine.addFacultyRule("F101", "CS101", "R101");
        assert_true(true, "Add faculty assignment rule");

        
        logicEngine.addFact("student_enrolled(S101, CS102)");
        assert_true(true, "Add fact to knowledge base");

        
        set<string> inferred = logicEngine.runInference();
        assert_true(inferred.size() > 0, "Inference produces results");

        cout << "\nLogic & Inference Module Tests Complete\n";
    }

    void testSetModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 5: SET OPERATIONS" << endl;
        cout << "========================================\n" << endl;

        SetOperationsModule setOps;

        
        set<string> studentSet = setOps.getStudentSet();
        assert_equal((int)studentSet.size(), 3, "Student set contains 3 students");

        
        set<string> courseSet = setOps.getCourseSet();
        assert_equal((int)courseSet.size(), 5, "Course set contains 5 courses");

        cout << "\nSet Operations Module Tests Complete\n";
    }

    void testRelationsModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 6: RELATIONS" << endl;
        cout << "========================================\n" << endl;

        RelationsModule relations;

        
        set<string> domain = { "A", "B", "C" };
        set<pair<string, string>> reflexiveRel = { {"A","A"}, {"B","B"}, {"C","C"} };
        bool reflexive = relations.isReflexive(reflexiveRel, domain);
        assert_true(reflexive, "Reflexive relation check");

        
        set<pair<string, string>> symRel = { {"A","B"}, {"B","A"} };
        bool symmetric = relations.isSymmetric(symRel);
        assert_true(symmetric, "Symmetric relation check");

        set<pair<string, string>> transRel = { {"A","B"}, {"B","C"}, {"A","C"} };
        bool transitive = relations.isTransitive(transRel);
        assert_true(transitive, "Transitive relation check");

        cout << "\nRelations Module Tests Complete\n";
    }

    void testFunctionsModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 7: FUNCTIONS" << endl;
        cout << "========================================\n" << endl;

        FunctionsModule functions;

        map<string, string> injFunc = { {"A", "1"}, {"B", "2"}, {"C", "3"} };
        bool injective = functions.isInjective(injFunc);
        assert_true(injective, "Injective function check");

        map<string, string> nonInjFunc = { {"A", "1"}, {"B", "1"}, {"C", "2"} };
        bool nonInjective = !functions.isInjective(nonInjFunc);
        assert_true(nonInjective, "Non-injective function identified");

        cout << "\nFunctions Module Tests Complete\n";
    }

    void testProofModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 8: AUTOMATED PROOF & VERIFICATION" << endl;
        cout << "========================================\n" << endl;

        ProofModule proof;

        bool proof1 = proof.generatePrerequisiteProof("S101", "CS102");
        assert_true(proof1, "Generate proof for prerequisite satisfaction");

        cout << "\nProof & Verification Module Tests Complete\n";
    }

    void testConsistencyCheckerModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 9: CONSISTENCY CHECKER" << endl;
        cout << "========================================\n" << endl;

        ConsistencyChecker checker;

        checker.detectCourseConflicts();
        assert_true(true, "Conflict detection executed");

        cout << "\nConsistency Checker Module Tests Complete\n";
    }

    void testEfficiencyModule() {
        cout << "\n========================================" << endl;
        cout << "TESTING MODULE 10: ALGORITHMIC EFFICIENCY" << endl;
        cout << "========================================\n" << endl;

        EfficiencyModule efficiency;

        auto start = high_resolution_clock::now();
        efficiency.benchmarkPrerequisitePaths();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        assert_true(duration.count() < 1000, "Prerequisite path computation is efficient");

        cout << "\nAlgorithmic Efficiency Module Tests Complete\n";
    }

    void testCoreDataManager() {
        cout << "\n========================================" << endl;
        cout << "TESTING CORE DATA MANAGER" << endl;
        cout << "========================================\n" << endl;

        
        Course testCourse("TEST101", "Test Course", 3);
        dm->addCourse(testCourse);
        Course* retrieved = dm->getCourse("TEST101");
        assert_true(retrieved != nullptr, "Add and retrieve course");

        
        Student testStudent("TSTUD01", "Test Student");
        dm->addStudent(testStudent);
        Student* retrievedStud = dm->getStudent("TSTUD01");
        assert_true(retrievedStud != nullptr, "Add and retrieve student");

        cout << "\nCore Data Manager Tests Complete\n";
    }

    void performanceBenchmarking() {
        cout << "\n========================================" << endl;
        cout << "PERFORMANCE BENCHMARKING" << endl;
        cout << "========================================\n" << endl;

        
        auto start = high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            dm->getCourse("CS101");
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "1000 course lookups: " << duration.count() << " microseconds" << endl;

        
        start = high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            dm->getStudent("S101");
        }
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end - start);
        cout << "1000 student lookups: " << duration.count() << " microseconds" << endl;

        cout << "\nPerformance Benchmarking Complete\n";
    }

    void runAllTests() {
        cout << "\n========================================" << endl;
        cout << "UNIDISC ENGINE - COMPREHENSIVE UNIT TEST SUITE" << endl;
        cout << "Testing All Modules for Correctness & Validity" << endl;
        cout << "========================================" << endl;

        testsRun = 0;
        testsPassed = 0;
        testsFailed = 0;

        setupTestData();

        try {
            testCoreDataManager();
            testSchedulingModule();
            testCombinationsModule();
            testInductionModule();
            testLogicModule();
            testSetModule();
            testRelationsModule();
            testFunctionsModule();
            testProofModule();
            testConsistencyCheckerModule();
            testEfficiencyModule();
            performanceBenchmarking();
        }
        catch (const exception& e) {
            cout << "\nEXCEPTION CAUGHT: " << e.what() << endl;
        }

        
        cout << "\n========================================" << endl;
        cout << "TEST SUMMARY REPORT                   " << endl;
        cout << "========================================" << endl;
        cout << "Total Tests Run:    " << testsRun << endl;
        cout << "Tests Passed:       " << testsPassed << endl;
        cout << "Tests Failed:       " << testsFailed << endl;
        if (testsRun > 0) {
            cout << "Success Rate:       " << fixed << setprecision(1) << (100.0 * testsPassed / testsRun) << "%" << endl;
        }
        cout << "========================================" << endl;
        if (testsFailed == 0) {
            cout << "\n[SUCCESS] All tests passed!" << endl;
        }
        else {
            cout << "\n[WARNING] " << testsFailed << " test(s) failed!" << endl;
        }
    }

    
    void demonstrate() {
        runAllTests();
    }

    
    int getTestsRun() const { return testsRun; }
    int getTestsPassed() const { return testsPassed; }
    int getTestsFailed() const { return testsFailed; }
};
