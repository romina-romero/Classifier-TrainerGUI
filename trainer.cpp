#include "trainer.h"
#include <QDebug>

Trainer::Trainer()
{
    svm = SVM::create();
    /* Default values to train SVM */
    svm->setCoef0(0.0);
    svm->setDegree(3);
    svm->setTermCriteria(TermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-3 ));
    svm->setGamma(0);
    svm->setKernel(SVM::LINEAR);
    svm->setNu(0.5);
    svm->setP(0.1);
    svm->setC(0.01); // From paper, soft classifier
    svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task

}

void Trainer::load_images(const QString & directory, QVector<Mat> &img_lst )
{
    //allowed images extensions
    QStringList allowedExtensions;
    allowedExtensions << ".png" << ".jpeg" << ".jpg";

    //listing
    QDir dir(directory);
    dir.setFilter(QDir::Files);

    foreach(const QString &file, dir.entryList()){
        foreach(const QString &ext, allowedExtensions){
            if(file.toLower().endsWith(ext.toLower())){
                //adding
                std::string fname = (directory+QDir::separator()+file).toStdString();
                Mat img = imread(fname.c_str());
                img_lst.append(img);
            }
        }
    }
}


void Trainer::sample_neg(const Size & size){
    Rect box;

    srand( (unsigned int)time( NULL ) );

    QVector< Mat > full_neg;

    foreach(Mat img, negative_lst){

        box.width = size.width;
        box.height = size.height;

        //if the picture is small
        if(img.rows<size.height || img.cols<size.width){
            box.width-=8; box.height-=8;
        }
        //small again
        if(img.rows<size.height-8 || img.cols<size.width-8){
            box.width-=8; box.height-=8;
        }
        //smaller, discard
        if(img.rows<size.height-16 || img.cols<size.width-16){
            qDebug() << "Warning: imagen descartada";
            continue;
        }

        box.x = rand() % (img.cols - box.width-1);
        box.y = rand() % (img.rows - box.height-1);

       // qDebug() << "x" << box.x << "y" << box.y << "w" << img.cols << "h" << img.rows;
        Mat roi = img(box);
        full_neg.append(roi.clone());
    }
    negative_lst = full_neg;

   // 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows in function Mat


}


void Trainer::loadPositiveImages(const QString &directory){
    positive_lst.clear();
    this->load_images(directory,positive_lst);
}

void Trainer::loadNegativeImages(const QString &directory){
    negative_lst.clear();
    this->load_images(directory,negative_lst);
    sample_neg(Size(WINDOW_WIDTH,WINDOW_HEIGHT));
}

void Trainer::compute_hog(const QVector< Mat > & img_lst, int label){
    HOGDescriptor hog;
    hog.winSize = Size(WINDOW_WIDTH,WINDOW_HEIGHT);
    Mat gray;

    vector< Point > location;
    vector< float > descriptors;

    foreach( Mat img, img_lst){
        cvtColor(img, gray, COLOR_BGR2GRAY);
        hog.compute(gray, descriptors, Size(8,8), Size(0,0),location);
        if(descriptors.size()==0){
            qDebug() << "Warning: no se pudo obtener descriptor" << label;
            continue;
        }
        gradient_lst.append(Mat(descriptors).clone());
        labels.append(label);

    }
}

void Trainer::compute_hogs()
{
    gradient_lst.clear();
    labels.clear();
    compute_hog(positive_lst,+1);
    compute_hog(negative_lst,-1);
}

void Trainer::train( const QString & outputFilename){

    //compute descriptors
    qDebug() << "computando descriptores";
    compute_hogs();

    //convert data to mat
    const int rows = gradient_lst.size();
    const int cols = (int)std::max( gradient_lst[0].cols, gradient_lst[0].rows );
    Mat tmp(1, cols, CV_32FC1); //< used for transposition if needed
    Mat trainData = Mat(rows, cols, CV_32FC1 );

    int i=0;
    qDebug() << "generando vectores para entrenamiento ";
    foreach(Mat gradient, gradient_lst){
        CV_Assert( gradient.cols == 1 || gradient.rows == 1 );
        if(gradient.cols == 1){
            transpose(gradient,tmp);
            tmp.copyTo(trainData.row(i));
        }else if(gradient.rows==1){
            gradient.copyTo( trainData.row( i ) );
        }
        i++;
    }

    //train :)
    qDebug() << "Entrenando";
    svm->train(trainData, ROW_SAMPLE, Mat(labels.toStdVector()));
    qDebug() << "Guardando archivo resultante";
    svm->save(outputFilename.toStdString().c_str());
}



