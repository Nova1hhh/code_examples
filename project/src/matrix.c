#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix* matrix = malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->element = (double**) calloc(matrix->rows, sizeof(double*));
    if (matrix->element == NULL) {
        free(matrix);
        return NULL;
    }
    for (size_t i = 0; i < matrix->rows; i++) {
        matrix->element[i] = (double*) calloc(matrix->cols, sizeof(double));
        if (matrix->element[i] == NULL) {
            free_matrix(matrix);
            return NULL;
        }
    }
    return matrix;
}

Matrix* create_matrix_from_file(const char* path_file) {
    if (path_file == NULL) {
        return NULL;
    }
    FILE* file_contains_matrix = fopen(path_file, "r");
    if (file_contains_matrix == NULL) {
        return NULL;
    }
    size_t m_rows = 0;
    size_t m_cols = 0;
    if (fscanf(file_contains_matrix, "%zu %zu", &m_rows, &m_cols) != 2) {
        fclose(file_contains_matrix);
        return NULL;
    }
    Matrix* matrix = create_matrix(m_rows, m_cols);
    if (matrix == NULL) {
        fclose(file_contains_matrix);
        return NULL;
    }
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            if (fscanf(file_contains_matrix, "%lf", &matrix->element[i][j]) != 1) {
                free_matrix(matrix);
                fclose(file_contains_matrix);
                return NULL;
            }
        }
    }
    fclose(file_contains_matrix);
    return matrix;
}

int free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return 0;
    }
    for (size_t i = 0; i < matrix->rows; i++) {
        free(matrix->element[i]);
    }
    free(matrix->element);
    free(matrix);
    return 0;
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (rows == NULL) {
        return UNCORRECT_POINTER;
    }
    if (matrix == NULL) {
        return MATRIX_EXIST_ERROR;
    }
    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (cols == NULL) {
        return UNCORRECT_POINTER;
    }
    if (matrix == NULL) {
        return MATRIX_EXIST_ERROR;
    }
    *cols = matrix->cols;
    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (val == NULL) {
        return UNCORRECT_POINTER;
    }
    if (matrix == NULL) {
        return MATRIX_EXIST_ERROR;
    }
    if (row >= matrix->rows || col >= matrix->cols) {
        return UNCORRECT_ROW_OR_COL_NUM;
    }
    *val = matrix->element[row][col];
    return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL) {
        return MATRIX_EXIST_ERROR;
    }
    if (row >= matrix->rows || col >= matrix->cols) {
        return UNCORRECT_ROW_OR_COL_NUM;
    }
    matrix->element[row][col] = val;
    return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* new_matrix = create_matrix(matrix->rows, matrix->cols);
    if (new_matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < new_matrix->rows; i++) {
        for (size_t j = 0; j < new_matrix->cols; j++) {
            new_matrix->element[i][j] = matrix->element[i][j] * val;
        }
    }
    return new_matrix;
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* new_matrix = create_matrix(matrix->cols, matrix->rows);
    if (new_matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            new_matrix->element[j][i] = matrix->element[i][j];
        }
    }
    return new_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    if (l->rows != r->rows) {
        return NULL;
    }
    if (l->cols != r->cols) {
        return NULL;
    }
    Matrix* sum_matrix = create_matrix(l->rows, l->cols);
    if (sum_matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < sum_matrix->rows; i++) {
        for (size_t j = 0; j < sum_matrix->cols; j++) {
            sum_matrix->element[i][j] = l->element[i][j] + r->element[i][j];
        }
    }
    return sum_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    if (l->rows != r->rows) {
        return NULL;
    }
    Matrix* sub_matrix = create_matrix(l->rows, l->cols);
    if (sub_matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < sub_matrix->rows; i++) {
        for (size_t j = 0; j < sub_matrix->cols; j++) {
            sub_matrix->element[i][j] = l->element[i][j] - r->element[i][j];
        }
    }
    return sub_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    if (l->cols != r->rows) {
        return NULL;
    }
    Matrix* mul_matrix = create_matrix(l->rows, r->cols);
    if (mul_matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < mul_matrix->rows; i++) {
        for (size_t j = 0; j < mul_matrix->cols; j++) {
            mul_matrix->element[i][j] = 0;
            for (size_t k = 0; k < l->cols; k++) {
                mul_matrix->element[i][j] += l->element[i][k] * r->element[k][j];
            }
        }
    }
    return mul_matrix;
}

int det(const Matrix* matrix, double* val) {
    if (matrix == NULL) {
        return UNCORRECT_POINTER;
    }
    if (val == NULL) {
        return UNCORRECT_POINTER;
    }
    *val = 0;
    free_matrix((Matrix*) matrix);
    return 0;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }
    free_matrix((Matrix*) matrix);
    return NULL;
}

Matrix* inv(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }
    free_matrix((Matrix*) matrix);
    return NULL;
}
