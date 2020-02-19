#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int main(void) {
    char* path = "test.dat";
    Matrix* matrix = create_matrix_from_file(path);
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            printf("%lf ", matrix->element[i][j]);
        } printf("\n");
    }
    Matrix* m_matrix = create_matrix(1, 1);
    size_t rows_cols = 0;
    get_rows(matrix, &rows_cols);
    get_cols(m_matrix, &rows_cols);
    double matrix_elem = 0;
    get_elem(matrix, rows_cols, rows_cols, &matrix_elem);
    set_elem(matrix, rows_cols, rows_cols, matrix_elem);
    printf("%lf\n", matrix_elem);
    Matrix* mul = mul_scalar(matrix, matrix_elem);
    transp(mul);
    Matrix* sum_matrix = sum(matrix, mul);
    sum_matrix = sub(sum_matrix, matrix);
    det(matrix, &matrix_elem);
    sum_matrix = adj(matrix);
    matrix = inv(sum_matrix);
    free_matrix(matrix);
    free_matrix(m_matrix);
    free_matrix(mul);
    free_matrix(sum_matrix);
    return 0;
}
