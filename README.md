# Student Attendance Management System (CLI)

A lightweight, terminal-based Student Attendance Management application written entirely in **Core C**. This project features an interactive, menu-driven command-line interface that allows faculty to efficiently track, update, and audit student attendance records using persistent file handling.

## 🚀 Features
* **Menu-Driven CLI:** Interactive terminal console interface for quick navigation.
* **Persistent Storage:** Built using C file handling (`stdio.h`) to securely store, update, and read student data from structured flat-files.
* **Automated Computations:** Instant computation of individual attendance percentages:
  $$\text{Attendance \%} = \left( \frac{\text{Classes Attended}}{\text{Total Classes}} \right) \times 100$$
* **Defaulter Tracking:** Built-in filter that dynamically flags and lists students falling below the mandatory threshold (e.g., $75\%$).
* **Data Verification:** Built-in safeguards to prevent duplicate roll numbers and handle invalid data entries.

## 🛠️ Tech Stack
* **Language:** C (C11 standard)
* **Paradigm:** Procedural Programming
* **Storage:** File Handling (Text-based database)

## 💻 Sample Terminal Interface View
```text
===================================================
      STUDENT ATTENDANCE MANAGEMENT SYSTEM        
===================================================
1. Record Daily Attendance
2. View Cumulative Attendance Report
3. Generate Defaulter List (< 75%)
4. Search Student Record by Roll Number
5. Exit System
===================================================
Enter your choice (1-5): _
