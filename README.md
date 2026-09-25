# CWA Calculator

A C++ console application that calculates a student's Cumulative Weighted Average (CWA) across multiple semesters and years of a degree program. Built as Task 1 of the CodeAlpha C++ Programming Internship.

## Features

- Enter courses (name, score, credit hours) for any semester of any academic year (up to 8 years, supporting standard 4-year degrees and longer professional programs)
- Automatically calculates:
  - CWA per semester
  - CWA per academic year (weighted by credit hours)
  - Cumulative CWA across all years entered
- Prints a clean, aligned transcript table showing every year, semester, and course on record
- Saves and loads records automatically to/from `student_record.txt`, so data persists between runs
- Input validation on every prompt — invalid scores, credit hours, or menu choices are rejected with a clear message instead of crashing or corrupting saved data
- Safe file loading — a missing, empty, or corrupted data file is detected and the program starts fresh with a warning, rather than loading bad data

## Project Structure

| File | Purpose |
|---|---|
| `main.cpp` | Program entry point, menu loop, input handling, file save/load, transcript printing |
| `Semester.h` / `Semester.cpp` | Represents one semester: its courses and CWA calculation |
| `AcademicYear.h` / `AcademicYear.cpp` | Represents one academic year (two semesters) and its combined CWA |
| `Degree.h` / `Degree.cpp` | Represents the full degree (up to 8 academic years) and the cumulative CWA |
| `CMakeLists.txt` | CMake build configuration |

## Building and Running

### Option 1: CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
./CODE_ALPHA_TASK_1
```

### Option 2: g++ directly

```bash
g++ -std=c++14 -Wall -Wextra -o cwa main.cpp Semester.cpp AcademicYear.cpp Degree.cpp
./cwa
```

## Usage

On launch, the program loads any existing record from `student_record.txt` (or starts fresh if none exists). From the menu:

1. **Enter courses / calculate CWA** — choose a year and semester, then enter each course's name, score (0–100), and credit hours (1–10). The program calculates and displays the semester, year, and cumulative CWA, then saves automatically.
2. **Print Transcript** — displays a formatted table of every year and semester with recorded courses, along with each CWA.
3. **Exit** — closes the program.

## Notes

- CWA is always calculated live from stored course data, so it can never go out of sync with what was entered.
- Year CWA and cumulative CWA are weighted by credit hours, not a simple average of semester CWAs.
- `student_record.txt` is generated at runtime in the directory the program is run from.

## Author

Built as part of the [CodeAlpha](https://www.codealpha.tech) C++ Programming Internship.
