#include "detector.h"

Detector::Detector()
{
}

Detector::Detector(const QString& className){
    this->className = className;
}

void Detector::setClassName(const QString& className){
    this->className = className;
}

QString Detector::getClassName() const{
    return className;
}



void Detector::init(const QString &filename){

    // Load the trained SVM.
    svm = StatModel::load<SVM>( filename.toStdString() );

    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) || (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;

    // Set the trained svm to my_hog
    my_hog.winSize = Size(WINDOW_WIDTH,WINDOW_HEIGHT);
    my_hog.setSVMDetector( hog_detector );

}

void Detector::detect(const Mat &img, vector<Rect> &foundLocations, vector<double> &foundWeights){

    my_hog.detectMultiScale( img, foundLocations ,foundWeights);

}
