/**
 * Модуль Matrix, в котором определены различные операции
 * с матрицами (сложение, вычитание, умножение)
 */

#include "matrix.h"
#include <iostream>
#include <math.h>

/// @brief Оператор индексации матрицы
/// @param row Строка
/// @param col Столбец
/// @return Ссылка на значение в указанной строке и столбце
real &Matrix::operator()(int row, int col)
{
    if (row >= this->rows_) 
    {
        std::cerr << "Matrix: row number out of bounds" << std::endl;        
    }
    if (col >= this->cols_) 
    {
        std::cerr << "Matrix: col number out of bounds" << std::endl;        
    }
    int pos(0);
    pos = cols_ * row + col;
    return this->mvec_.at(pos);
}

/// @brief Оператор индексации матрицы с гарантией неизменности данных
/// @param row Строка
/// @param col Столбец
/// @return Ссылку на значение в указанной строке и столбце
real Matrix::operator()(int row, int col) const
{
    if (row >= this->rows_) 
    {
        std::cerr << "Matrix: row number out of bounds" << std::endl;        
    }
    if (col >= this->cols_) 
    {
        std::cerr << "Matrix: col number out of bounds" << std::endl;        
    }
    int pos(0);
    pos = cols_ * row + col;
    return this->mvec_.at(pos);
}

/// @brief Функция выводит матрицу в стандартный поток вывода
void Matrix::print()
{
    std::cout << *this;
}

/// @brief Оператор сложения матриц
/// @param A Первая матрица
/// @param B Вторая матрица
/// @return Итоговая матрица
Matrix operator+(const Matrix& A, const Matrix& B)
{
    if ((A.rows_ != B.rows_) || (A.cols_ != B.cols_))
    {
        std::cerr << "Matrix: Matrices can't be added" << std::endl;
        return Matrix(0,0);
    }
    Matrix M = Matrix(A.rows_,A.cols_);
    for (int idx=0;idx<A.mvec_.size();++idx) {
        M.mvec_.at(idx) = A.mvec_.at(idx) + B.mvec_.at(idx);
    }
    return M;
}

/// @brief Оператор вычитания матриц
/// @param A Первая матрица
/// @param B Вторая матрица
/// @return Итоговая матрица
Matrix operator-(const Matrix& A, const Matrix& B)
{
    if ((A.rows_ != B.rows_) || (A.cols_ != B.cols_))
    {
        std::cerr << "Matrix: Matrices can't be added" << std::endl;
        return Matrix(0,0);
    }
    Matrix M = Matrix(A.rows_,A.cols_);
    for (int idx=0;idx<A.mvec_.size();++idx) {
        M.mvec_.at(idx) = A.mvec_.at(idx) - B.mvec_.at(idx);
    }
    return M;
}


/// @brief Оператор умножения матриц
/// @param A Первая матрица
/// @param B Вторая матрица
/// @return Итоговая матрица
Matrix operator*(const Matrix& A, const Matrix& B)
{
    if (A.cols_ != B.rows_) 
    {
        std::cerr << "Matrix: Matrices can't be multiplied" << std::endl;
        return Matrix(0,0);
    }

    Matrix M = Matrix(A.rows_,B.cols_);

    for (int pos = 0;pos<M.mvec_.size();++pos)
    {
        int row = (int)std::floor(pos/M.cols_);
        int col = pos - row*M.cols_;
        for (int k = 0;k<A.cols_;++k)
        {
            M.mvec_.at(pos) += A(row,k) * B(k,col);
        }
    }
    
    return M;
}

/// @brief Оператор вывода матрицы в поток
/// @param os Ссылка на поток вывода
/// @param self Матрица-источник
/// @return Поток вывода
std::ostream& operator<<(std::ostream &os, const Matrix &self)
{
    for (int row=0; row < self.rows_; ++row)
    {
        for (int col=0; col < self.cols_; ++col)
        {
            os << self.mvec_.at(self.cols_*row + col) << " ";
        }
        os << std::endl;
    }
    return os;
}

/// @brief Оператор ввода матрицы из потока
/// @param is Сссылка на поток ввода
/// @param self Матрица-приемник
/// @return Поток ввода
std::istream& operator>>(std::istream &is, Matrix &self)
{
    for (int idx=0;idx<self.rows_*self.cols_;idx++)
    {
        is >> self.mvec_.at(idx);
    }
    return is;
}

/// @brief Оператор сложения матриц с присваиванием
/// @param A Вторая матрица, с которой складывается текущая матрица
/// @return Текущая матрица
Matrix &Matrix::operator+=(const Matrix &A)
{
    *this = *this + A;
    return *this;
}

/// @brief Оператор вычитания матриц с присваиванием
/// @param A Вторая матрица, которая вычитается из текущей матрицы
/// @return Текущая матрица
Matrix &Matrix::operator-=(const Matrix &A)
{
    *this = *this - A;
    return *this;
}

/// @brief Оператор умножения матриц с присваиванием
/// @param A Вторая матрица, на которую умножается текущая матрица
/// @return Текущая матрица
Matrix &Matrix::operator*=(const Matrix &A)
{
    *this = *this * A;
    return *this;
}