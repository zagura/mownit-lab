//
// Created by misiek on 19.03.16.
//

#ifndef MOWNIT_MZ_AXB_H
#define MOWNIT_MZ_AXB_H
#include <iostream>
#include <vector>
#include <utility>
namespace mz {
    struct Matrix {
        Matrix(void);

        Matrix(int n);

        Matrix(int r, int c);

        Matrix(const Matrix &M);

        void print();

        std::vector <std::vector<double>> v;
        int rows;
        int columns;
    };

    struct pivot {
        Matrix *M;
        int row;
        int column;
        double val;
    };
    struct LU_dec {
        LU_dec(const Matrix &L, const Matrix &U, std::vector<double> b);

        Matrix L;
        Matrix U;
        std::vector<double> b;
    };

    pivot find_pivot(Matrix *M, int row_begin, int col_begin);

    pivot find_partial_pivot(Matrix *M, int row_begin, int column);

    void swap_columns(Matrix &M, int first, int second, std::vector<double *> &variables);

    void swap_rows(Matrix &M, int first, int second, std::vector<double> &results);

    void reduce(std::vector<double> reducing_row, std::vector<double> &reduced_row, pivot p, double res, double &result);

    void solve_triangle_up(Matrix &M, std::vector<double *> &variables, std::vector<double> &results);

    void solve_triangle_down(Matrix &M, std::vector<double *> &variables, std::vector<double> &results);

    void solve(Matrix &M, std::vector<double *> &variables, std::vector<double> &results);

    Matrix multiply(Matrix &left, Matrix &right);

    std::vector<double> multiply(Matrix &left, std::vector<double> vec);

    Matrix get_transpose(const Matrix &a);

    LU_dec *LU_decomposition(Matrix &A, std::vector<double *> &x, std::vector<double> b);

    void solve_lu_method(Matrix &A, std::vector<double *> &x, std::vector<double> &b);

    void solve_normalized_LU(Matrix &A, std::vector<double *> &x, std::vector<double> &b);

    void multiple_row(std::vector<double> &row_v, std::vector<double> &b, int row, pivot p);

    void scale(Matrix &M, std::vector<double> b, pivot p);
}
#endif //MOWNIT_MZ_AXB_H
