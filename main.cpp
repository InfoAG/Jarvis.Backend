#include "JarvisService.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    //JarvisService service(argc, argv);
    //return service.exec();

    QCoreApplication app(argc, argv);
    JarvisServer server;
    app.exec();
}
