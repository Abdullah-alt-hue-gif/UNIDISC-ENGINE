#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;


class Course;
class Student;
class Faculty;
class Room;
class Lab;


class Course {
private:
    string courseID;
    string courseName;
    int credits;
    set<string> prerequisites; 

public:
    Course() : credits(0) {}
    Course(string id, string name, int cred)
        : courseID(id), courseName(name), credits(cred) {
    }

    string getID() const { return courseID; }
    string getName() const { return courseName; }
    int getCredits() const { return credits; }
    set<string> getPrerequisites() const { return prerequisites; }

    void addPrerequisite(const string& prereq) { prerequisites.insert(prereq); }
    void setPrerequisites(const set<string>& prereqs) { prerequisites = prereqs; }

    bool operator<(const Course& other) const { return courseID < other.courseID; }
};

class Student {
private:
    string studentID;
    string name;
    set<string> enrolledCourses; 
    set<string> completedCourses; 
    int currentCredits;

public:
    Student() : currentCredits(0) {}
    Student(string id, string n)
        : studentID(id), name(n), currentCredits(0) {
    }

    string getID() const { return studentID; }
    string getName() const { return name; }
    set<string> getEnrolledCourses() const { return enrolledCourses; }
    set<string> getCompletedCourses() const { return completedCourses; }
    int getCurrentCredits() const { return currentCredits; }

    void enrollCourse(const string& courseID, int credits) {
        enrolledCourses.insert(courseID);
        currentCredits += credits;
    }

    void completeCourse(const string& courseID, int credits) {
        enrolledCourses.erase(courseID);
        completedCourses.insert(courseID);
        currentCredits -= credits;
    }

    void dropCourse(const string& courseID, int credits) {
        enrolledCourses.erase(courseID);
        currentCredits -= credits;
    }

    bool operator<(const Student& other) const { return studentID < other.studentID; }
};

class Faculty {
private:
    string facultyID;
    string name;
    set<string> assignedCourses; 
    int maxCourses;

public:
    Faculty() : maxCourses(3) {}
    Faculty(string id, string n, int max = 3)
        : facultyID(id), name(n), maxCourses(max) {
    }

    string getID() const { return facultyID; }
    string getName() const { return name; }
    set<string> getAssignedCourses() const { return assignedCourses; }
    int getMaxCourses() const { return maxCourses; }

    bool canAssignCourse() const { return assignedCourses.size() < maxCourses; }
    void assignCourse(const string& courseID) { assignedCourses.insert(courseID); }
    void removeCourse(const string& courseID) { assignedCourses.erase(courseID); }

    bool operator<(const Faculty& other) const { return facultyID < other.facultyID; }
};

class Room {
private:
    string roomID;
    int capacity;
    string type; 

public:
    Room() : capacity(0) {}
    Room(string id, int cap, string t)
        : roomID(id), capacity(cap), type(t) {
    }

    string getID() const { return roomID; }
    int getCapacity() const { return capacity; }
    string getType() const { return type; }

    bool operator<(const Room& other) const { return roomID < other.roomID; }
};

class Lab {
private:
    string labID;
    string associatedCourse; 
    int capacity;
    set<string> enrolledStudents; 

public:
    Lab() : capacity(0) {}
    Lab(string id, string course, int cap)
        : labID(id), associatedCourse(course), capacity(cap) {
    }

    string getID() const { return labID; }
    string getAssociatedCourse() const { return associatedCourse; }
    int getCapacity() const { return capacity; }
    set<string> getEnrolledStudents() const { return enrolledStudents; }

    bool canEnroll() const { return enrolledStudents.size() < capacity; }
    void enrollStudent(const string& studentID) { enrolledStudents.insert(studentID); }
    void removeStudent(const string& studentID) { enrolledStudents.erase(studentID); }

    bool operator<(const Lab& other) const { return labID < other.labID; }
};


class DataManager {
private:
    static DataManager* instance;

    map<string, Course> courses;
    map<string, Student> students;
    map<string, Faculty> faculty;
    map<string, Room> rooms;
    map<string, Lab> labs;

    DataManager() {} 

public:
    static DataManager* getInstance() {
        if (!instance) {
            instance = new DataManager();
        }
        return instance;
    }

    
    void addCourse(const Course& course) { courses[course.getID()] = course; }
    Course* getCourse(const string& id) {
        return courses.find(id) != courses.end() ? &courses[id] : nullptr;
    }
    map<string, Course>& getAllCourses() { return courses; }
    bool courseExists(const string& id) { return courses.find(id) != courses.end(); }

    
    void addStudent(const Student& student) { students[student.getID()] = student; }
    Student* getStudent(const string& id) {
        return students.find(id) != students.end() ? &students[id] : nullptr;
    }
    map<string, Student>& getAllStudents() { return students; }
    bool studentExists(const string& id) { return students.find(id) != students.end(); }

    void addFaculty(const Faculty& fac) { faculty[fac.getID()] = fac; }
    Faculty* getFaculty(const string& id) {
        return faculty.find(id) != faculty.end() ? &faculty[id] : nullptr;
    }
    map<string, Faculty>& getAllFaculty() { return faculty; }
    bool facultyExists(const string& id) { return faculty.find(id) != faculty.end(); }

    void addRoom(const Room& room) { rooms[room.getID()] = room; }
    Room* getRoom(const string& id) {
        return rooms.find(id) != rooms.end() ? &rooms[id] : nullptr;
    }
    map<string, Room>& getAllRooms() { return rooms; }
    bool roomExists(const string& id) { return rooms.find(id) != rooms.end(); }

    void addLab(const Lab& lab) { labs[lab.getID()] = lab; }
    Lab* getLab(const string& id) {
        return labs.find(id) != labs.end() ? &labs[id] : nullptr;
    }
    map<string, Lab>& getAllLabs() { return labs; }
    bool labExists(const string& id) { return labs.find(id) != labs.end(); }

    void clearAll() {
        courses.clear();
        students.clear();
        faculty.clear();
        rooms.clear();
        labs.clear();
    }
};