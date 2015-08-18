#include "classifier.h"

Classifier::Classifier()
{
}

void Classifier::init(const QString & foldername){

    detectors.clear();

    //allowed extension
    QStringList allowedExtensions;
    allowedExtensions << ".yml";

    //listing
    QDir dir(foldername);
    dir.setFilter(QDir::Files);

    foreach(const QString &file, dir.entryList()){
        foreach(const QString &ext, allowedExtensions){
            if(file.toLower().endsWith(ext.toLower())){
                //adding
                QString fname = (foldername+QDir::separator()+file);
                Detector detector(file.left(file.length()-4));
                detector.init(fname);
                detectors.append(detector);
            }
        }
    }
}


void Classifier::detect(const Mat &img){

    maxScores.clear();
    foreach(Detector detector, detectors){

        vector<Rect> foundLocations;
        vector<double> foundWeights;
        detector.detect(img,foundLocations,foundWeights);

        for(uint i=0; i<foundLocations.size(); i++){
            double val = qMax((*(maxScores.find(detector.getClassName()))),foundWeights.at(i));
            qDebug() << val;
            maxScores.insert(detector.getClassName(),val);
        }
    }

}

QVector<QString> Classifier::getClassesList(){
    QVector<QString> classes;
    foreach(Detector detector, detectors){
        classes.append(detector.getClassName());
    }
    return classes;
}

//function to sort
bool greaterThan( const QPair<double, QString> e1, const QPair<double, QString>e2);

bool greaterThan(const QPair<double, QString> e1, const QPair<double, QString> e2){
    return e1.first > e2.first;
}

QVector<QString> Classifier::getBestMatches(){

    QHash<QString, double>::iterator it;
    QVector< QPair<double, QString> > maxScoresVector;
    for(it = maxScores.begin(); it!=maxScores.end(); it++){
        if(it.value()>0){
            QPair<double, QString> nvote(it.value(),it.key());
            maxScoresVector.append(nvote);
        }
    }
    qSort(maxScoresVector.begin(),maxScoresVector.end(),greaterThan);
    QVector <QString> classes;

    for(int i=0; i<maxScoresVector.size(); i++){
        QPair<double , QString> pair = maxScoresVector.at(i);
        classes.append(pair.second);
    }
    return classes;
}

