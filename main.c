//Admin Id: 011
//Admin Password: 2604
//Special Password: 061107


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constants and File Names ---
#define MAX_NAME 50
#define MAX_PASS 20
#define MAX_CLASS 30
#define MAX_SUBJECT 50

#define ADMIN_ROLL 11
#define ADMIN_PASSWORD "2604"
#define REPORT_PASSWORD "061107"

#define DATA_FILE_STUDENT "student_data.dat"
#define DATA_FILE_FACULTY "faculty_data.dat"
#define DATA_FILE_CLASS "class_data.dat"
#define DATA_FILE_ATTENDANCE "attendance_data.dat"

// --- Data Structures ---

// Structure for a Student
typedef struct {
    int rollNo;
    char name[MAX_NAME];
    char password[MAX_PASS];
    char className[MAX_CLASS]; // e.g., "CS-A"
    int year; // 4-digit Academic Year (e.g., 2024)
} Student;

// Structure for a Faculty member
typedef struct {
    int rollNo;
    char name[MAX_NAME];
    char password[MAX_PASS];
} Faculty;

// Structure for a Class/Subject mapping
typedef struct {
    char className[MAX_CLASS]; // e.g., "B.Tech-CS-A"
    char branch[MAX_NAME];
    int year; // 4-digit Academic Year (e.g., 2024)
    char subject[MAX_SUBJECT];
    int facultyRollNo; // Faculty responsible for this subject in this class/year
} Class;

// Structure for an Attendance record
typedef struct {
    int studentRollNo;
    char className[MAX_CLASS]; // Link to Class
    char subject[MAX_SUBJECT]; // Link to Class
    char date[11]; // YYYY-MM-DD
    int isPresent; // 1 for Present, 0 for Absent
} AttendanceRecord;

// --- Global Utility Functions ---

/**
 * @brief Reads all existing Students from the file.
 * @param count Pointer to store the number of records read.
 * @return A dynamically allocated array of Student structs, or NULL on failure.
 */
Student *read_all_students(int *count) {
    FILE *fp = fopen(DATA_FILE_STUDENT, "rb");
    if (!fp) {
        *count = 0;
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    *count = size / sizeof(Student);
    if (*count == 0) {
        fclose(fp);
        return NULL;
    }

    Student *students = (Student *)malloc(size);
    if (!students) {
        perror("Memory allocation failed for students");
        fclose(fp);
        *count = 0;
        return NULL;
    }

    fread(students, sizeof(Student), *count, fp);
    fclose(fp);
    return students;
}

/**
 * @brief Reads all existing Faculty from the file.
 * @param count Pointer to store the number of records read.
 * @return A dynamically allocated array of Faculty structs, or NULL on failure.
 */
Faculty *read_all_faculty(int *count) {
    FILE *fp = fopen(DATA_FILE_FACULTY, "rb");
    if (!fp) {
        *count = 0;
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    *count = size / sizeof(Faculty);
    if (*count == 0) {
        fclose(fp);
        return NULL;
    }

    Faculty *faculty = (Faculty *)malloc(size);
    if (!faculty) {
        perror("Memory allocation failed for faculty");
        fclose(fp);
        *count = 0;
        return NULL;
    }

    fread(faculty, sizeof(Faculty), *count, fp);
    fclose(fp);
    return faculty;
}

/**
 * @brief Reads all existing Classes from the file.
 * @param count Pointer to store the number of records read.
 * @return A dynamically allocated array of Class structs, or NULL on failure.
 */
Class *read_all_classes(int *count) {
    FILE *fp = fopen(DATA_FILE_CLASS, "rb");
    if (!fp) {
        *count = 0;
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    *count = size / sizeof(Class);
    if (*count == 0) {
        fclose(fp);
        return NULL;
    }

    Class *classes = (Class *)malloc(size);
    if (!classes) {
        perror("Memory allocation failed for classes");
        fclose(fp);
        *count = 0;
        return NULL;
    }

    fread(classes, sizeof(Class), *count, fp);
    fclose(fp);
    return classes;
}

/**
 * @brief Utility function to check if a class exists.
 * @return 1 if found, 0 otherwise.
 */
int class_exists(const char *className, int year, const char *subject) {
    int count;
    Class *classes = read_all_classes(&count);
    if (!classes) return 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(classes[i].className, className) == 0 && 
            classes[i].year == year && 
            strcmp(classes[i].subject, subject) == 0) {
            free(classes);
            return 1;
        }
    }

    free(classes);
    return 0;
}

/**
 * @brief Gets the current date as YYYY-MM-DD.
 */
void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date_str, 11, "%Y-%m-%d", tm);
}

// --- Admin Module Services ---

