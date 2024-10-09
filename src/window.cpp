/**
 * Модуль главного окна приложения
 */

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QTransform>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QDir>
#include <tuple>
#include "random.h"
#include "window.h"
#include "math.h"

// Ширина одного элемента поля
#define ROW_HEIGHT 13
// Длина одного элемента поля
#define COL_WIDTH 13

// Частота срабатывания таймера (мс)
#define TIMER_INTERVAL 300

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent)
{    
    // Изначальный размер окна
    this->resize(520, 520);
    // Создаем объект для таймера
    this->timer = new QTimer(this);
    // и привязываем обработчик таймера "timerEvent"
    // в нем происходит основной цикл игры: перемещение змеи,
    // проверка коллизий и перерисовка поля
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    // Загрузка изображений
    this->loadImages();
    // Настройка элементов управления в окне
    this->setupUI();
    // Старт игры
    this->initGame();
}

/// @brief Настройка элементов управления окна
void Window::setupUI()
{
    // Форма ввода угла наклона
    QFormLayout *form = new QFormLayout();
    this->step_angle = new QSpinBox();
    // По умолчанию, угол будет меняться на 30 градусов
    this->step_angle->setValue(30);
    // Угол должен быть в этом диапазоне
    this->step_angle->setRange(0,360);
    form->addRow("&Угол поворота:", this->step_angle);
    // Layout для окна
    QVBoxLayout *vbox = new QVBoxLayout();
    // Добавляем пустую метку поверх игрового поля
    this->surface = new QLabel();
    vbox->addWidget(this->surface, Qt::AlignJustify);
    // Добавляем форму ввода угла наклона под игровое поле
    vbox->addLayout(form);
    // Устанавливаем Layout окна
    this->setLayout(vbox);
    // Убираем фокус с поля ввода угла наклона
    this->step_angle->clearFocus();
    this->surface->setFocus();
}
/// @brief Загружает все необходимые изображения
void Window::loadImages()
{
    // трава
    this->bg_image = QPixmap(qApp->applicationDirPath()+"/img/bg.png");
    // яблоко
    this->apple_image = QPixmap(qApp->applicationDirPath()+"/img/apple.png");
    // сегмент тела змеи
    this->snake_image = QPixmap(qApp->applicationDirPath()+"/img/body.png");
}

/// @brief Старт игры
void Window::initGame() {
    // Изначально змея движется горизонтально 
    this->current_angle = 0;
    // Размещаем яблоко
    this->locateApple();
    // Получаем произвольную позицию головы змеи
    pair snakeHeadPos = getRandPos(this->surface->size().width()-COL_WIDTH,this->surface->size().height()-ROW_HEIGHT);
    // Подгоняем позицию под сетку COL_WIDTH x ROW_HEIGHT
    snakeHeadPos.first = (snakeHeadPos.first / COL_WIDTH) * COL_WIDTH;
    snakeHeadPos.second = (snakeHeadPos.second / ROW_HEIGHT) * ROW_HEIGHT;
    // Добавляем голову к змее
    this->snakePos = {snakeHeadPos};
    // Добавляем два сегмента к змее
    this->snakePos.push_back({snakeHeadPos.first-COL_WIDTH,snakeHeadPos.second});
    this->snakePos.push_back({snakeHeadPos.first-COL_WIDTH*2,snakeHeadPos.second});
    // Выходим из режима "Game Over"
    this->isGameOver = false;
    // Запускаем таймер
    this->timer->start(TIMER_INTERVAL);
}

/// @brief Обработчик нажатия клавиши на клавиатуре
/// @param e Событие нажатия клавиши
void Window::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_Left:
        case Qt::Key_Right:
            // Если нажата клавиша управления курсором,
            // то изменяем направление движения змеи
            this->move(e->key());
            break;
        case Qt::Key_Space:
            // Если нажат пробел
            if (this->isGameOver) {                
                // и игра завершена,
                // то запускаем игру заново
                this->initGame();
            }
            break;
        case Qt::Key_Escape:
            // Выход из программы при нажатии Esc
            qApp->quit();
            break;
    }
}

