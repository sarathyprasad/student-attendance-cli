# Student Attendance Management System (CLI)

A lightweight, terminal-based Student Attendance Management application written entirely in **Core C**. This project features an interactive, menu-driven command-line interface that allows faculty to efficiently track, update, and audit student attendance records using persistent file handling.

---

## 🔒 Administrative Security Credentials
To access the faculty dashboard and administrative features of this terminal application, use the following default hardcoded system credentials:

* **Admin ID:** `011`
* **Admin Password:** `2604`
* **Special Master Password:** `061107`

---

## 📝 Project Description

The **Student Attendance Management System** is a lightweight, terminal-based application designed to streamline the academic tracking of student logs. Built entirely from scratch in C, this system bypasses heavy graphical frameworks to deliver an ultra-fast, resource-efficient tool tailored for educational environments.

The project was developed to simulate real-world software engineering challenges, focusing on structural data modeling, persistent text-file architectures, and robust terminal interaction logic.

### 🧠 Core Engineering Principles Demonstrated

* **Data Architecture & Management:** Utilizes custom C structures (`struct`) to securely model complex student records containing attributes like unique Roll Numbers, Names, Total Classes, and Attended Classes.
* **Persistent Flat-File Database:** Implements reliable file-handling algorithms (`fopen`, `fprintf`, `fscanf`) to save data permanently onto a structured text database. The system ensures that records persist safely even after the application process terminates.
* **Data Integrity & Validation:** Built-in validation algorithms aggressively scan user input to prevent critical runtime errors, check for duplicate Roll Numbers during registration, and enforce boundaries on numerical tracking scores.
* **Algorithmic Filtering:** Features an automated parsing module that evaluates real-time student ratios on the fly. It dynamically catches records dropping below standard academic thresholds (e.g., $75\%$) and instantly isolates them into an actionable administrative audit report.

---

## 🚀 Features
* **Secure Access Gateway:** Protects faculty administrative menus behind custom credential checking.
* **Menu-Driven CLI:** Interactive terminal console interface for quick navigation.
* **Persistent Storage:** Built using C file handling (`stdio.h`) to securely store, update, and read student data from structured flat-files.
* **Automated Computations:** Instant computation of individual attendance percentages:
  $$\text{Attendance \%} = \left( \frac{\text{Classes Attended}}{\text{Total Classes}} \right) \times 100$$
* **Defaulter Tracking:** Built-in filter that dynamically flags and lists students falling below the mandatory threshold (e.g., $75\%$).

---

## 🛠️ Tech Stack
* **Language:** C (C11 standard)
* **Paradigm:** Procedural Programming
* **Storage:** File Handling (Text-based flat-file database)

---

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
