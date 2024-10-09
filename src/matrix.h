#pragma once
#include <vector>
#include <iostream>

// Пользовательский тип (псевдоним)
typedef double real;

// Класс матриц
class Matrix
{
private:
    // Количество столбцов матрицы
    int cols_;
    // Количество строк матрицы
    int rows_;
    // Массив элементов матрицы
    std::vector<real> mvec_;
public:
    // Конструктор по умолчанию
    Matrix(){};
    // Основной конструктор
    Matrix(int rows, int cols) : cols_(cols), rows_(rows), mvec_(std::vector<real>(cols*rows,0)) {};
    // Основной конструктор с исходной матрицей
    Matrix(int rows, int cols, std::vector<real> mvec) : cols_(cols), rows_(rows), mvec_(mvec) {};
    // Оператор индексирования
    real& operator()(int row, int col);
    // Оператор индексирования с гарантией неизменности данных
    real operator()(int row, int col) const;        
    // Выводит матрицу на экран (стандартный вывод)
    void print();
    // Оператор сложения матриц
    friend Matrix operator+(const Matrix& A, const Matrix& B);
    // Оператор вычитания матриц
    friend Matrix operator-(const Matrix& A, const Matrix& B);
    // Оператор умножения матриц
    friend Matrix operator*(const Matrix& A, const Matrix& B);
    // Оператор сложения матриц с присваиванием
    Matrix& operator+=(const Matrix& A);
    // Оператор вычитания матриц с присваиванием
    Matrix& operator-=(const Matrix& A);
    // Оператор умножения матриц с присваиванием
    Matrix& operator*=(const Matrix& A);
    // Оператор вывода объекта в поток
    friend std::ostream& operator<<(std::ostream &os, const Matrix &other);
    // Оператор ввода объекта из потока
    friend std::istream& operator>>(std::istream &is, Matrix &other);        
};

// Оператор сложения матриц
Matrix operator+(const Matrix& A, const Matrix& B);
// Оператор вычитания матриц
Matrix operator-(const Matrix& A, const Matrix& B);
// Оператор умножения матриц
Matrix operator*(const Matrix& A, const Matrix& B);
// Оператор вывода объекта в поток
std::ostream& operator<<(std::ostream &os, const Matrix &other);
// Оператор ввода объекта из потока
std::istream& operator>>(std::istream &is, Matrix &other);
