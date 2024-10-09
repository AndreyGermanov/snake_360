/**
 * Модуль, в котором определены функции для получения
 * значений с помощью генератора случайных чисел
 */

#include <random>
#include "random.h"
using namespace std;

/// @brief Функция возвращает произвольное число из указанного диапазона
/// @param min Нижняя граница диапазона
/// @param max Верхняя граница диапазона
/// @return Число
int rand(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

/// @brief Возвращает произвольную координату (x,y) в указанных пределах
/// @param maxX Верхний предел по X
/// @param maxY Верхний предел по Y
/// @return Пара (x,y)
pair<int,int> getRandPos(int maxX, int maxY) { 
    return {rand(0,maxX),rand(0,maxY)}; 
}