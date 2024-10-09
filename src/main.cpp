#include "window.h"
#include "matrix.h"
#include "math.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[]) {
    // Создание приложение Qt
    QApplication app(argc, argv);
    // Создание и отображение главного окна
    Window wnd;
    wnd.setWindowTitle("Snake");
    wnd.show();
    // Запуск приложения Qt
    return app.exec();
}