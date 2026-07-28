#include <QApplication>
#include "ui/ServerWindow.h"
#include "Database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("BookClub Server");
    QApplication::setApplicationVersion("1.0");

    ServerWindow window;
    window.show();

    return a.exec();
}