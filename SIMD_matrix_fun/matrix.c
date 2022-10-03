#include "matrix.h"
#include <stdbool.h>
#include <pthread.h>
#include <immintrin.h> // For AVX.

matrix_t matrix_create(int rows, int cols) { 
    double* elts = malloc(rows * cols * sizeof(double));
    matrix_t m = {rows, cols, elts};
    return m;
}

double matrix_get(matrix_t m, int r, int c) { 
    assert(r < m.rows && c < m.cols);
    return m.elts[r * m.cols + c];
}

void matrix_set(matrix_t m, int r, int c, double d) { 
    assert(r < m.rows && c < m.cols);
    m.elts[r * m.cols + c] = d;
}


void matrix_free(matrix_t m) { 
    if (m.elts != NULL)
    {
        free(m.elts);
    }
}

matrix_t matrix_multiply(matrix_t m1, matrix_t m2) { 
    assert(m1.cols == m2.rows);

    matrix_t m = matrix_create(m1.rows, m2.cols);
    
    for (int r1 = 0; r1 < m1.rows; ++r1)
    {
        for (int c2 = 0; c2 < m2.cols; ++c2)
        {
            double s = 0;
            
            for (int i = 0; i < m1.cols; ++i)
            {
                s += m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2];     
            }

            m.elts[r1 * m.cols + c2] = s;
        }
    }

    return m;
}

matrix_t matrix_transpose(matrix_t m) { 
    matrix_t t = matrix_create(m.cols, m.rows);

    for (int r = 0; r < m.rows; ++r)
    {
        for (int c = 0; c < m.cols; ++c)
        {
            t.elts[c * t.cols + r] = m.elts[r * m.cols + c];
        }
    }

    return t;
}

matrix_t matrix_multiply_transposed(matrix_t m1, matrix_t m2) { 
    assert(m1.cols == m2.cols);

    matrix_t m = matrix_create(m1.rows, m2.rows);

    for (int r1 = 0; r1 < m1.rows; ++r1)
    {
        for (int r2 = 0; r2 < m2.rows; ++r2)
        {
            double s = 0;
            
            for (int i = 0; i < m1.cols; ++i)
            {
                s += m1.elts[r1 * m1.cols + i] * m2.elts[r2 * m2.cols + i];     
            }

            m.elts[r1 * m.cols + r2] = s;
        }
    }

    return m;
}

matrix_t matrix_multiply_fast(matrix_t m1, matrix_t m2) { 
    matrix_t t2 = matrix_transpose(m2);
    matrix_t result = matrix_multiply_transposed(m1, t2);
    matrix_free(t2);
    return result; 
}

struct matrix_thread_args
{
    matrix_t mat1;
    matrix_t mat2;
    matrix_t output;
    int tile;
    int tid;
};

