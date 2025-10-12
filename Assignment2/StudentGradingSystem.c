#include <stdio.h>

struct studentRecord{
        int studentRollNumber;
        char studentName[50];
        int studentMarks[3];
};
 
int getStudentotalMarks(int studentMarks[]){
    int marks = 0;
    for(int i=0;i<3;i++){
        marks += studentMarks[i];
    }
    
    return marks;
}

void printStudentDetails(struct studentRecord student , float studentAverageMarks , int studentTotalMarks , char studentGrade){
    printf(" Roll: %d \n Name: %s \n Total: %d \n Average: %.2f \n Grade %c \n ",
    student.studentRollNumber,student.studentName,studentTotalMarks,studentAverageMarks,studentGrade);
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

float calculateStudentAverageMarks(int studentsTotalMarks){
    float averageMarks = studentsTotalMarks / 3.0;
    return averageMarks;
}

void printNumberRecursion(int index ,int numberStudents, struct studentRecord studentsArray[]){
    if(index > numberStudents-1){
        return;
    }
    printf("%d ",studentsArray[index].studentRollNumber);
    printNumberRecursion(index+1,numberStudents,studentsArray);
}

int areMarksValid(int marks[]) {
    for (int i = 0; i < 3; i++) {
        if (marks[i] < 0 || marks[i] > 100) {
            return 0;
        }
    }
    return 1;
}

int main()
{   
    printf("Enter the number of students in range (1-100): ");
    int numberStudents = 0;
    scanf("%d",&numberStudents);

    if (numberStudents < 1 || numberStudents > 100) {
    fprintf(stderr, "Error: Number of students must be between 1 and 100.\n");
    return 1;
    }

    getchar();
    
    struct studentRecord studentsArray[numberStudents];
    char lineBufferForInput[100];
    
    for(int i=0;i<numberStudents;i++){
        printf("Enter details for student %d (Roll Name Marks1 Marks2 Marks3): ", i + 1);
        fgets(lineBufferForInput , sizeof(lineBufferForInput) , stdin);
        
        sscanf(lineBufferForInput , "%d %s %d %d %d",
                &studentsArray[i].studentRollNumber,
                studentsArray[i].studentName,
                &studentsArray[i].studentMarks[0],
                &studentsArray[i].studentMarks[1],
                &studentsArray[i].studentMarks[2]
            );

        if (!areMarksValid(studentsArray[i].studentMarks)) {
            fprintf(stderr, "Error: Marks for student %s (Roll %d) must be between 0 and 100.\n",
                studentsArray[i].studentName, studentsArray[i].studentRollNumber);
                return 1;
        }
    }
    
    for(int i=0;i<numberStudents;i++){
        int studentsTotalMarks = getStudentotalMarks(studentsArray[i].studentMarks);
        float studentAverageMarks = calculateStudentAverageMarks(studentsTotalMarks);
        char studentGrade = getStudentGrade(studentAverageMarks);
        printStudentDetails(studentsArray[i] , studentAverageMarks ,  studentsTotalMarks , studentGrade);
        if(studentGrade != 'F'){
            printPerformancePattern(studentGrade);
        }
        printf("\n");
    }
    
    printf("\n Recursion for Roll Number of students :");
    printNumberRecursion(0, numberStudents , studentsArray);
    

    return 0;
}
