#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w("ActiviteMachine.db");
    w.show();
    return a.exec();
}
