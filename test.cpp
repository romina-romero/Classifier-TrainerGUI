#include "test.h"

Test::Test(const QString &folderName) : folderName(folderName)
{}

void Test::massiveTraining(){

    QDir dir(folderName);

    qint64 start = QDateTime::currentMSecsSinceEpoch();

    foreach(const QString &file, dir.entryList()){
        if(file=="." || file=="..") continue;
        if(!(QFileInfo(folderName+QDir::separator()+file)).isDir())continue;
        //adding
        qDebug() << "training " << file;

        trainer.loadPositiveImages(folderName+QDir::separator()+file+QDir::separator()+"positive");
        trainer.loadNegativeImages(folderName+QDir::separator()+file+QDir::separator()+"negative");
        trainer.train(folderName+QDir::separator()+file+".yml");
    }
    double duration = QDateTime::currentMSecsSinceEpoch()-start;
    duration=duration/1000;
    qDebug() << "Tiempo de entrenamiento:"<<duration<<"segundos";
}

void Test::test(const QString & queryFolderName){

    QHash<QString,int> false_positives;
    QHash<QString,double> true_positives;
    QHash<QString,double> as_seconds;

    classifier.init(folderName);
    int total=0;

    qint64 start = QDateTime::currentMSecsSinceEpoch();
    foreach(QString className, classifier.getClassesList()){
        false_positives.insert(className,0);
    }

    foreach(QString className, classifier.getClassesList()){
        qDebug() << "Testeando clase "<<className;
        QString sfolder = queryFolderName+QDir::separator()+className;

        //allowed images extensions
        QStringList allowedExtensions;
        allowedExtensions << ".png" << ".jpeg" << ".jpg";

        //listing
        QDir dir(sfolder);
        dir.setFilter(QDir::Files);

        double n = 0;
        double right = 0;
        double asSecond = 0;

        foreach(const QString &file, dir.entryList()){
            foreach(const QString &ext, allowedExtensions){
                if(file.toLower().endsWith(ext.toLower())){
                    //adding
                    std::string fname = (sfolder+QDir::separator()+file).toStdString();
                    Mat img = imread(fname.c_str());
                    //img_lst.append(img);
                    classifier.detect(img);
                    QVector<QString> matches = classifier.getBestMatches();

                    if(matches.size()>0 && matches[0]==className) right++;
                    else {
                        if(matches.size()>1 && matches[1]==className) asSecond++;
                        if(matches.size()>0) false_positives.insert(matches[0],(*false_positives.find(matches[0]))+1);
                    }
                    n+=1;
                }
            }
        }
        true_positives.insert(className,right/n);
        as_seconds.insert(className,asSecond/n);
        total+=n;
    }
    double duration = QDateTime::currentMSecsSinceEpoch()-start;
    duration=duration/1000;
    qDebug() << "Tiempo de entrenamiento:"<<duration<<"segundos";

    //guardar archivo
    QFile file(folderName+QDir::separator()+OUTPUT_FN);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "clase\tcorrectitud\tcomo segundo\tfalsos positivos\n";
    foreach(QString className, classifier.getClassesList()){
        out << className<<"\t"<<(*true_positives.find(className))<<"\t"<<(*as_seconds.find(className))<<"\t"<<(*false_positives.find(className))<<"\n";
    }
    file.close();
}
