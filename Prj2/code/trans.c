/* 
 * Name: Dou Yi-ming
 * ID: 519021910366
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // local variables
    int ii, jj, i, j;
    int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    if (M == 32 && N == 32)
    {
        // 8 * 8 blocks
        for(ii = 0;ii < 4;++ii)
            for(jj = 0;jj < 4;++jj)
                for(i = 0;i < 8;++i)
                {
                    for(j = 0;j < 8;++j)
                        if(i != j)
                            B[8 * jj + j][8 * ii + i] = A[8 * ii + i][8 * jj + j];
                    B[8 * jj + i][8 * ii + i] = A[8 * ii + i][8 * jj + i];
                }
    }
    else if (M == 64 && N == 64)
    {
        for(ii = 0;ii < 8;++ii)
            for(jj = 0;jj < 8;++jj)
            {
                for(i = 0;i < 4;++i)
                {
                    tmp0 = A[ii * 8 + i][jj * 8 + 0];
                    tmp1 = A[ii * 8 + i][jj * 8 + 1];
                    tmp2 = A[ii * 8 + i][jj * 8 + 2];
                    tmp3 = A[ii * 8 + i][jj * 8 + 3];
                    tmp4 = A[ii * 8 + i][jj * 8 + 4];
                    tmp5 = A[ii * 8 + i][jj * 8 + 5];
                    tmp6 = A[ii * 8 + i][jj * 8 + 6];
                    tmp7 = A[ii * 8 + i][jj * 8 + 7];

                    B[jj * 8 + 0][ii * 8 + i] = tmp0;
                    B[jj * 8 + 1][ii * 8 + i] = tmp1;
                    B[jj * 8 + 2][ii * 8 + i] = tmp2;
                    B[jj * 8 + 3][ii * 8 + i] = tmp3;

                    B[jj * 8 + 0][ii * 8 + i + 4] = tmp4;
                    B[jj * 8 + 1][ii * 8 + i + 4] = tmp5;
                    B[jj * 8 + 2][ii * 8 + i + 4] = tmp6;
                    B[jj * 8 + 3][ii * 8 + i + 4] = tmp7;
                }
                for(j = 0; j < 4; ++j)
                {
                    tmp0 = B[jj * 8 + j][ii * 8 + 4];
                    tmp1 = B[jj * 8 + j][ii * 8 + 5];
                    tmp2 = B[jj * 8 + j][ii * 8 + 6];
                    tmp3 = B[jj * 8 + j][ii * 8 + 7];

                    tmp4 = A[ii * 8 + 4][jj * 8 + j];
                    tmp5 = A[ii * 8 + 5][jj * 8 + j];
                    tmp6 = A[ii * 8 + 6][jj * 8 + j];
                    tmp7 = A[ii * 8 + 7][jj * 8 + j];

                    B[jj * 8 + j][ii * 8 + 4] = tmp4;
                    B[jj * 8 + j][ii * 8 + 5] = tmp5;
                    B[jj * 8 + j][ii * 8 + 6] = tmp6;
                    B[jj * 8 + j][ii * 8 + 7] = tmp7;

                    tmp4 = A[ii * 8 + 4][jj * 8 + j + 4];
                    tmp5 = A[ii * 8 + 5][jj * 8 + j + 4];
                    tmp6 = A[ii * 8 + 6][jj * 8 + j + 4];
                    tmp7 = A[ii * 8 + 7][jj * 8 + j + 4];

                    B[jj * 8 + j + 4][ii * 8 + 0] = tmp0;
                    B[jj * 8 + j + 4][ii * 8 + 1] = tmp1;
                    B[jj * 8 + j + 4][ii * 8 + 2] = tmp2;
                    B[jj * 8 + j + 4][ii * 8 + 3] = tmp3;
                    B[jj * 8 + j + 4][ii * 8 + 4] = tmp4;
                    B[jj * 8 + j + 4][ii * 8 + 5] = tmp5;
                    B[jj * 8 + j + 4][ii * 8 + 6] = tmp6;
                    B[jj * 8 + j + 4][ii * 8 + 7] = tmp7;
                }
            }
    }
    else if (M == 61 && N == 67)
    {
        for(ii = 0; ii < 5; ++ii)
            for(jj = 0; jj < 4; ++jj)
                for(i = 0; i < 16 && ii * 16 + i < N; ++i)
                    for(j = 0; j < 16 && jj * 16 + j < M; ++j)
                        B[16 * jj + j][16 * ii + i] = A[16 * ii + i][16 * jj + j];
    }
    else
    {
        int tmp;
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++) 
            {
                tmp = A[i][j];
                B[j][i] = tmp;
            }
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

