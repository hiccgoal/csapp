/* 
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
    // 只做了M=N=32这种情况
    // 第一种想法是对32*32矩阵分块转置，因为cache大小32组，每组32bytes，因此每个
    // cache一组能放8个矩阵元素，放一行矩阵元素就需要4组，因此cache最多只能存8行矩阵元素，故分块大小8*8
    // for (int i = 0; i < M; i += 8)
    // {
    //     for (int j = 0; j < N; j += 8)
    //     {
    //         for (int m = i; m < i + 8; m++)
    //         {
    //             for (int n = j; n < j + 8; j++)
    //             {
    //                 B[n][m] = A[m][n];
    //             }
    //         }
    //     }
    // }
    // https://www.cnblogs.com/xvic/p/16037046.html#32--32
    // https://zhuanlan.zhihu.com/p/79058089
    // https://zhuanlan.zhihu.com/p/342923482
    // 第一种想法对角线上的块会冲突，此时就会发生原地转置。譬如把 A 的第四行存进去，但要写 B44 时，发生了冲突，第四行被换成 B 的，然后读 A 的时候又换成了 A 的，就多造成了两次 miss。
    // 对对角线上的每个组，是读写读
    // 第二种想法是既然如此，以空间换时间，把一行一次性读完，减少冲突不命中。此时第四行被换成 B ，然后读 A 的时候直接用变量中存储的值
    // 对对角线上的每个组，是读写
    for (int i = 0; i < M; i += 8)
    {
        for (int j = 0; j < N; j += 8)
        {
            for (int k = i; k < (i + 8); k++)
            {
                int a_1 = A[k][j];
                int a_2 = A[k][j+1];
                int a_3 = A[k][j+2];
                int a_4 = A[k][j+3];
                int a_5 = A[k][j+4];
                int a_6 = A[k][j+5];
                int a_7 = A[k][j+6];
                int a_8 = A[k][j+7];
                B[j][k] = a_1;
                B[j+1][k] = a_2;
                B[j+2][k] = a_3;
                B[j+3][k] = a_4;
                B[j+4][k] = a_5;
                B[j+5][k] = a_6;
                B[j+6][k] = a_7;
                B[j+7][k] = a_8;
            }
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