void admin_add_faculty() {
    Faculty f;
    printf("\n--- Add Faculty Module ---\n");
    printf("Enter new Faculty Roll Number (e.g., 201): ");
    if (scanf("%d", &f.rollNo) != 1) {
        printf("Invalid Roll Number input.\n"); return;
    }
    printf("Enter Faculty Name: ");
    scanf("%s", f.name);
    printf("Enter temporary Password: ");
    scanf("%s", f.password);

    FILE *fp = fopen(DATA_FILE_FACULTY, "ab");
    if (fp) {
        fwrite(&f, sizeof(Faculty), 1, fp);
        fclose(fp);
        printf("\nFaculty Added Successfully:\n");
        printf("  Roll No: %d, Name: %s\n", f.rollNo, f.name);
    } else {
        perror("Error opening faculty data file");
    }
}

void admin_add_class() {
    Class c;
    char temp_className[MAX_CLASS];
    char temp_branch[MAX_NAME];
    int temp_year;
    int num_subjects;
    
    printf("\n--- Add Class/Subject Mapping Module ---\n");
    
    // 1. Get Class details once
    printf("Enter Class Name (e.g., BTech-CS-A): ");
    scanf("%s", temp_className);
    printf("Enter Branch (e.g., ComputerScience): ");
    scanf("%s", temp_branch);
    printf("Enter Academic Year (e.g., 2024 - must be 4 digits): ");
    if (scanf("%d", &temp_year) != 1 || temp_year < 1900 || temp_year > 2100) {
        printf("Invalid Academic Year input. Must be a 4-digit year between 1900 and 2100.\n"); 
        // Clear input buffer
        while (getchar() != '\n'); 
        return;
    }

    // 2. Get number of subjects/faculty to assign
    printf("Enter the Number of Subjects/Faculty to assign to this class: ");
    if (scanf("%d", &num_subjects) != 1 || num_subjects <= 0) {
        printf("Invalid number of subjects.\n"); 
        // Clear input buffer
        while (getchar() != '\n'); 
        return;
    }

    // Check if faculty exists (to avoid multiple reads inside the loop, though this is optional)
    int f_count;
    Faculty *faculty_list = read_all_faculty(&f_count);
    
    FILE *fp = fopen(DATA_FILE_CLASS, "ab");
    if (!fp) {
        perror("Error opening class data file");
        if (faculty_list) free(faculty_list);
        return;
    }
    
    int successful_adds = 0;

    // 3. Loop for each subject/faculty assignment
    for (int i = 0; i < num_subjects; i++) {
        Class c;
        int facultyRollNo;
        char subject[MAX_SUBJECT];

        printf("\nDetails for Subject/Faculty Assignment %d:\n", i + 1);
        printf("Enter Subject Name (e.g., DataStructures): ");
        scanf("%s", subject);
        printf("Enter Faculty Roll Number assigned to '%s': ", subject);
        if (scanf("%d", &facultyRollNo) != 1) {
            printf("Invalid Faculty Roll Number input, skipping this assignment.\n");
            // Clear input buffer
            while (getchar() != '\n'); 
            continue;
        }

        // Validate Faculty Roll No
        int found = 0;
        for (int j = 0; j < f_count; j++) {
            if (faculty_list[j].rollNo == facultyRollNo) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Warning: Faculty Roll No. %d not found in records. Assignment skipped.\n", facultyRollNo);
            continue; 
        }

        // Populate the Class structure
        strcpy(c.className, temp_className);
        strcpy(c.branch, temp_branch);
        c.year = temp_year;
        strcpy(c.subject, subject);
        c.facultyRollNo = facultyRollNo;

        // 4. Save the mapping
        fwrite(&c, sizeof(Class), 1, fp);
        printf("Class/Subject Mapping Added: Class %s, Subject %s, Faculty Roll %d, Academic Year %d.\n", c.className, c.subject, c.facultyRollNo, c.year);
        successful_adds++;
    }

    fclose(fp);
    if (faculty_list) free(faculty_list);
    printf("\nFinished adding %d subject-faculty mappings for Class %s, Academic Year %d.\n", successful_adds, temp_className, temp_year);
}

