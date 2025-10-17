#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <ctype.h>
#include <string.h>

void swapValues(int* pointerA, int* pointerB) {
    int tempValue = *pointerA;
    *pointerA = *pointerB;
    *pointerB = tempValue;
}

void smoothImageMatrix(int matrixSide, int** imageMatrix) {
    int* previousRowBuffer = (int*)malloc(matrixSide * sizeof(int));
    int* currentRowBuffer = (int*)malloc(matrixSide * sizeof(int));

    if (previousRowBuffer == NULL || currentRowBuffer == NULL) {
        fprintf(stderr, "Buffer memory allocation failed.\n");
        return;
    }

    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        for (int bufferIndex = 0; bufferIndex < matrixSide; bufferIndex++) {
            *(currentRowBuffer + bufferIndex) = *(*(imageMatrix + rowIndex) + bufferIndex);
        }

        for (int columnIndex = 0; columnIndex < matrixSide; columnIndex++) {
            int neighborSum = 0;
            int neighborCount = 0;

            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaColumn = -1; deltaColumn <= 1; deltaColumn++) {
                    int neighborRow = rowIndex + deltaRow;
                    int neighborColumn = columnIndex + deltaColumn;

                    if (neighborRow >= 0 && neighborRow < matrixSide &&
                        neighborColumn >= 0 && neighborColumn < matrixSide) {
                        
                        neighborCount++;

                        if (deltaRow == -1) {
                            neighborSum += *(previousRowBuffer + neighborColumn);
                        } else if (deltaRow == 0) {
                            neighborSum += *(currentRowBuffer + neighborColumn);
                        } else { 
                            neighborSum += *(*(imageMatrix + neighborRow) + neighborColumn);
                        }
                    }
                }
            }
            if (neighborCount > 0) {
                *(*(imageMatrix + rowIndex) + columnIndex) = neighborSum / neighborCount;
            }
        }
        for (int bufferIndex = 0; bufferIndex < matrixSide; bufferIndex++) {
            *(previousRowBuffer + bufferIndex) = *(currentRowBuffer + bufferIndex);
        }
    }
    free(previousRowBuffer);
    free(currentRowBuffer);
}

void printMatrix(int** imageMatrix, int matrixSide) {
    printf("\n");
    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSide; columnIndex++) {
            printf("%d\t", *(*(imageMatrix + rowIndex) + columnIndex));
        }
        printf("\n");
    }
}

void reverseRow(int rowSize, int* imageRow) {
    int* frontPointer = imageRow;
    int* backPointer = imageRow + rowSize - 1;
    while (frontPointer < backPointer) {
        swapValues(frontPointer, backPointer);
        frontPointer++;
        backPointer--;
    }
}

void transposeMatrix(int matrixSide, int** imageMatrix) {
    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        for (int columnIndex = rowIndex + 1; columnIndex < matrixSide; columnIndex++) {
            swapValues((*(imageMatrix + rowIndex) + columnIndex), (*(imageMatrix + columnIndex) + rowIndex));
        }
    }
}

void rotateImageMatrix(int matrixSide, int** imageMatrix) {
    transposeMatrix(matrixSide, imageMatrix);
    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        reverseRow(matrixSide, *(imageMatrix + rowIndex));
    }
}

int main(){
    printf("Enter the dimension(Side) of the Matrix in range(2-10): ");
   char inputBuffer[100];
    char* endPointer;
    int matrixSide = 0;
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input line.\n");
        return 1;
    }
    long conversionResult = strtol(inputBuffer, &endPointer, 10);
    if (endPointer == inputBuffer) {
        fprintf(stderr, "Invalid input. No number was found.\n");
        return 1;
    }
    while (isspace((unsigned char)*endPointer)) {
        endPointer++;
    }
    if (*endPointer != '\0') {
        fprintf(stderr, "Invalid input. Please enter only a single whole number.\n");
        return 1;
    }
    matrixSide = (int)conversionResult;
    if (matrixSide < 2 || matrixSide > 10) {
        fprintf(stderr, "Invalid input. Number must be between 2 and 10.\n");
        return 1;
    }

    int** matrix = (int**)malloc(matrixSide * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        *(matrix + rowIndex) = (int*)malloc(matrixSide * sizeof(int));
        if (*(matrix + rowIndex) == NULL) {
            fprintf(stderr, "Memory allocation for row %d failed.\n", rowIndex);
            return 1;
        }
    }
    
    srand(time(NULL));

    for (int rowIndex = 0;  rowIndex < matrixSide;  rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSide; columnIndex++) {
            *(*(matrix +  rowIndex) + columnIndex) = rand() % 256;
        }
    }

    printf("Original Randomly Generated Matrix:");
    printMatrix(matrix, matrixSide);
    
    rotateImageMatrix(matrixSide, matrix);
    printf("\nMatrix after 90 Clockwise Rotation:");
    printMatrix(matrix, matrixSide);
    
    smoothImageMatrix(matrixSide, matrix);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:");
    printMatrix(matrix, matrixSide);


    for (int rowIndex = 0; rowIndex < matrixSide; rowIndex++) {
        free(*(matrix + rowIndex));
    }
    free(matrix);
    matrix = NULL;

    return 0;
}