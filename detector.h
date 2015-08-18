#ifndef DETECTOR_H
#define DETECTOR_H
#include <opencv2/opencv.hpp>
#include <QtCore>
#include "trainer.h"

using namespace cv;
using namespace cv::ml;
using namespace std;

class Detector
{
public:
    Detector();
    Detector(const QString& className);

    void setClassName(const QString& className);
    QString getClassName() const;

    void init(const QString & filename);
    void detect(const Mat& img, vector<Rect>& foundLocations, vector<double>& foundWeights);

private:
    Ptr<SVM> svm;
    vector< float > hog_detector;
    HOGDescriptor my_hog;
    QString className;
};

#endif // DETECTOR_H
