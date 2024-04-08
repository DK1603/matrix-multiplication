// Problem 4
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void addMatrices(int **A, int **B, int **result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}
void addMatricesStr(int **A, int **B, int **result, int n, int *cnt) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] + B[i][j];
            cnt[1]++; // addition count
        }
    }
}

void subtractMatricesStr(int **A, int **B, int **result, int n, int *cnt) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] - B[i][j];
            cnt[2]++; // subtraction count
        }
    }
}

int **allocateMatrix(int n) {
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
    return matrix;
}

void freeMatrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillRandom(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 1000; 
        }
    }
}

void printMatrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void combine(int **C, int **C11, int **C12, int **C21, int **C22, int n) {
    for (int i = 0; i < n; i++) {
        for (int j =0; j < n; j++) {
            C[i][j] = C11[i][j];
            C[i][j+n] = C12[i][j];
            C[i+n][j] = C21[i][j];
            C[i+n][j+n] = C22[i][j];
        }
    }
}

void divideConqMultiply(int **A, int **B, int **C, int n, int *cnt) {

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        cnt[0]++; // multiplication count
        return;
    }

    int halfOfMatrix = n / 2;

    // Dynamic allocation for all submatrices
    int **A11 = allocateMatrix(halfOfMatrix);
    int **A12 = allocateMatrix(halfOfMatrix);
    int **A21 = allocateMatrix(halfOfMatrix), 
        **A22 = allocateMatrix(halfOfMatrix),
        **B11 = allocateMatrix(halfOfMatrix), 
        **B12 = allocateMatrix(halfOfMatrix),
        **B21 = allocateMatrix(halfOfMatrix), 
        **B22 = allocateMatrix(halfOfMatrix),
        **C11 = allocateMatrix(halfOfMatrix), 
        **C12 = allocateMatrix(halfOfMatrix),
        **C21 = allocateMatrix(halfOfMatrix), 
        **C22 = allocateMatrix(halfOfMatrix),
        **temp1 = allocateMatrix(halfOfMatrix), 
        **temp2 = allocateMatrix(halfOfMatrix);

    // Submatrices
    for (int i = 0; i < halfOfMatrix; i++) {
        for (int j = 0; j < halfOfMatrix; j++) {
            int firstHalf = i + halfOfMatrix, secondHalf = j + halfOfMatrix;
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][secondHalf];
            A21[i][j] = A[firstHalf][j];
            A22[i][j] = A[firstHalf][secondHalf];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][secondHalf];
            B21[i][j] = B[firstHalf][j];
            B22[i][j] = B[firstHalf][secondHalf];
        }
    }

    divideConqMultiply(A11, B11, temp1, halfOfMatrix, cnt);
    divideConqMultiply(A12, B21, temp2, halfOfMatrix,cnt);
    addMatrices(temp1, temp2, C11, halfOfMatrix); // C11 = A11*B11 + A12*B21
    cnt[1] += halfOfMatrix * halfOfMatrix;


    divideConqMultiply(A11, B12, temp1, halfOfMatrix,cnt);
    divideConqMultiply(A12, B22, temp2, halfOfMatrix,cnt);
    addMatrices(temp1, temp2, C12, halfOfMatrix); // C12 = A11*B12 + A12*B22
    cnt[1] += halfOfMatrix * halfOfMatrix;

    divideConqMultiply(A21, B11, temp1, halfOfMatrix,cnt);
    divideConqMultiply(A22, B21, temp2, halfOfMatrix,cnt);
    addMatrices(temp1, temp2, C21, halfOfMatrix); // C21 = A21*B11 + A22*B21
    cnt[1] += halfOfMatrix * halfOfMatrix;

    divideConqMultiply(A21, B12, temp1, halfOfMatrix,cnt);
    divideConqMultiply(A22, B22, temp2, halfOfMatrix,cnt);
    addMatrices(temp1, temp2, C22, halfOfMatrix); // C22 = A21*B12 + A22*B22
    cnt[1] += halfOfMatrix * halfOfMatrix;


       if (n > 2) { 
        printf("Partial Matrix C11 for n=%d:\n", n);
        printMatrix(C11, halfOfMatrix);
        puts("");
        printf("Partial Matrix C12 for n=%d:\n", n);
        printMatrix(C12, halfOfMatrix);
        puts("");
        printf("Partial Matrix C21 for n=%d:\n", n);
        printMatrix(C21, halfOfMatrix);
        puts("");
        printf("Partial Matrix C22 for n=%d:\n", n);
        printMatrix(C22, halfOfMatrix);
        puts("");
    }

    // Combine partial matrices into C
    combine(C, C11, C12, C21, C22, halfOfMatrix);

    // Deallocation of temporary arrays
    freeMatrix(A11, halfOfMatrix);
    freeMatrix(A12, halfOfMatrix);
    freeMatrix(A21, halfOfMatrix);
    freeMatrix(A22, halfOfMatrix);
    freeMatrix(B11, halfOfMatrix);
    freeMatrix(B12, halfOfMatrix);
    freeMatrix(B21, halfOfMatrix);
    freeMatrix(B22, halfOfMatrix);
    freeMatrix(temp1, halfOfMatrix);
    freeMatrix(temp2, halfOfMatrix);
}

