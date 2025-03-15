#include <QApplication>
#include <QFile>
#include <QStyle>
#include <QMainWindow>
#include "main_window.h"

QString loadStyleSheet(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        return QString(); // Return an empty string if the file cannot be opened
    }
    QTextStream stream(&file);
    return stream.readAll(); // Read the entire file
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;

    QString styleSheet = loadStyleSheet("styles.css");
    if (!styleSheet.isEmpty()) {
        app.setStyleSheet(styleSheet); // Apply to the entire application
        // Or: mainWindow.setStyleSheet(styleSheet); // Apply only to the main window
    }

    mainWindow.show();

    return app.exec();
}
