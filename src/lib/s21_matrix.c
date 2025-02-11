#include "../s21_matrix.h"

#include "../common/s21_common.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int flag = OK;
  if ((!result) || (rows < 1) || (columns < 1))
    flag = INCORRECT_MATRIX;
  else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)malloc(sizeof(double *) * rows);
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
    }
  }

  return flag;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && !negative_size(A->rows, A->columns)) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i] != NULL) {
        free(A->matrix[i]);
      }
    }
    free(A->matrix);
  }
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;

  if (ptr_is_null(A) || ptr_is_null(B) || equal_size(A, B)) status = FAILURE;

  for (int i = 0; i < A->rows && status == SUCCESS; i++) {
    for (int j = 0; j < B->columns && status == SUCCESS; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
        status = FAILURE;
        break;
      }
    }
  }

  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;

  if (ptr_is_null(A) || ptr_is_null(B) || ptr_is_null(result))
    status = INCORRECT_MATRIX;
  else if (equal_size(A, B) || inf_or_nan(A) || inf_or_nan(B))
    status = CALCULATION_ERROR;

  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;

  if (ptr_is_null(A) || ptr_is_null(B) || result == NULL) {
    status = INCORRECT_MATRIX;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    status = CALCULATION_ERROR;
  } else {
    if (s21_create_matrix(A->rows, A->columns, result) != OK) {
      status = INCORRECT_MATRIX;
    } else {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
    }
  }

  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = OK;

  if (ptr_is_null(A) || ptr_is_null(result))
    status = INCORRECT_MATRIX;
  else if (inf_or_nan(A) || isinf(number) || isnan(number))
    status = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = OK;
  if (ptr_is_null(A) || ptr_is_null(result)) {
    code = INCORRECT_MATRIX;
  } else if (inf_or_nan(A) || inf_or_nan(B) || equal_size(A, B)) {
    code = CALCULATION_ERROR;
  } else {
    int a = (A->rows > A->columns) ? A->rows : A->columns;
    int s = (A->rows < A->columns) ? A->rows : A->columns;
    s21_create_matrix(a, a, result);
    for (int i = 0; i < a; i++) {
      for (int j = 0; j < a; j++) {
        for (int k = 0; k < s; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (ptr_is_null(A) || ptr_is_null(result)) {
    status = INCORRECT_MATRIX;
  } else if (inf_or_nan(A)) {
    status = CALCULATION_ERROR;
  } else {
    if (s21_create_matrix(A->columns, A->rows, result) != OK) {
      status = INCORRECT_MATRIX;
    } else {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  }

  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (ptr_is_null(A) || ptr_is_null(result)) {
    return INCORRECT_MATRIX;
  } else if (A->rows != A->columns || inf_or_nan(A)) {
    return CALCULATION_ERROR;
  }

  s21_create_matrix(A->rows, A->columns, result);

  matrix_t determinant;

  if (A->rows > 1) {
    s21_create_matrix(A->rows - 1, A->columns - 1, &determinant);
  } else {
    result->matrix[0][0] = 1;
  }

  for (int minor_row = 0; minor_row < A->rows && A->rows > 1; minor_row++) {
    for (int minor_col = 0; minor_col < A->columns; minor_col++) {
      record_determinant_matrix(&determinant, A, minor_row, minor_col);
      s21_determinant(&determinant, &result->matrix[minor_row][minor_col]);
    }
  }

  if (A->rows > 1) {
    s21_remove_matrix(&determinant);
    algebraic_complements(A, result);
  }

  return OK;
}

int s21_determinant(matrix_t *A, double *result) {
  if (ptr_is_null(A) || result == NULL)
    return INCORRECT_MATRIX;
  else if (A->rows != A->columns || inf_or_nan(A))
    return CALCULATION_ERROR;

  matrix_t copy;
  s21_create_matrix(A->rows, A->columns, &copy);

  for (int i = 0; i < copy.rows; i++) {
    for (int j = 0; j < copy.columns; j++) {
      copy.matrix[i][j] = A->matrix[i][j];
    }
  }

  int not_zero = 0, all_zero = 1, sign = 1;
  double *tmp = NULL;
  *result = 1;

  for (int rows = 0; rows < copy.rows - 1 && *result; rows++) {
    for (not_zero = rows, all_zero = 1; not_zero < copy.rows; not_zero++) {
      if (copy.matrix[not_zero][rows]) {
        tmp = copy.matrix[not_zero];
        all_zero = 0;
        break;
      }
    }

    if (copy.matrix[rows][rows] == 0. && tmp) {
      copy.matrix[not_zero] = copy.matrix[rows];
      copy.matrix[rows] = tmp;
      sign = -sign;
    }

    if (all_zero) {
      *result = 0.;
    } else {
      for (int m = rows + 1; m < copy.rows; m++) {
        double tmp = copy.matrix[m][rows] / copy.matrix[rows][rows];

        for (int n = rows; n < copy.columns; n++) {
          copy.matrix[m][n] -= copy.matrix[rows][n] * tmp;
        }
      }
    }
  }

  for (int k = 0; k < A->rows; k++) {
    *result *= copy.matrix[k][k];
  }

  *result *= sign;
  s21_remove_matrix(&copy);

  return OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (ptr_is_null(A) || ptr_is_null(result)) {
    status = INCORRECT_MATRIX;
  } else if (A->rows != A->columns || inf_or_nan(A)) {
    status = CALCULATION_ERROR;
  } else {
    double determinant = 0.0;

    if (s21_determinant(A, &determinant) == OK && determinant != 0) {
      matrix_t complements = {0}, transpose = {0};

      if (s21_calc_complements(A, &complements) == OK) {
        if (s21_transpose(&complements, &transpose) == OK) {
          determinant = 1.0 / determinant;
          s21_mult_number(&transpose, determinant, result);

        } else {
          status = CALCULATION_ERROR;
        }

        s21_remove_matrix(&transpose);
      } else {
        status = CALCULATION_ERROR;
      }

      s21_remove_matrix(&complements);
    } else {
      status = CALCULATION_ERROR;
    }
  }

  return status;
}