void strassenMultiply(int **A, int **B, int **C, int n, int *cnt) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        cnt[0]++; // multiplication count
        return;
    }
    
    int half = n/2;

    // Submatrices
    int **A11 = allocateMatrix(half);
    int **A12 = allocateMatrix(half),
        **A21 = allocateMatrix(half), 
        **A22 = allocateMatrix(half),
        **B11 = allocateMatrix(half), 
        **B12 = allocateMatrix(half),
        **B21 = allocateMatrix(half), 
        **B22 = allocateMatrix(half),
        **C11 = allocateMatrix(half), 
        **C12 = allocateMatrix(half),
        **C21 = allocateMatrix(half), 
        **C22 = allocateMatrix(half),
        **P1 = allocateMatrix(half), 
        **P2 = allocateMatrix(half),
        **P3 = allocateMatrix(half), 
        **P4 = allocateMatrix(half),
        **P5 = allocateMatrix(half), 
        **P6 = allocateMatrix(half),
        **P7 = allocateMatrix(half), 
        **tempA = allocateMatrix(half),
        **tempB = allocateMatrix(half);

    // Submatrices
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            int firstHalf = i + half, secondHalf = j + half;
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][secondHalf];
            A21[i][j] = A[firstHalf][j];
            A22[i][j] = A[firstHalf][secondHalf];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][secondHalf];
            B21[i][j] = B[firstHalf][j];
            B22[i][j] = B[firstHalf][secondHalf];
        }
    }

    // Strassen's 7 multiplications (P1 ~ P7)
    addMatricesStr(A11, A22, tempA, half, cnt); 
    addMatricesStr(B11, B22, tempB, half, cnt); 
    strassenMultiply(tempA, tempB, P1, half, cnt); // P1 = (A11+A22) * (B11+B22)


    addMatricesStr(A21, A22, tempA, half, cnt); 
    strassenMultiply(tempA, B11, P2, half, cnt); // P2 = (A21+A22) * B11

    subtractMatricesStr(B12, B22, tempB, half, cnt); 
    strassenMultiply(A11, tempB, P3, half, cnt); // P3 = A11 * (B12-B22)

    subtractMatricesStr(B21, B11, tempB, half, cnt); 
    strassenMultiply(A22, tempB, P4, half, cnt); // P4 = A22 * (B21-B11)

    addMatricesStr(A11, A12, tempA, half, cnt); 
    strassenMultiply(tempA, B22, P5, half, cnt); // P5 = (A11+A12) * B22

    subtractMatricesStr(A21, A11, tempA, half, cnt); 
    addMatricesStr(B11, B12, tempB, half, cnt); 
    strassenMultiply(tempA, tempB, P6, half, cnt); // P6 = (A21-A11) * (B11+B12)

    subtractMatricesStr(A12, A22, tempA, half, cnt); 
    addMatricesStr(B21, B22, tempB, half, cnt); 
    strassenMultiply(tempA, tempB, P7, half, cnt); // P7 = (A12-A22) * (B21+B22)

    
    // P1 + P4 - P5 + P7 -> C11
    addMatricesStr(P1, P4, C11, half, cnt); 
    subtractMatricesStr(C11, P5, C11, half, cnt); 
    addMatricesStr(C11, P7, C11, half, cnt); 

    // P3 + P5 -> C12
    addMatricesStr(P3, P5, C12, half, cnt);

    // P2 + P4 -> C21
    addMatricesStr(P2, P4, C21, half, cnt); 

    // P1 - P2 + P3 + P6 -> C22
    subtractMatricesStr(P1, P2, C22, half, cnt); 
    addMatricesStr(C22, P3, C22, half, cnt); 
    addMatricesStr(C22, P6, C22, half, cnt); 


    // Combine partial matrices into C
    combine(C, C11, C12, C21, C22, half);

    freeMatrix(A11, half); freeMatrix(A12, half); freeMatrix(A21, half); freeMatrix(A22, half);
    freeMatrix(B11, half); freeMatrix(B12, half); freeMatrix(B21, half); freeMatrix(B22, half);
    freeMatrix(C11, half); freeMatrix(C12, half); freeMatrix(C21, half); freeMatrix(C22, half);
    freeMatrix(P1, half); freeMatrix(P2, half); freeMatrix(P3, half); freeMatrix(P4, half);
    freeMatrix(P5, half); freeMatrix(P6, half); freeMatrix(P7, half);
    freeMatrix(tempA, half); freeMatrix(tempB, half); 
    
    
}

int main() {
    srand(time(NULL));
    int n = 4; // Size of the matrix

    int **A = allocateMatrix(n);
    int **B = allocateMatrix(n);
    int **C1 = allocateMatrix(n); // Divide and conquer result
    int **C2 = allocateMatrix(n); // Strassen's algorithm result

    fillRandom(A, n);
    fillRandom(B, n);

    printf("Matrix A:\n");
    printMatrix(A, n);
    printf("\nMatrix B:\n");
    printMatrix(B, n);
    printf("\n");

    // Operation counters ---> 0 for multiplications, 1 for additions, 2 for subtractions
    int cntDivideConquer[3] = {0};
    int cntStrassen[3] = {0};

    // Divide and conquer algorithm
    divideConqMultiply(A, B, C1, n, cntDivideConquer);

    // Strassen's algorithm
    strassenMultiply(A, B, C2, n, cntStrassen);

    printf("----> Resulting Matrix C (Divide-and-Conquer):\n");
    printMatrix(C1, n);
    printf("\n----> Resulting Matrix C (Strassen's Algorithm):\n");
    printMatrix(C2, n);

    printf("\n>Divide-and-Conquer computations: Multiplications = %d, Additions = %d, Subtractions = %d\n",cntDivideConquer[0], cntDivideConquer[1], cntDivideConquer[2]);
    printf("\n>Strassen's Algorithm computations: Multiplications = %d, Additions = %d, Subtractions = %d\n",cntStrassen[0], cntStrassen[1], cntStrassen[2]);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C1, n);
    freeMatrix(C2, n);

    return 0;
}


