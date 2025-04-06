#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

#include "Controller.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    Controller controller;
    controller.show();
    controller.setWindowTitle("Мои напоминания");
    controller.resize(400, 500);
    return a.exec();
}
