#pragma once
#include "Core.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <chrono>
using namespace std::chrono;

class EfficiencyModule {
private:
    DataManager* dm;
    map<string, vector<string>> memoizedPaths;

    vector<string> getPrerequisitePath(const string& courseID) {
        if (memoizedPaths.find(courseID) != memoizedPaths.end()) {
            return memoizedPaths[courseID];
        }

        vector<string> path;
        Course* course = dm->getCourse(courseID);

        if (!course) return path;

        path.push_back(courseID);

        for (const string& prereq : course->getPrerequisites()) {
            vector<string> prereqPath = getPrerequisitePath(prereq);
            path.insert(path.end(), prereqPath.begin(), prereqPath.end());
        }

        memoizedPaths[courseID] = path;
        return path;
    }

public:
    EfficiencyModule() {
        dm = DataManager::getInstance();
    }

    void benchmarkPrerequisitePaths() {
        cout << "\n--- Benchmarking Prerequisite Path Computation ---" << endl;

        if (dm->getAllCourses().empty()) {
            cout << "No courses to benchmark" << endl;
            return;
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < 100; i++) {
            for (auto& pair : dm->getAllCourses()) {
                vector<string> path;
                Course* course = dm->getCourse(pair.first);
                if (course) {
                    path.push_back(pair.first);
                }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "Without memoization: " << duration.count() << " us" << endl;

        memoizedPaths.clear();
        start = high_resolution_clock::now();

        for (int i = 0; i < 100; i++) {
            for (auto& pair : dm->getAllCourses()) {
                getPrerequisitePath(pair.first);
            }
        }

        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end - start);

        cout << "With memoization: " << duration.count() << " us" << endl;
        cout << "Memoization cache size: " << memoizedPaths.size() << " entries" << endl;
    }

    void benchmarkSetOperations() {
        cout << "\n--- Benchmarking Set Operations ---" << endl;

        int n = 1000;

        auto start = high_resolution_clock::now();

        set<int> setA, setB;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) setA.insert(i);
            if (i % 3 == 0) setB.insert(i);
        }

        set<int> intersection;
        set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(),
            inserter(intersection, intersection.begin()));

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "Standard set operations: " << duration.count() << " us" << endl;
        cout << "Intersection size: " << intersection.size() << endl;
    }

    void demonstrateRecursionOptimization() {
        cout << "\n--- Recursion Optimization Demo ---" << endl;

        function<long long(int)> fib = [&fib](int n) -> long long {
            if (n <= 1) return n;
            return fib(n - 1) + fib(n - 2);
            };

        map<int, long long> fibMemo;
        function<long long(int)> fibMemoized = [&](int n) -> long long {
            if (n <= 1) return n;
            if (fibMemo.find(n) != fibMemo.end()) return fibMemo[n];
            fibMemo[n] = fibMemoized(n - 1) + fibMemoized(n - 2);
            return fibMemo[n];
            };

        int n = 30;

        auto start = high_resolution_clock::now();
        long long result1 = fib(n);
        auto end = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end - start);

        start = high_resolution_clock::now();
        long long result2 = fibMemoized(n);
        end = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end - start);

        cout << "Fibonacci(" << n << ") = " << result1 << endl;
        cout << "Without memoization: " << duration1.count() << " us" << endl;
        cout << "With memoization: " << duration2.count() << " us" << endl;
        cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x" << endl;
    }

    void demonstrate() {
        cout << "\n========================================" << endl;
        cout << "MODULE 10: ALGORITHMIC EFFICIENCY & BENCHMARKING DEMONSTRATION" << endl;
        cout << "========================================\n" << endl;

        benchmarkPrerequisitePaths();
        benchmarkSetOperations();
        demonstrateRecursionOptimization();
    }
};