void admin_add_students() {
    char class_name[MAX_CLASS];
    int year;
    int num_students;
    printf("\n--- Add Student Details Module ---\n");
    printf("Choose Class Name (e.g., CS-A): ");
    scanf("%s", class_name);
    printf("Choose Academic Year (e.g., 2024 - must be 4 digits): ");
    if (scanf("%d", &year) != 1 || year < 1900 || year > 2100) {
        printf("Invalid Academic Year input. Must be a 4-digit year between 1900 and 2100.\n"); 
        // Clear input buffer
        while (getchar() != '\n'); 
        return;
    }

    printf("Enter the Number of Students to add: ");
    if (scanf("%d", &num_students) != 1 || num_students <= 0) {
        printf("Invalid number of students.\n"); return;
    }

    FILE *fp = fopen(DATA_FILE_STUDENT, "ab");
    if (!fp) {
        perror("Error opening student data file");
        return;
    }

    for (int i = 0; i < num_students; i++) {
        Student s;
        printf("\nDetails for Student %d:\n", i + 1);
        printf("Enter Roll Number: ");
        if (scanf("%d", &s.rollNo) != 1) {
            printf("Invalid Roll Number, skipping.\n");
            // Clear input buffer
            while (getchar() != '\n'); 
            continue;
        }
        printf("Enter Name: ");
        scanf("%s", s.name);
        printf("Enter unique Password: ");
        scanf("%s", s.password);

        strcpy(s.className, class_name);
        s.year = year;

        fwrite(&s, sizeof(Student), 1, fp);
        printf("Student %d (%s) added for Academic Year %d.\n", s.rollNo, s.name, s.year);
    }
    fclose(fp);
    printf("\nFinished adding student details.\n");
}

void admin_change_password() {
    int type, rollNo;
    char current_pass[MAX_PASS], new_pass[MAX_PASS];
    printf("\n--- Change Password Module ---\n");
    printf("Change Password for:\n1. Self (Admin)\n2. Faculty\n3. Student\nEnter choice (1-3): ");
    if (scanf("%d", &type) != 1) {
        printf("Invalid choice.\n"); return;
    }

    if (type == 1) {
        printf("This feature is for self-change. Admin password is hardcoded.\n");
        return;
    } else if (type == 2) {
        printf("Enter Faculty Roll Number: ");
        if (scanf("%d", &rollNo) != 1) { printf("Invalid Roll Number.\n"); return; }
        printf("Enter Faculty's current password: ");
        scanf("%s", current_pass);

        int count, found = 0;
        Faculty *faculty_list = read_all_faculty(&count);
        FILE *fp = fopen(DATA_FILE_FACULTY, "r+b");

        for (int i = 0; i < count; i++) {
            if (faculty_list[i].rollNo == rollNo && strcmp(faculty_list[i].password, current_pass) == 0) {
                printf("Enter new Password: ");
                scanf("%s", new_pass);
                strcpy(faculty_list[i].password, new_pass);
                
                fseek(fp, i * sizeof(Faculty), SEEK_SET);
                fwrite(&faculty_list[i], sizeof(Faculty), 1, fp);
                printf("Faculty password changed successfully.\n");
                found = 1;
                break;
            }
        }
        if (faculty_list) free(faculty_list);
        if (fp) fclose(fp);
        if (!found) printf("Faculty Roll No or current password incorrect.\n");

    } else if (type == 3) {
        printf("Enter Student Roll Number: ");
        if (scanf("%d", &rollNo) != 1) { printf("Invalid Roll Number.\n"); return; }
        printf("Enter Student's current password: ");
        scanf("%s", current_pass);

        int count, found = 0;
        Student *student_list = read_all_students(&count);
        FILE *fp = fopen(DATA_FILE_STUDENT, "r+b");

        for (int i = 0; i < count; i++) {
            if (student_list[i].rollNo == rollNo && strcmp(student_list[i].password, current_pass) == 0) {
                printf("Enter new Password: ");
                scanf("%s", new_pass);
                strcpy(student_list[i].password, new_pass);
                
                fseek(fp, i * sizeof(Student), SEEK_SET);
                fwrite(&student_list[i], sizeof(Student), 1, fp);
                printf("Student password changed successfully.\n");
                found = 1;
                break;
            }
        }
        if (student_list) free(student_list);
        if (fp) fclose(fp);
        if (!found) printf("Student Roll No or current password incorrect.\n");

    } else {
        printf("Invalid option.\n");
    }
}