/// @brief Изменяет направление движения змеи
/// @param key Код клавиши управления курсором
void Window::move(int key) {
    switch (key) {
        case Qt::Key_Left:
            // Если влево, то уменьшаем угол на "step_angle" градусов
            this->current_angle -= this->step_angle->value();
            // не допускаем чтбы угол был меньше 0
            if (this->current_angle < 0) {
                this->current_angle = 360 + this->current_angle;
            };
            break;
        case Qt::Key_Right:
            // Если вправо, то увеличиваем угол на "step_angle" градусов
            this->current_angle += this->step_angle->value();
            // не допускаем чтбы угол был больше 360
            if (this->current_angle > 360) {
                this->current_angle = this->current_angle - 360;
            };
            break;
    }
}

/// @brief Обработчик таймера, запускается периодически
void Window::timerEvent()
{   
    // Перемещаем змею в зависимости от текущего угла направления    
    pair<int,int> prevPos = {0,0};
    for (int idx=0;idx<this->snakePos.size();++idx) {
        if (idx == 0) {
            // перемещаем голову
            prevPos = this->snakePos[idx];
            this->snakePos[idx] = this->moveBy(prevPos,COL_WIDTH,this->current_angle);
        } else {
            // остальные сегменты следуют за головой и друг за другом
            auto swap = this->snakePos[idx];
            this->snakePos[idx] = prevPos;
            prevPos = swap;
        }
    }
    // перерисовываем окно
    this->repaint();
    // проверяем коллизии
    this->checkCollision();
}

/// @brief Перемещает позицию головы змеи на указанную дистанцию под указанным углом
/// @param pos Позиция головы змеи (x,y)
/// @param distance Дистанция, на которую нужно переместить голову
/// @param angle Угол в градусах
/// @return Новая позицию (x,y)
pair<int,int> Window::moveBy(pair<int,int> pos, int distance, int angle) {
    
    // Матрица-столбец координат
    Matrix coords_vector = Matrix(3,1, {
        (double)pos.first,
        (double)pos.second,
        1        
    });
    
    // Матрица-столбец перемещения
    auto move_vector = Matrix(3,1, {        
        (double)distance,
        0,
        1
    });

    // Матрица поворота
    auto rotate_matrix = Matrix(3,3,{
        cos(deg2rad(angle)),sin(deg2rad(angle)),0.,
        -sin(deg2rad(angle)),cos(deg2rad(angle)),0.,
        0.,0.,1. 
    });

    // Поворачиваем вектор перемещения и прибавляем к вектору координат
    auto result_matrix = coords_vector + rotate_matrix * move_vector;

    // Возвращаем новые координаты
    return {(int)(result_matrix(0,0)),(int)(result_matrix(1,0))};
}

/// @brief Отображает яблоко на поле
void Window::locateApple() {
    // Получаем произвольную позицию яблока
    this->applePos = getRandPos(this->surface->size().width()-COL_WIDTH,this->surface->size().height()-ROW_HEIGHT);    
    // Подгоняем позицию под сетку COL_WIDTH x ROW_HEIGHT
    this->applePos.first = (this->applePos.first / COL_WIDTH) * COL_WIDTH;
    this->applePos.second = (this->applePos.second / ROW_HEIGHT) * ROW_HEIGHT;
    // Если яблоко слишком близко к краям окна или оказалось под змеей,
    // то вызываем функцию снова
    if (this->applePos.first == 0 || this->applePos.second == 0 || 
        this->applePos.first >= this->surface->size().width()-COL_WIDTH*2 || 
        this->applePos.second >= this->surface->size().height()-ROW_HEIGHT*2 ||
        this->collideWithSnake(this->applePos)) {
            this->locateApple();
    }
}

/// @brief Функция перерисовки содержимого окна. Вызывается каждый раз когда необходимо перерисовать содержимое
/// @param e Событие перерисовки окна
void Window::paintEvent(QPaintEvent *e) {    
    QPainter painter;
    painter.begin(this);
    // Рисуем поле
    for (int y=0;y<this->surface->size().height();y+=ROW_HEIGHT) {
        for (int x=0;x<this->size().width();x+=COL_WIDTH) {
            painter.drawPixmap(x,y, this->bg_image);
        }
    }    
    
    if (!this->isGameOver) {        
        // В режиме когда игра не закончена
        // Рисуем яблоко
        painter.drawPixmap(this->applePos.first, this->applePos.second, this->apple_image);                    
        // Рисуем змею
        for (int idx=0;idx<this->snakePos.size();++idx) {
            auto [x,y] = this->snakePos[idx];
            painter.drawPixmap(x,y,this->snake_image.transformed(QTransform().rotate(this->current_angle)));
                       
        }
    } else {
        // Если игра закончена, то просто пишем "GAME OVER"

        // Устанавливаем шрифт
        QFont font = QFont();
        font.setBold(true);
        font.setPointSize(48);        
        painter.setFont(font);
        // Устанавливаем цвет
        painter.setPen(QColor(0,255,0));
        // Рисуем надпись по центру окна
        painter.drawText(QRect(0,0,this->size().width(),this->size().height()),Qt::AlignCenter | Qt::AlignVCenter, "GAME OVER");
    };
    painter.end();
}

