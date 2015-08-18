#ifndef TEST_H
#define TEST_H

#include <QtCore>
#include "trainer.h"
#include "classifier.h"
#include <QDebug>

#define OUTPUT_FN "statistics.txt"

class Test
{
public:
    Test(const QString & folderName);
    void massiveTraining();
    void test(const QString & queryFolderName);

private:
    QString folderName;
    Trainer trainer;
    Classifier classifier;
};

#endif // TEST_H
