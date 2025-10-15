#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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
    for (int i = 0; i < matrixSide; i++) {
        for (int j = 0; j < matrixSide; j++) {
            printf("%d\t", *(*(imageMatrix+ i) + j));
        }
        printf("\n");
    }
}

void reverseRow(int rowSize, int* imageRow) {
    int* frontPointer = imageRow;
    int* backPointer = imageRow + rowSize - 1;
    while (frontPointer < backPointer) {
        int temp = *frontPointer;
        *frontPointer = *backPointer;
        *backPointer = temp;
        frontPointer++;
        backPointer--;
    }
}
void transposeMatrix(int sideLength, int** imageMatrix) {
    for (int i = 0; i < sideLength; i++) {
        for (int j = i + 1; j < sideLength; j++) {
            // Swaping values
            int tempVariable = *(*(imageMatrix + i) + j);
            *(*(imageMatrix + i) + j) = *(*(imageMatrix + j) + i);
            *(*(imageMatrix + j) + i) = tempVariable;
        }
    }
}

void rotateImageMatrix(int sideLength, int** imageMatrix) {
    transposeMatrix(sideLength, imageMatrix);
    for (int i = 0; i < sideLength; i++) {
        reverseRow(sideLength, *(imageMatrix + i));
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
    
    smoothImageMatrix(matrixSide, matrix);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:");
    printMatrix(matrix, matrixSide);


    for (int i = 0; i < matrixSide; i++) {
        free(*(matrix + i));
    }
    free(matrix);
    matrix = NULL;

    return 0;
}