/// @brief Проверяет столкновения змеи
void Window::checkCollision()
{
    // столкновение с границами окна и со своим телом
    auto [x,y] = this->snakePos[0];
    if (x<=0 || y<=0 || x>=this->surface->size().width() || y>=this->surface->size().height() || this->collideWithSnake(this->snakePos[0])) { 
        // завершение игры
        this->gameOver(); 
        return;
    };    

    // столкновение с яблоком
    auto [apple_x, apple_y] = this->applePos;
    // Вычисляем площадь области пересечения головы змеи и яблока
    int intersect = this->intersection(this->snakePos[0],this->applePos);
    // Если площадь области пересечения больше чем
    if (intersect > 20) {
        // перемещаем яблоко в другое место
        this->locateApple();
        // добавляем сегмент к телу змеи
        this->extendBody();
    }
}

/// @brief Проверяет, пересекается ли точка с одним из сегментов тела змеи (кроме первого)
/// @param pos Координата точки (x,y)
/// @return True если пересекается
bool Window::collideWithSnake(pair<int,int> pos)
{
    for (int idx=1;idx<this->snakePos.size();++idx) {
        // Вычисляем площадь области пересечения объекта в позиции pos с каждым
        // сегментом тела змеи кроме первого
        int intersect = this->intersection(pos,this->snakePos[idx]);
        // Если площадь области пересечения больше чем
        if (intersect > 20) {
            return true;
        }
    }
    return false;
}

/// @brief Добавляет сегмент в конец тела змеи
void Window::extendBody()
{
    auto [x,y] = this->snakePos[this->snakePos.size()-1];
    this->snakePos.push_back({x+COL_WIDTH, y});
}

/// @brief Завершает игру
void Window::gameOver() {
    // Устанавливаем флаг что игра завершена
    this->isGameOver = true;
    // Перерисовываем экран
    this->repaint();
    // Останавливаем таймер
    this->timer->stop();
}

/// @brief Возвращает площадь области пересечения двух объектов
/// @param box1 Координаты левого верхнего угла первого объекта
/// @param box2 Координаты левого верхнего угла второго объекта
/// @return Площадь области пересечения объектов
int Window::intersection(pair<int,int> &box1, pair<int,int> &box2)
{
    // Вычисляем прямоугольник первого объекта
    auto [box1_x1,box1_y1] = box1;
    int box1_x2 = box1_x1 + COL_WIDTH, box1_y2 = box1_y1 + ROW_HEIGHT;
    // Вычисляем прямоугольник второго объекта
    auto [box2_x1,box2_y1] = box2;
    int box2_x2 = box2_x1 + COL_WIDTH, box2_y2 = box2_y1 + ROW_HEIGHT;
    // Вычисляем площадь области их пересечения
    int x1 = max(box1_x1,box2_x1);
    int y1 = max(box1_y1,box2_y1);
    int x2 = min(box1_x2,box2_x2);
    int y2 = min(box1_y2,box2_y2);
    return max(0,x2-x1)*max(0,y2-y1);    
}

/// @brief Обработчик события щелчка мыши в окне
/// @param event 
void Window::mousePressEvent(QMouseEvent *event)
{
    // Устанавливаем фокус на поверхость игрового поля
    this->surface->setFocus();
    // Убираем фокус с поля ввода угла поворота
    this->step_angle->clearFocus();
}

/// @brief Переводит угол из градусов в радианы
/// @param deg Угол в градусах
/// @return Угол в радианах
double deg2rad(int deg) {
    return (double)deg * M_PI / 180.0;
}
