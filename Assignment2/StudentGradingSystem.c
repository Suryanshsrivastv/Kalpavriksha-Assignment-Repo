#include <stdio.h>

struct studentRecord{
    int studentRollNumber;
    char studentName[50];
    float studentMarks[3];
};

float getStudentTotalMarks(float studentMarks[]){
    float marks = 0.0f;
    for(int subjectIndex = 0; subjectIndex < 3; subjectIndex++){
        marks += studentMarks[subjectIndex];
    }
    return marks;
}

void printStudentDetails(struct studentRecord student, float studentAverageMarks, float studentTotalMarks, char studentGrade){
    printf(" Roll: %d \n Name: %s \n Total: %.2f \n Average: %.2f \n Grade %c \n ",
    student.studentRollNumber, student.studentName, studentTotalMarks, studentAverageMarks, studentGrade);
}

void printPerformancePattern(char studentGrade) {
    printf("Performance: ");
    if (studentGrade == 'A') {
        printf("*****");
    } else if (studentGrade == 'B') {
        printf("****");
    } else if (studentGrade == 'C') {
        printf("***");
    } else if (studentGrade == 'D') {
        printf("**");
    }
    printf("\n");
}

char getStudentGrade(float averageMarks) {
    if (averageMarks >= 85.0) {
        return 'A';
    } else if (averageMarks >= 70.0) {
        return 'B';
    } else if (averageMarks >= 50.0) {
        return 'C';
    } else if (averageMarks >= 35.0) {
        return 'D';
    } else {
        return 'F';
    }
}


float calculateStudentAverageMarks(float studentsTotalMarks){
    return studentsTotalMarks / 3.0f;
}

void printNumberRecursion(int index, int numberStudents, struct studentRecord studentsArray[]){
    if(index > numberStudents - 1){
        return;
    }
    printf("%d ", studentsArray[index].studentRollNumber);
    printNumberRecursion(index + 1, numberStudents, studentsArray);
}


int areMarksValid(float marks[]) {
    for (int subjectIndex = 0; subjectIndex < 3; subjectIndex++) {
        if (marks[subjectIndex] < 0 || marks[subjectIndex] > 100) {
            return 0;
        }
    }
    return 1;
}

int main() {
    printf("Enter the number of students in range (1-100): ");
    char lineBufferForInput[100];
    int numberStudents = 0;

    fgets(lineBufferForInput, sizeof(lineBufferForInput), stdin);

    if (sscanf(lineBufferForInput, "%d", &numberStudents) != 1) {
        fprintf(stderr, "Error: Invalid input. Please enter a whole number for the number of students.\n");
        return 1;
    }

    if (numberStudents < 1 || numberStudents > 100) {
        fprintf(stderr, "Error: Number of students must be between 1 and 100.\n");
        return 1;
    }
    
    struct studentRecord studentsArray[numberStudents];
    
    for(int studentIndex = 0; studentIndex < numberStudents; studentIndex++){
        printf("Enter details for student %d (Roll Name Marks1(0-100) Marks2(0-100) Marks3(0-100)): ", studentIndex + 1);
        fgets(lineBufferForInput, sizeof(lineBufferForInput), stdin);
        
        int itemsScanned = sscanf(lineBufferForInput, "%d %s %f %f %f",
               &studentsArray[studentIndex].studentRollNumber,
               studentsArray[studentIndex].studentName,
               &studentsArray[studentIndex].studentMarks[0],
               &studentsArray[studentIndex].studentMarks[1],
               &studentsArray[studentIndex].studentMarks[2]);
        
        if (itemsScanned != 5) {
           fprintf(stderr, "Error: Invalid input format. Please use 'Roll Name Marks1 Marks2 Marks3'.\n");
           return 1;
        }

        if (studentsArray[studentIndex].studentRollNumber < 0) {
            fprintf(stderr, "Error: Roll number cannot be negative.\n");
            return 1;
        }

        if (!areMarksValid(studentsArray[studentIndex].studentMarks)) {
            fprintf(stderr, "Error: Marks for student %s (Roll %d) must be between 0 and 100.\n",
                    studentsArray[studentIndex].studentName, studentsArray[studentIndex].studentRollNumber);
            return 1;
        }
    }
    
    for(int studentIndex = 0; studentIndex < numberStudents; studentIndex++){
        float studentsTotalMarks = getStudentTotalMarks(studentsArray[studentIndex].studentMarks);
        float studentAverageMarks = calculateStudentAverageMarks(studentsTotalMarks);
        char studentGrade = getStudentGrade(studentAverageMarks);
        printStudentDetails(studentsArray[studentIndex], studentAverageMarks, studentsTotalMarks, studentGrade);
        if(studentGrade != 'F'){
            printPerformancePattern(studentGrade);
        }
        printf("\n");
    }
    
    printf("\n Recursion for Roll Number of students :");
    printNumberRecursion(0, numberStudents, studentsArray);
    
    return 0;
}