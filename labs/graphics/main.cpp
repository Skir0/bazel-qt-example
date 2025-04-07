#include <QtWidgets/QApplication>
#include <QtWidgets>

#include "Controller.h"
#include "PaintWidget.h"


int main(int argc, char** argv) {
    QApplication a(argc, argv);

    Controller controller;
    controller.show();
    controller.setWindowTitle("Мои напоминания");
    controller.resize(400, 500);
    return a.exec();
}
