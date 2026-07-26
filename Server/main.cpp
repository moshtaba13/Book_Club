#include <QApplication>
#include "Database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // تنظیم اطلاعات اپلیکیشن
    QApplication::setApplicationName("BookClub Server");
    QApplication::setApplicationVersion("1.0");


    return a.exec();
}