void* matrix_fast_fast_thread(void* input)
{
    matrix_t m1 = ((struct matrix_thread_args*)input)->mat1;
    matrix_t m2 = ((struct matrix_thread_args*)input)->mat2;
    matrix_t m = ((struct matrix_thread_args*)input)->output;
    int tile = ((struct matrix_thread_args*)input)->tile;
    int tid = ((struct matrix_thread_args*)input)->tid;
    
    for (int tr1 = tid*tile; tr1 < m1.rows; tr1+=(tile*8))
    {
        for (int tc2 = 0; tc2 < m2.cols; tc2+=tile)
        {
            for (int ti = 0; ti < m1.cols; ti+=tile)
            {
                // Inner loop similar to ordinary matrix multiply.
                for (int r1 = tr1; r1 < MIN(tr1+tile, m1.rows); ++r1)
                {
                    int c2_len = MIN(tc2+tile, m2.cols);
                    for (int c2 = tc2; c2 < c2_len-3 ; c2+=4)
                    {
                        int i_len = MIN(ti+tile, m1.cols);
                        for (int i = ti; i < i_len-3; i+=4)
                        {
                            __m256d ymm0 =
                                _mm256_broadcast_sd(m1.elts + r1 * m1.cols + i + 0);
                            __m256d ymm1 =
                                _mm256_broadcast_sd(m1.elts + r1 * m1.cols + i + 1);
                            __m256d ymm2 =
                                _mm256_broadcast_sd(m1.elts + r1 * m1.cols + i + 2);
                            __m256d ymm3 =
                                _mm256_broadcast_sd(m1.elts + r1 * m1.cols + i + 3);

                            __m256d ymm4 =
                                _mm256_loadu_pd(m2.elts + (i+0) * m2.cols + c2);
                            __m256d ymm5 =
                                _mm256_loadu_pd(m2.elts + (i+1) * m2.cols + c2);
                            __m256d ymm6 =
                                _mm256_loadu_pd(m2.elts + (i+2) * m2.cols + c2);
                            __m256d ymm7 =
                                _mm256_loadu_pd(m2.elts + (i+3) * m2.cols + c2);

                            ymm0 = _mm256_mul_pd(ymm0, ymm4);
                            ymm1 = _mm256_mul_pd(ymm1, ymm5);
                            ymm0 = _mm256_add_pd(ymm0, ymm1);

                            ymm2 = _mm256_mul_pd(ymm2, ymm6);
                            ymm3 = _mm256_mul_pd(ymm3, ymm7);
                            ymm2 = _mm256_add_pd(ymm2, ymm3);

                            ymm0 = _mm256_add_pd(ymm0, ymm2);

                            // Store the result to the output matrix.
                            __m256d ymmx =
                                _mm256_loadu_pd(m.elts + r1 * m.cols + c2);

                            ymmx = _mm256_add_pd(ymmx, ymm0);

                            _mm256_storeu_pd(m.elts + r1 * m.cols + c2, ymmx);
                        }
                        // For i offsets that are not multiples of 4 and not used
                        // with AVX.
                        for (int i = i_len-(i_len % 4); i < i_len; ++i)
                        {
                             m.elts[r1 * m.cols + c2 + 0] += 
                                m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2 + 0];     

                             m.elts[r1 * m.cols + c2 + 1] += 
                                m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2 + 1];     

                             m.elts[r1 * m.cols + c2 + 2] += 
                                m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2 + 2];     

                             m.elts[r1 * m.cols + c2 + 3] += 
                                m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2 + 3];     
                        }
                    }
                    // For column offsets that are not multiple of 4 and not used
                    // with AVX.
                    for (int c2 = c2_len-(c2_len % 4); c2 < c2_len; ++c2)
                    {
                        for (int i = ti; i < MIN(ti+tile, m1.cols); ++i)
                        {
                             m.elts[r1 * m.cols + c2] += 
                                m1.elts[r1 * m1.cols + i] * m2.elts[i * m2.cols + c2];     
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

// CPU matrix multiplication ... as fast as possible.
// Optimisations include: Multithreaded to utilise all cores/hyperthreads,
// Minimising cache misses by employing loop tiling, where each tile = 16,
// AVX2 for operating on 4 doubles per instruction.
matrix_t matrix_multiply_fast_fast(matrix_t m1, matrix_t m2)
{
    assert(m1.cols == m2.rows);

    // Creating the output matrix.
    // Not using matrix_create as we need to use calloc, not malloc.
    double* elts = calloc(m1.rows * m2.cols, sizeof(double));
    matrix_t m = {m1.rows, m2.cols, elts};

    int tile = 16;

    // Using 8 worker threads.
    pthread_t tid_arr[8];
    struct matrix_thread_args args_arr[8];

    for (int i = 0; i < 8; ++i)
    {
        args_arr[i].mat1 = m1;
        args_arr[i].mat2 = m2;
        args_arr[i].output = m;
        args_arr[i].tile = tile;
        args_arr[i].tid = i;

        pthread_create(&tid_arr[i], NULL, matrix_fast_fast_thread, (void*)&args_arr[i]);
    }

    for (int i = 0; i < 8; ++i)
    {
        pthread_join(tid_arr[i], NULL);
    }

    return m;
}

void matrix_print(matrix_t m) 
{ 
    for (int i = 0; i < m.rows; i++) { 
        for (int j = 0; j < m.cols; j++) { 
            printf("%g\t", matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}


