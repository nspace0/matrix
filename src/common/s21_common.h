#ifndef S21_COMMON_H
#define S21_COMMON_H
#include "../s21_matrix.h"

int negative_size(int rows, int columns);
int ptr_is_null(matrix_t *A);
int equal_size(matrix_t *A, matrix_t *B);
int inf_or_nan(matrix_t *A);

void algebraic_complements(matrix_t *A, matrix_t *result);
void record_determinant_matrix(matrix_t *determinant, matrix_t *A,
                               int minor_row, int minor_col);
#endif