void admin_delete_module() {
    int type, rollNo, year;
    char pass[MAX_PASS], class_name[MAX_CLASS], admin_pass_check[MAX_PASS];
    printf("\n--- Delete Module ---\n");
    printf("What do you want to delete?\n1. Faculty Data\n2. Whole Class Data\n3. Particular Student Data\nEnter choice (1-3): ");
    if (scanf("%d", &type) != 1) {
        printf("Invalid choice.\n"); return;
    }

    // Admin Password Check for Deletion
    printf("Enter Admin Password to authorize deletion: ");
    scanf("%s", admin_pass_check);
    if (strcmp(admin_pass_check, ADMIN_PASSWORD) != 0) {
        printf("Admin Password incorrect. Deletion aborted.\n");
        return;
    }

    if (type == 1) { // Delete Faculty
        printf("Enter Faculty Roll No: ");
        if (scanf("%d", &rollNo) != 1) { printf("Invalid Roll No.\n"); return; }
        printf("Enter Faculty Password: ");
        scanf("%s", pass);

        int count, new_count = 0, deleted = 0;
        Faculty *faculty_list = read_all_faculty(&count);
        Faculty *new_list = (Faculty *)malloc(count * sizeof(Faculty));

        for (int i = 0; i < count; i++) {
            if (faculty_list[i].rollNo == rollNo && strcmp(faculty_list[i].password, pass) == 0) {
                deleted = 1;
                printf("Faculty %s (Roll No: %d) deleted.\n", faculty_list[i].name, rollNo);
            } else {
                new_list[new_count++] = faculty_list[i];
            }
        }
        free(faculty_list);

        if (deleted) {
            FILE *fp = fopen(DATA_FILE_FACULTY, "wb"); // Overwrite file
            if (fp) {
                fwrite(new_list, sizeof(Faculty), new_count, fp);
                fclose(fp);
            } else {
                perror("Error saving faculty data after deletion");
            }
        } else {
            printf("Faculty Roll No or Password incorrect. No faculty deleted.\n");
        }
        free(new_list);

    } else if (type == 2) { // Delete Class Data
        printf("Enter Class Name (e.g., CS-A): ");
        scanf("%s", class_name);
        printf("Enter Academic Year (e.g., 2024): ");
        if (scanf("%d", &year) != 1) { printf("Invalid Year.\n"); return; }

        // Deleting Class records (Subject/Faculty mappings)
        int c_count, c_new_count = 0, c_deleted = 0;
        Class *class_list = read_all_classes(&c_count);
        Class *c_new_list = (Class *)malloc(c_count * sizeof(Class));

        for (int i = 0; i < c_count; i++) {
            if (strcmp(class_list[i].className, class_name) == 0 && class_list[i].year == year) {
                c_deleted = 1;
                printf("Class/Subject mapping: %s (Year %d, Subject %s) deleted.\n", class_list[i].className, class_list[i].year, class_list[i].subject);
            } else {
                c_new_list[c_new_count++] = class_list[i];
            }
        }
        free(class_list);
        if (c_deleted) {
            FILE *fp = fopen(DATA_FILE_CLASS, "wb");
            if (fp) {
                fwrite(c_new_list, sizeof(Class), c_new_count, fp);
                fclose(fp);
            }
        }
        free(c_new_list);
        
        // Deleting associated Student records
        int s_count, s_new_count = 0, s_deleted = 0;
        Student *student_list = read_all_students(&s_count);
        Student *s_new_list = (Student *)malloc(s_count * sizeof(Student));

        for (int i = 0; i < s_count; i++) {
            if (strcmp(student_list[i].className, class_name) == 0 && student_list[i].year == year) {
                s_deleted = 1;
                printf("Student %s (Roll No: %d) deleted from class.\n", student_list[i].name, student_list[i].rollNo);
            } else {
                s_new_list[s_new_count++] = student_list[i];
            }
        }
        free(student_list);
        if (s_deleted) {
            FILE *fp = fopen(DATA_FILE_STUDENT, "wb");
            if (fp) {
                fwrite(s_new_list, sizeof(Student), s_new_count, fp);
                fclose(fp);
            }
        }
        free(s_new_list);

        if (!c_deleted && !s_deleted) {
            printf("No matching Class or Students found to delete.\n");
        }

    } else if (type == 3) { // Delete Particular Student Data
        printf("Enter Student Roll No: ");
        if (scanf("%d", &rollNo) != 1) { printf("Invalid Roll No.\n"); return; }
        printf("Enter Student's unique password: ");
        scanf("%s", pass);

        int count, new_count = 0, deleted = 0;
        Student *student_list = read_all_students(&count);
        Student *new_list = (Student *)malloc(count * sizeof(Student));

        for (int i = 0; i < count; i++) {
            if (student_list[i].rollNo == rollNo && strcmp(student_list[i].password, pass) == 0) {
                deleted = 1;
                printf("Student %s (Roll No: %d) deleted.\n", student_list[i].name, rollNo);
            } else {
                new_list[new_count++] = student_list[i];
            }
        }
        free(student_list);

        if (deleted) {
            FILE *fp = fopen(DATA_FILE_STUDENT, "wb"); // Overwrite file
            if (fp) {
                fwrite(new_list, sizeof(Student), new_count, fp);
                fclose(fp);
            } else {
                perror("Error saving student data after deletion");
            }
        } else {
            printf("Student Roll No or Password incorrect. No student deleted.\n");
        }
        free(new_list);

    } else {
        printf("Invalid option.\n");
    }
}

