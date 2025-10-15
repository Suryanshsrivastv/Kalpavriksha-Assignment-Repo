#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void printMatrix(int** imageMatrix, int matrixSide) {
    printf("\n");
    for (int i = 0; i < matrixSide; i++) {
        for (int j = 0; j < matrixSide; j++) {
            printf("%d\t", *(*(imageMatrix+ i) + j));
        }
        printf("\n");
    }
}

void reverseRow(int rowSize, int* imageRow) {
    int* front = imageRow;
    int* back = imageRow + rowSize - 1;
    while (front < back) {
        int temp = *front;
        *front = *back;
        *back = temp;
        front++;
        back--;
    }
}
void transposeMatrix(int sideLength, int** imageMatrix) {
    for (int i = 0; i < sideLength; i++) {
        for (int j = i + 1; j < sideLength; j++) {
            // Swaping values
            int temp = *(*(imageMatrix + i) + j);
            *(*(imageMatrix + i) + j) = *(*(imageMatrix + j) + i);
            *(*(imageMatrix + j) + i) = temp;
        }
    }
}

void rotateImageMatrix(int sideLength, int** matrix) {
    transposeMatrix(sideLength, matrix);
    for (int i = 0; i < sideLength; i++) {
        reverseRow(sideLength, *(matrix + i));
    }
}

int main(){
    printf("Enter the dimension(Side) of the Matrix in range(1-9): ");
    int matrixSide = 0;
    if (scanf("%d", &matrixSide) != 1 || matrixSide < 1 || matrixSide > 10) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    int** matrix = (int**)malloc(matrixSide * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < matrixSide; i++) {
        *(matrix + i) = (int*)malloc(matrixSide * sizeof(int));
        if (*(matrix + i) == NULL) {
            fprintf(stderr, "Memory allocation for row %d failed.\n", i);
            return 1;
        }
    }
    srand(time(NULL));

    for (int i = 0; i < matrixSide; i++) {
        for (int j = 0; j < matrixSide; j++) {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }

    printf("Original Randomly Generated Matrix:");
    printMatrix(matrix, matrixSide);
    
    rotateImageMatrix(matrixSide, matrix);
    printf("\nMatrix after 90 Clockwise Rotation:");
    printMatrix(matrix, matrixSide);
    
    
    for (int i = 0; i < matrixSide; i++) {
        free(*(matrix + i));
    }
    free(matrix);
    matrix = NULL;

    return 0;
}