# UNIDISC-ENGINE
🎓 A comprehensive C++ university management system implementing multiple discrete mathematics modules: logic, sets, relations, functions, induction, scheduling, and automated reasoning etc. with prerequisite enforcement.
## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Discrete Mathematics Concepts](#discrete-mathematics-concepts)
- [System Architecture](#system-architecture)
- [Installation](#installation)
- [Usage Guide](#usage-guide)
- [Module Documentation](#module-documentation)
- [Project Structure](#project-structure)
- [Contributors](#contributors)

## 🌟 Overview

**UNIDISC ENGINE** is a high-level computational framework designed for FAST University that models academic operations using discrete mathematical structures. The system manages courses, prerequisites, student enrollment, faculty assignments, and resource allocation while enforcing complex dependencies and validation rules.

### Key Highlights

✅ **10+ Fully Integrated Modules** - Each implementing core discrete mathematics concepts  
✅ **Prerequisite Enforcement** - Students cannot enroll without completing prerequisites  
✅ **Automated Conflict Detection** - Finds direct and indirect scheduling conflicts  
✅ **Logic-Based Inference** - Rule parsing and forward chaining inference engine  
✅ **Mathematical Proof Generation** - Automated formal proofs for course eligibility  
✅ **Comprehensive Testing Suite** - Unit tests and performance benchmarking  
✅ **Interactive CLI** - User-friendly menu system for all operations  

## 🎯 Features

### Core Functionalities

- **Course Management**: Add courses, define prerequisites, generate valid sequences
- **Student Enrollment**: Track enrollments with prerequisite validation
- **Faculty Assignment**: Assign courses to faculty with constraint checking
- **Resource Allocation**: Manage rooms and lab assignments
- **Group Formation**: Create project groups using combinatorial algorithms
- **Conflict Detection**: Identify scheduling conflicts through relation composition
- **Proof Verification**: Generate step-by-step mathematical proofs
- **Performance Optimization**: Benchmark operations with memoization

### Advanced Features

- **Transitive Prerequisite Closure**: Detect indirect prerequisite violations
- **Set-Based Analysis**: Union, intersection, difference, and power set operations
- **Function Property Checking**: Verify injective, surjective, and bijective mappings
- **Logic Rule Inference**: Forward chaining to derive new facts from rules
- **Topological Sorting**: Optimal course ordering respecting prerequisites
- **Strong Induction Verification**: Validate multi-level prerequisite chains

## 📐 Discrete Mathematics Concepts

This project demonstrates practical applications of:

|         Concept            |              Implementation                 |  Module   |
|----------------------------|---------------------------------------------|-----------|
| **Sets & Set Theory**      | Student/course representations, power sets  | Module 5  |
| **Relations**              | Student-course, faculty-course mappings     | Module 6  |
| **Functions**              | Injective/surjective/bijective assignments  | Module 7  |
| **Logic & Inference**      | Propositional logic, predicate logic, rules | Module 4  |
| **Mathematical Induction** | Prerequisite chain verification             | Module 3  | 
| **Combinatorics**          | C(n,r) calculations, group formations       | Module 2  |
| **Graph Theory**           | Topological sort, transitive closure        | Module 1  |
| **Proof Techniques**       | Direct proof, proof by induction            | Module 8  |
| **Partial Orders**         | Prerequisite hierarchies                    | Module 6  |
| **Algorithms**             | Recursion, memoization, DP                  | Module 10 |

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    UNIDISC ENGINE                       │
│                  Main Application Layer                 │
└─────────────────────────────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        ▼                   ▼                   ▼
┌──────────────┐   ┌──────────────┐   ┌──────────────┐
│   Course &   │   │   Student    │   │  Induction & │
│  Scheduling  │   │   Groups &   │   │    Strong    │
│    Module    │   │ Combinations │   │  Induction   │
└──────────────┘   └──────────────┘   └──────────────┘
        │                   │                   │
        ▼                   ▼                   ▼
┌──────────────┐   ┌──────────────┐   ┌──────────────┐
│   Logic &    │   │     Set      │   │   Relations  │
│  Inference   │   │  Operations  │   │    Module    │
└──────────────┘   └──────────────┘   └──────────────┘
        │                   │                   │
        ▼                   ▼                   ▼
┌──────────────┐   ┌──────────────┐   ┌──────────────┐
│  Functions   │   │  Automated   │   │ Consistency  │
│    Module    │   │    Proof     │   │   Checker    │
└──────────────┘   └──────────────┘   └──────────────┘
        │                   │                   │
        └───────────────────┼───────────────────┘
                            ▼
                   ┌──────────────┐
                   │  Algorithmic │
                   │  Efficiency  │
                   └──────────────┘
                            │
                            ▼
                   ┌──────────────┐
                   │ Data Manager │
                   │  (Singleton) │
                   └──────────────┘
```

## 🚀 Installation

### Prerequisites

- **C++ Compiler**: GCC 4.8+ or Clang 3.4+ (C++11 support required)
- **Operating System**: Linux, macOS, or Windows (with MinGW/Cygwin)
- **Memory**: Minimum 512MB RAM
- **Disk Space**: 50MB free space

### Compilation

```bash
# Clone the repository
git clone https://github.com/yourusername/unidisc-engine.git
cd unidisc-engine

# Compile the project
g++ -std=c++11 -O2 main.cpp -o unidisc

# Run the application
./unidisc
```

### Alternative Build Methods

**Using Make:**
```bash
make
./unidisc
```

**Using CMake:**
```bash
mkdir build && cd build
cmake ..
make
./unidisc
```

**Windows (MinGW):**
```bash
g++ -std=c++11 main.cpp -o unidisc.exe
unidisc.exe
```

## 📖 Usage Guide

### Quick Start (Recommended for First-Time Users)

1. Launch the application: `./unidisc`
2. Select **Option 3: Quick Start**
3. This will:
   - Load sample data (students, courses, faculty, rooms)
   - Run demonstrations of all 10 modules
   - Show example outputs for each feature

### Manual Exploration

#### Step 1: Load Data
```
Main Menu → 1. Data Management → 9. Load Sample Data
```

#### Step 2: Explore Modules
```
Main Menu → 2. Access Module Menus → [Choose any module]
```

## 📁 Project Structure

```
unidisc-engine/
│
├── Core.h                          # Base classes and data structures
│   ├── Course                      # Course entity with prerequisites
│   ├── Student                     # Student entity with enrollments
│   ├── Faculty                     # Faculty entity with assignments
│   ├── Room                        # Room entity
│   ├── Lab                         # Lab entity
│   └── DataManager (Singleton)     # Central data management
│
├── CourseScheduling.h              # Module 1: Scheduling & Prerequisites
├── StudentGroupCombination.h       # Module 2: Combinatorics
├── Induction.h                     # Module 3: Mathematical Induction
├── LogicInference.h                # Module 4: Logic & Inference Engine
├── Set.h                           # Module 5: Set Operations
├── Relation.h                      # Module 6: Relations & Conflict Detection
├── Function.h                      # Module 7: Function Analysis
├── AutomatedProofVerification.h    # Module 8: Proof Generation
├── ConsistencyChecker.h            # Module 9: Consistency Validation
├── AlgorithmicEfficiency.h         # Module 10: Performance Benchmarking
├── UnitTesting.h                   # Comprehensive Test Suite
│
├── main.cpp                        # Main application with menu system
├── README.md                       # This file
├── LICENSE                         # MIT License
└── Makefile                        # Build configuration
```

## 🤝 Contributors

This project was developed as part of the Discrete Mathematics course at FAST University.

**Team Members:**
- [Muhammad Abdullah] - Project Lead
- [Junaid Haider]     - Member
- [Maryam Usman]      - Member

**Instructor:** [Amer Rasheed]  
**Course:** Discrete Mathematics  
**Institution:** FAST University  
**Year:** 2025

## 📞 Support & Contact

For questions, issues, or contributions:

- **Email**: ryomansukuna3097@gmail.com

## 📊 Project Statistics

- **Total Lines of Code**: ~4,500
- **Number of Functions**: 150+
- **Test Coverage**: ~100%
- **Modules**: 10
- **Documentation**: Comprehensive README File

---

<div align="center">

**Built with ❤️ using Discrete Mathematics**

[⬆ Back to Top](#-unidisc-engine---university-management-system)

</div>