void admin_report_module() {
    char special_pass[MAX_PASS];
    printf("\n--- Report Module ---\n");
    printf("Enter special password to view reports: ");
    scanf("%s", special_pass);

    if (strcmp(special_pass, REPORT_PASSWORD) != 0) {
        printf("Special password incorrect. Access denied.\n");
        return;
    }

    int choice;
    printf("\nView Report For:\n1. Faculty\n2. Class & Students\nEnter choice (1-2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice.\n"); return;
    }

    if (choice == 1) {
        int count;
        Faculty *faculty_list = read_all_faculty(&count);
        printf("\n\n------------------------------------------------------\n");
        printf("|              FACULTY DATA REPORT                   |\n");
        printf("------------------------------------------------------\n");
        printf("| Roll No | %-30s | Password           |\n", "Name");
        printf("------------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            printf("| %-7d | %-30s | %-18s |\n", faculty_list[i].rollNo, faculty_list[i].name, faculty_list[i].password);
        }
        printf("------------------------------------------------------\n");
        if (faculty_list) free(faculty_list);
    } else if (choice == 2) {
        int s_count, c_count;
        Student *student_list = read_all_students(&s_count);
        Class *class_list = read_all_classes(&c_count);

        printf("\n\n-------------------------------------------------------------------\n");
        printf("|                     CLASS MAPPING REPORT                        |\n");
        printf("-------------------------------------------------------------------\n");
        printf("| %-15s | %-15s | %-20s | Faculty Roll |\n", "Class", "Academic Year", "Subject");
        printf("-------------------------------------------------------------------\n");
        for (int i = 0; i < c_count; i++) {
            printf("| %-15s | %-15d | %-20s | %-12d |\n", 
                   class_list[i].className, class_list[i].year, class_list[i].subject, class_list[i].facultyRollNo);
        }
        printf("-------------------------------------------------------------------\n");
        
        printf("\n\n--------------------------------------------------------------------------\n");
        printf("|                        STUDENT DATA REPORT                             |\n");
        printf("--------------------------------------------------------------------------\n");
        printf("| Roll No | %-30s | Class | Academic Year | Password           |\n", "Name");
        printf("--------------------------------------------------------------------------\n");
        for (int i = 0; i < s_count; i++) {
            printf("| %-7d | %-30s | %-5s | %-13d | %-18s |\n", 
                   student_list[i].rollNo, student_list[i].name, student_list[i].className, student_list[i].year, student_list[i].password);
        }
        printf("--------------------------------------------------------------------------\n");

        if (student_list) free(student_list);
        if (class_list) free(class_list);

    } else {
        printf("Invalid option.\n");
    }
}

