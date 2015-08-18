#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QDebug>
#include "test.h"

int main(int argc, char *argv[])
{
    if (argc>3){
        if(argv[1][0]=='-' && argv[1][1]=='t'){
            //testear
            Test *test = new Test(QString(argv[2]));
            test->massiveTraining();
            test->test(QString(argv[3]));
            return 0;
        }
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
