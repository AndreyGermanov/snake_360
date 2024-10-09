#pragma once
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QSpinBox>
#include "matrix.h"

using namespace std;

/// @brief Класс основного окна приложения
class Window : public QWidget {
Q_OBJECT    
private:    
    // Текущий угол под которым движется змея
    int current_angle = 0;
    // Объект таймера, через который реализуется основной цикл игры
    QTimer *timer;
    // Поверхность игрового поля
    QLabel *surface;
    // Поле ввода значения угла, на которое меняется угол
    // под которым движется змея при управлении
    QSpinBox *step_angle;
    // Изображения
    // Фон
    QPixmap bg_image;
    // Яблоко
    QPixmap apple_image;
    // Фрагмент тела змеи
    QPixmap snake_image;
    // Координаты яблока (x,y)
    pair<int,int> applePos;
    // Координаты всех сегментов змеи.
    // Первый сегмент это голова
    vector<pair<int,int>> snakePos;
    // Признак того, что игра завершена
    bool isGameOver;
    // Метод загрузки изображений из файлов
    void loadImages();
    // Метод проверяет столкновения змеи с другими объектами
    void checkCollision();
    // Метод проверяет столкновение указанной точки со змеей
    bool collideWithSnake(pair<int,int> pos);
    // Метод настройки элементов интерфейса в окне
    void setupUI();
    // Метод запускающий игру
    void initGame();
    // Метод размещает яблоко на поле
    void locateApple();
    // Метод завершающий игру
    void gameOver();
    // Метод увеличивает размер змеи на один сегмент
    void extendBody();
    // Метод меняет направление змеи в зависимости от
    // нажатой клавиши
    void move(int key);
    // Метод вычисляет новые координаты головы змеи после перемещения
    // на указанное растояние под указанным углом
    pair<int,int> moveBy(pair<int,int> pos, int distance, int angle);
    // Метод вычисляет площадь области пересечения двух прямоугольников
    // (голова змеи и какой-либо другой объект)
    int intersection(pair<int,int> &box1, pair<int,int> &box2);    
private slots:
    // Метод обработки события таймера
    void timerEvent();
protected:
    // Метод обработки нажатия клавиши на клавиатуре
    void keyPressEvent(QKeyEvent *e);
    // Метод перерисовки содержимого окна
    void paintEvent(QPaintEvent *e);
    // Метод обработки щелчка мыши в окне
    void mousePressEvent(QMouseEvent *event);
public:
    // Основной конструктор окна
    Window(QWidget *parent = 0);
};

// Функция переводит градусы в радианы
double deg2rad(int deg);