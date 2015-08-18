#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include "detector.h"


class Classifier
{
public:
    Classifier();
    void init(const QString & foldername);
    void detect(const Mat & img);
    QVector<QString> getBestMatches();
    QVector<QString> getClassesList();

private:
    QVector<Detector> detectors;
    QHash<QString,double> maxScores;
    QVector<QPair<int, QString> > detections;

};

#endif // CLASSIFIER_H
