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

int main()
{   
    printf("Enter the number of Students ");
    int numberStudents = 0;
    scanf("%d",&numberStudents);
    getchar();
    
    struct studentRecord studentsArray[numberStudents];
    char lineBufferForInput[100];
    
    for(int i=0;i<numberStudents;i++){
        printf("Enter the student details separated by space ");
        fgets(lineBufferForInput , sizeof(lineBufferForInput) , stdin);
        
        sscanf(lineBufferForInput , "%d %s %d %d %d",
                &studentsArray[i].studentRollNumber,
                studentsArray[i].studentName,
                &studentsArray[i].studentMarks[0],
                &studentsArray[i].studentMarks[1],
                &studentsArray[i].studentMarks[2]);
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
    
    printf("\n Recursion for Roll Number of students (via Recursion) :");
    printNumberRecursion(0, numberStudents , studentsArray);
    

    return 0;
}
