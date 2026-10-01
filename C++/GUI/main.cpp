#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWindow window;
    window.setWindowTitle("Sales Tracking System");
    window.resize(1000, 680);
    window.show();

    return app.exec();
}

