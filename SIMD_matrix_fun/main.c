#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"

void matrix_test(void) { 
  matrix_t m1 = matrix_create(3, 5);
  for (int i = 0; i < m1.rows; i++) { 
    for (int j = 0; j < m1.cols; j++) { 
      matrix_set(m1, i, j, i - j);
    }
  }
  matrix_print(m1);
  matrix_t m2 = matrix_transpose(m1);
  matrix_print(m2);
  matrix_t m3 = matrix_create(5, 2); 
  for (int i = 0; i < m3.rows; i++) { 
    for (int j = 0; j < m3.cols; j++) { 
      matrix_set(m3, i, j, i - j);
    }
  }
  matrix_print(m3);

  matrix_t m4 = matrix_multiply(m1, m3);
  matrix_print(m4);

  matrix_t m5 = matrix_transpose(m3);
  matrix_print(m5);

  matrix_t m6 = matrix_multiply_transposed(m1, m5);
  matrix_print(m6);

  matrix_t m7 = matrix_multiply_fast_fast(m1, m3);
  matrix_print(m7);

  matrix_t m8 = matrix_create(22, 22);
  for (int i = 0; i < m8.rows; i++) { 
    for (int j = 0; j < m8.cols; j++) { 
      matrix_set(m8, i, j, i - j);
    }
  }
  matrix_t m9 = matrix_create(22, 11);
  for (int i = 0; i < m9.rows; i++) { 
    for (int j = 0; j < m9.cols; j++) { 
      matrix_set(m9, i, j, i - j);
    }
  }

  matrix_print(m8);
  matrix_print(m9);

  matrix_t m10 = matrix_multiply(m8, m9);
  matrix_print(m10);

  matrix_t m11 = matrix_multiply_fast_fast(m8, m9);
  matrix_print(m11);

  matrix_free(m1);
  matrix_free(m2);
  matrix_free(m3);
  matrix_free(m4);
  matrix_free(m5);
  matrix_free(m6);
  matrix_free(m7);
  matrix_free(m8);
  matrix_free(m9);
  matrix_free(m10);
  matrix_free(m11);
}
/* Expected result of matrix_test():
0	-1	-2	-3	-4	
1	0	-1	-2	-3	
2	1	0	-1	-2	

0	1	2	
-1	0	1	
-2	-1	0	
-3	-2	-1	
-4	-3	-2	

0	-1	
1	0	
2	1	
3	2	
4	3	

-30	-20	
-20	-15	
-10	-10	

0	1	2	3	4	
-1	0	1	2	3	

-30	-20	
-20	-15	
-10	-10	

-30	-20	
-20	-15	
-10	-10	
*/
 

typedef matrix_t (*multiplier)(matrix_t m1, matrix_t m2); 

void time_it(multiplier f, matrix_t m1, matrix_t m2, char *msg) { 
  struct timespec start, finish;
  double elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  matrix_t m = (*f)(m1, m2); 
  matrix_free(m);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  printf("%s: %f seconds\n", msg, elapsed);
}

int main(void) { 
  matrix_test();

  matrix_t m1 = matrix_create(3000, 3000);
  for (int i = 0; i < m1.rows; i++) { 
    for (int j = 0; j < m1.cols; j++) { 
      matrix_set(m1, i, j, i - j);
    }
  }
  matrix_t m2 = matrix_create(3000, 3000);
  for (int i = 0; i < m2.rows; i++) { 
    for (int j = 0; j < m2.cols; j++) { 
      matrix_set(m2, i, j, i + j);
    }
  }

  printf("3000x3000 by 3000x3000 matrix multiplication test\n");

  time_it(matrix_multiply, m1, m2, "matrix_multiply");
  time_it(matrix_multiply_fast, m1, m2, "matrix_multiply_fast");
  time_it(matrix_multiply_fast_fast, m1, m2, "matrix_multiply_fast_fast");
  time_it(matrix_multiply, m1, m2, "matrix_multiply");
  time_it(matrix_multiply_fast, m1, m2, "matrix_multiply_fast");
  time_it(matrix_multiply_fast_fast, m1, m2, "matrix_multiply_fast_fast");

  matrix_free(m1);
  matrix_free(m2);
  
  return 0;
}
