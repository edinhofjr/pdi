#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include "challenge.cpp"

int main(int argc, char *argv[])
{
    challenge(); 
    
    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