void admin_portal() {
    int choice;
    do {
        printf("\n============================================\n");
        printf("             ADMIN SERVICE PORTAL           \n");
        printf("============================================\n");
        printf("1. Add Faculty\n");
        printf("2. Add Class Details\n");
        printf("3. Add Students Details\n");
        printf("4. Change Password \n");
        printf("5. Delete Module \n");
        printf("6. Report Module\n");
        printf("7. Logout\n");
        printf("Enter your service choice (1-7): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear input buffer
            while (getchar() != '\n'); 
            choice = 0; 
            continue;
        }

        switch (choice) {
            case 1: admin_add_faculty(); break;
            case 2: admin_add_class(); break;
            case 3: admin_add_students(); break;
            case 4: admin_change_password(); break;
            case 5: admin_delete_module(); break;
            case 6: admin_report_module(); break;
            case 7: printf("Logging out of Admin Portal.\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

// --- Faculty Module Services ---

void faculty_add_attendance(int f_rollNo) {
    char class_name[MAX_CLASS], subject[MAX_SUBJECT], date[11];
    int year;

    printf("\n--- Add Attendance Module ---\n");
    printf("Enter Class Name (e.g., CS-A): ");
    scanf("%s", class_name);
    printf("Enter Academic Year (e.g., 2024 - must be 4 digits): ");
    if (scanf("%d", &year) != 1 || year < 1900 || year > 2100) {
        printf("Invalid Academic Year input. Must be a 4-digit year between 1900 and 2100.\n"); 
        // Clear input buffer
        while (getchar() != '\n'); 
        return;
    }
    printf("Enter Subject Name (e.g., DataStructures): ");
    scanf("%s", subject);
    printf("Enter Date (YYYY-MM-DD) or type 'today' for today's date: ");
    scanf("%s", date);

    if (strcmp(date, "today") == 0) {
        get_current_date(date);
    }

    // Validation: Check if the faculty is authorized for this class/subject
    int c_count;
    Class *class_list = read_all_classes(&c_count);
    int authorized = 0;
    for (int i = 0; i < c_count; i++) {
        if (strcmp(class_list[i].className, class_name) == 0 && 
            class_list[i].year == year && 
            strcmp(class_list[i].subject, subject) == 0 &&
            class_list[i].facultyRollNo == f_rollNo) {
            authorized = 1;
            break;
        }
    }
    if (class_list) free(class_list);

    if (!authorized) {
        printf("Error: You are not authorized to take attendance for Class %s, Academic Year %d, Subject %s.\n", class_name, year, subject);
        return;
    }

    // Get list of students in this class/year
    int s_count;
    Student *all_students = read_all_students(&s_count);
    Student *class_students[s_count];
    int class_student_count = 0;

    for (int i = 0; i < s_count; i++) {
        if (strcmp(all_students[i].className, class_name) == 0 && all_students[i].year == year) {
            class_students[class_student_count++] = &all_students[i];
        }
    }

    if (class_student_count == 0) {
        printf("No students found for Class %s, Academic Year %d.\n", class_name, year);
        if (all_students) free(all_students);
        return;
    }

    FILE *fp = fopen(DATA_FILE_ATTENDANCE, "ab");
    if (!fp) {
        perror("Error opening attendance data file");
        if (all_students) free(all_students);
        return;
    }

    printf("\n--- Taking Attendance for %s (Academic Year %d) on %s ---\n", class_name, year, date);
    for (int i = 0; i < class_student_count; i++) {
        AttendanceRecord ar;
        ar.studentRollNo = class_students[i]->rollNo;
        strcpy(ar.className, class_name);
        strcpy(ar.subject, subject);
        strcpy(ar.date, date);

        printf("Roll No %d (%s) - Present (1) / Absent (0): ", class_students[i]->rollNo, class_students[i]->name);
        if (scanf("%d", &ar.isPresent) != 1 || (ar.isPresent != 0 && ar.isPresent != 1)) {
            printf("Invalid input. Marking as Absent (0).\n");
            ar.isPresent = 0;
            // Clear input buffer
            while (getchar() != '\n');
        }

        fwrite(&ar, sizeof(AttendanceRecord), 1, fp);
    }

    fclose(fp);
    printf("\nAttendance recorded successfully for %d students.\n", class_student_count);
    if (all_students) free(all_students);
}

void faculty_modify_attendance(int f_rollNo) {
    char class_name[MAX_CLASS], subject[MAX_SUBJECT], date[11];
    int year, mod_type, rollNo;

    printf("\n--- Modify Attendance Module ---\n");
    printf("Enter Class Name: ");
    scanf("%s", class_name);
    printf("Enter Academic Year (e.g., 2024): ");
    if (scanf("%d", &year) != 1) { printf("Invalid Year.\n"); return; }
    printf("Enter Subject Name: ");
    scanf("%s", subject);
    printf("Enter Date of attendance to modify (YYYY-MM-DD): ");
    scanf("%s", date);

    // Validation: Check if the faculty is authorized for this class/subject
    int c_count;
    Class *class_list = read_all_classes(&c_count);
    int authorized = 0;
    for (int i = 0; i < c_count; i++) {
        if (strcmp(class_list[i].className, class_name) == 0 && 
            class_list[i].year == year && 
            strcmp(class_list[i].subject, subject) == 0 &&
            class_list[i].facultyRollNo == f_rollNo) {
            authorized = 1;
            break;
        }
    }
    if (class_list) free(class_list);
    if (!authorized) {
        printf("Error: You are not authorized to modify attendance for this Subject/Class.\n");
        return;
    }

    printf("Modify for:\n1. Entire Class\n2. Particular Student\nEnter choice (1-2): ");
    if (scanf("%d", &mod_type) != 1) { printf("Invalid choice.\n"); return; }

    int a_count;
    FILE *fp = fopen(DATA_FILE_ATTENDANCE, "r+b");
    if (!fp) {
        perror("Error opening attendance data file");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size == 0) {
        printf("No attendance records found to modify.\n");
        fclose(fp);
        return;
    }

    a_count = size / sizeof(AttendanceRecord);
    AttendanceRecord ar;

    if (mod_type == 1) { // Entire Class
        printf("\n--- Modifying Entire Class Attendance for %s (%s) on %s ---\n", class_name, subject, date);
        for (int i = 0; i < a_count; i++) {
            fseek(fp, i * sizeof(AttendanceRecord), SEEK_SET);
            fread(&ar, sizeof(AttendanceRecord), 1, fp);

            if (strcmp(ar.className, class_name) == 0 && 
                strcmp(ar.subject, subject) == 0 &&
                strcmp(ar.date, date) == 0) {
                
                // Find student name for display
                int s_count;
                Student *all_students = read_all_students(&s_count);
                char student_name[MAX_NAME] = "Unknown";
                for(int j = 0; j < s_count; j++) {
                    if (all_students[j].rollNo == ar.studentRollNo) {
                        strcpy(student_name, all_students[j].name);
                        break;
                    }
                }
                if (all_students) free(all_students);

                printf("Roll No %d (%s) - Current: %s. New Status (1/0): ", ar.studentRollNo, student_name, ar.isPresent ? "Present" : "Absent");
                int new_status;
                if (scanf("%d", &new_status) != 1 || (new_status != 0 && new_status != 1)) {
                    printf("Invalid input. Keeping current status.\n");
                    // Clear input buffer
                    while (getchar() != '\n');
                    continue;
                }
                ar.isPresent = new_status;
                fseek(fp, i * sizeof(AttendanceRecord), SEEK_SET); // Seek back
                fwrite(&ar, sizeof(AttendanceRecord), 1, fp); // Write updated record
            }
        }
    } else if (mod_type == 2) { // Particular Student
        printf("Enter Student Roll Number: ");
        if (scanf("%d", &rollNo) != 1) { printf("Invalid Roll No.\n"); fclose(fp); return; }
        
        int found = 0;
        for (int i = 0; i < a_count; i++) {
            fseek(fp, i * sizeof(AttendanceRecord), SEEK_SET);
            fread(&ar, sizeof(AttendanceRecord), 1, fp);

            if (ar.studentRollNo == rollNo &&
                strcmp(ar.className, class_name) == 0 && 
                strcmp(ar.subject, subject) == 0 &&
                strcmp(ar.date, date) == 0) {
                
                printf("Found record for Roll No %d on %s. Current Status: %s.\n", rollNo, date, ar.isPresent ? "Present" : "Absent");
                printf("Enter New Status (1 for Present, 0 for Absent): ");
                int new_status;
                if (scanf("%d", &new_status) != 1 || (new_status != 0 && new_status != 1)) {
                    printf("Invalid input. Modification aborted.\n");
                    // Clear input buffer
                    while (getchar() != '\n');
                    fclose(fp);
                    return;
                }
                ar.isPresent = new_status;
                fseek(fp, i * sizeof(AttendanceRecord), SEEK_SET);
                fwrite(&ar, sizeof(AttendanceRecord), 1, fp);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Attendance record not found for Roll No %d on %s.\n", rollNo, date);
        }
    } else {
        printf("Invalid modification choice.\n");
    }

    fclose(fp);
    printf("\nAttendance modification complete.\n");
}

void faculty_portal(int rollNo) {
    int choice;
    do {
        printf("\n============================================\n");
        printf("             FACULTY SERVICE PORTAL         \n");
        printf("============================================\n");
        printf("1. Add Attendance\n");
        printf("2. Modify Attendance\n");
        printf("3. Logout\n");
        printf("Enter your service choice (1-3): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear input buffer
            while (getchar() != '\n'); 
            choice = 0; 
            continue;
        }

        switch (choice) {
            case 1: faculty_add_attendance(rollNo); break;
            case 2: faculty_modify_attendance(rollNo); break;
            case 3: printf("Logging out of Faculty Portal.\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

// --- Student Module Service ---

void student_view_report(int s_rollNo) {
    int s_count;
    Student *student_list = read_all_students(&s_count);
    Student current_student;
    int found_s = 0;

    for (int i = 0; i < s_count; i++) {
        if (student_list[i].rollNo == s_rollNo) {
            current_student = student_list[i];
            found_s = 1;
            break;
        }
    }

    if (!found_s) {
        printf("Error: Student record not found.\n");
        if (student_list) free(student_list);
        return;
    }
    if (student_list) free(student_list);

    // Get all classes/subjects the student is enrolled in
    int c_count;
    Class *class_list = read_all_classes(&c_count);
    
    // Aggregate attendance data
    int a_count;
    FILE *fp = fopen(DATA_FILE_ATTENDANCE, "rb");
    if (!fp) {
        printf("No attendance records have been entered yet.\n");
        if (class_list) free(class_list);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    a_count = size / sizeof(AttendanceRecord);

    AttendanceRecord *all_attendance = (AttendanceRecord *)malloc(size);
    if (!all_attendance) {
        perror("Memory allocation failed for attendance records");
        fclose(fp);
        if (class_list) free(class_list);
        return;
    }
    fread(all_attendance, sizeof(AttendanceRecord), a_count, fp);
    fclose(fp);

    // Group attendance by subject
    typedef struct {
        char subject[MAX_SUBJECT];
        int totalClasses;
        int presentClasses;
    } AttendanceSummary;

    // A simple array to store summaries for up to MAX_CLASS mappings
    AttendanceSummary summaries[c_count];
    int summary_count = 0;

    for (int i = 0; i < c_count; i++) {
        // Only consider subjects relevant to the student's class/year
        if (strcmp(class_list[i].className, current_student.className) == 0 && 
            class_list[i].year == current_student.year) {
            
            // Check if subject already processed (due to possible multiple faculty for one subject, though simplified here)
            int summary_index = -1;
            for(int k = 0; k < summary_count; k++) {
                if (strcmp(summaries[k].subject, class_list[i].subject) == 0) {
                    summary_index = k;
                    break;
                }
            }

            if (summary_index == -1) {
                // Initialize new summary entry
                summary_index = summary_count++;
                strcpy(summaries[summary_index].subject, class_list[i].subject);
                summaries[summary_index].totalClasses = 0;
                summaries[summary_index].presentClasses = 0;
            }

            // Iterate through all attendance records
            for (int j = 0; j < a_count; j++) {
                if (all_attendance[j].studentRollNo == s_rollNo &&
                    strcmp(all_attendance[j].subject, class_list[i].subject) == 0 &&
                    strcmp(all_attendance[j].className, current_student.className) == 0) {
                    
                    summaries[summary_index].totalClasses++;
                    if (all_attendance[j].isPresent == 1) {
                        summaries[summary_index].presentClasses++;
                    }
                }
            }
        }
    }
    
    printf("\n======================================================\n");
    printf("         STUDENT ATTENDANCE REPORT: %s (Roll No: %d)       \n", current_student.name, current_student.rollNo);
    printf("         Academic Year: %d\n", current_student.year);
    printf("======================================================\n");
    printf("| %-20s | Total Classes | Present Classes | Percentage |\n", "Subject");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < summary_count; i++) {
        float percentage = 0.0;
        if (summaries[i].totalClasses > 0) {
            percentage = ((float)summaries[i].presentClasses / summaries[i].totalClasses) * 100.0;
        }
        printf("| %-20s | %-13d | %-15d | %-9.2f%% |\n",
               summaries[i].subject,
               summaries[i].totalClasses,
               summaries[i].presentClasses,
               percentage);
    }
    printf("------------------------------------------------------\n");

    if (all_attendance) free(all_attendance);
    if (class_list) free(class_list);
}

// --- Main Program Logic ---

void run_login(int role) {
    int rollNo;
    char password[MAX_PASS];

    printf("Enter Roll Number: ");
    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid Roll Number.\n"); return;
    }
    printf("Enter Password: ");
    scanf("%s", password);

    if (role == 1) { // Admin
        if (rollNo == ADMIN_ROLL && strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("\n--- Admin Login Successful ---\n");
            admin_portal();
        } else {
            printf("\nAdmin Roll No or Password incorrect.\n");
        }
    } else if (role == 2) { // Faculty
        int count, found = 0;
        Faculty *faculty_list = read_all_faculty(&count);
        for (int i = 0; i < count; i++) {
            if (faculty_list[i].rollNo == rollNo && strcmp(faculty_list[i].password, password) == 0) {
                printf("\n--- Faculty Login Successful: %s ---\n", faculty_list[i].name);
                found = 1;
                faculty_portal(rollNo);
                break;
            }
        }
        if (faculty_list) free(faculty_list);
        if (!found) {
            printf("\nFaculty Roll No or Password incorrect.\n");
        }
    } else if (role == 3) { // Student
        int count, found = 0;
        Student *student_list = read_all_students(&count);
        for (int i = 0; i < count; i++) {
            if (student_list[i].rollNo == rollNo && strcmp(student_list[i].password, password) == 0) {
                printf("\n--- Student Login Successful: %s ---\n", student_list[i].name);
                found = 1;
                student_view_report(rollNo);
                break;
            }
        }
        if (student_list) free(student_list);
        if (!found) {
            printf("\nStudent Roll No or Password incorrect.\n");
        }
    }
}

int main() {
    int role;
    printf("============================================\n");
    printf("  ATTENDANCE MANAGEMENT SYSTEM INITIALIZED  \n");
    printf("============================================\n");
    printf("Select Your Role:\n1. Admin\n2. Faculty\n3. Student\n4. Exit\n");
    printf("Enter your choice (1-4): ");
    
    while (scanf("%d", &role) == 1 && role != 4) {
        if (role >= 1 && role <= 3) {
            run_login(role);
        } else {
            printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
        }
        
        printf("\n============================================\n");
        printf("Select Your Role:\n1. Admin\n2. Faculty\n3. Student\n4. Exit\n");
        printf("Enter your choice (1-4): ");
    }
    
    printf("\nThank you for using the Attendance Management System. Goodbye!\n");
    return 0;
}
