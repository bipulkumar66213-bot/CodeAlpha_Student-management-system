#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

// Structure Definition
struct Student {
    int roll;
    char name[50];
    char branch[30];
    float marks; // Sahi kiya: 'oat' ko 'float' banaya
};

// Function Prototypes
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    do {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return 0;
            default: printf("Invalid Choice!\n");
        }
    } while (choice != 6);

    return 0;
}

// 1. Add Student Function
void addStudent() {
    FILE *fp;
    struct Student s;
    
    fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Branch: ");
    scanf(" %[^\n]", s.branch);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);
    printf("Student record added successfully!\n");
}

// 2. Display All Students Function
void displayStudents() {
    FILE *fp;
    struct Student s;
    int found = 0;

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found! (File not created yet)\n");
        return;
    }

    printf("\n%-10s%-20s%-15s%-10s\n", "Roll", "Name", "Branch", "Marks");
    printf("------------------------------------------------------------\n");

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        printf("%-10d%-20s%-15s%-10.2f\n", s.roll, s.name, s.branch, s.marks);
        found = 1;
    }

    fclose(fp);

    if (!found) {
        printf("No student records available.\n");
    }
}

// 3. Search Student Function
void searchStudent() {
    FILE *fp;
    struct Student s;
    int roll, found = 0;

    printf("\nEnter Roll Number to search: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\nStudent Found:\n");
            printf("Roll No: %d\nName: %s\nBranch: %s\nMarks: %.2f\n", s.roll, s.name, s.branch, s.marks);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Student with Roll No %d not found.\n", roll);
    }
}

// 4. Update Student Function
void updateStudent() {
    FILE *fp;
    struct Student s;
    int roll, found = 0;

    printf("\nEnter Roll Number to update: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("Enter new Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter new Branch: ");
            scanf(" %[^\n]", s.branch);
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);

            // Sahi kiya: File pointer ko sahi se peeche bheja record overwrite karne ke liye
            fseek(fp, -1 * (long)sizeof(struct Student), SEEK_CUR);
            fwrite(&s, sizeof(struct Student), 1, fp);
            printf("Record updated successfully!\n");
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Student with Roll No %d not found.\n", roll);
    }
}

// 5. Delete Student Function
void deleteStudent() {
    FILE *fp, *temp;
    struct Student s;
    int roll, found = 0;

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1; // Is record ko skip karenge (likhenge nahi), matlab delete ho gaya
        } else {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
    }

    // Sahi kiya: Pehle dono files close hongi, uske baad hi remove aur rename chalega
    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILENAME);
        rename("temp.dat", FILENAME);
        printf("Student with Roll No %d deleted successfully!\n", roll);
    } else {
        remove("temp.dat"); // Agar nahi mila toh temp file delete kar do
        printf("Student with Roll No %d not found.\n", roll);
    }
}