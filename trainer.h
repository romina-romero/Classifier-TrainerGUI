#ifndef TRAINER_H
#define TRAINER_H

#include <opencv2/opencv.hpp>

#include <vector>

#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QtCore>
#include <time.h>

#define WINDOW_WIDTH 96
#define WINDOW_HEIGHT 96

using namespace cv;
using namespace cv::ml;
using namespace std;

class Trainer
{
public:
    Trainer();
    void loadPositiveImages(const QString & directory);
    void loadNegativeImages(const QString & directory);
    void train(const QString &outputFilename);

private:
    void load_images(const QString & directory, QVector<Mat> &img_lst );
    void sample_neg(const Size & size);
    void compute_hog(const QVector<Mat> &img_lst, int label);
    void compute_hogs();
    Ptr<SVM> svm;
    QVector< Mat > positive_lst;
    QVector< Mat > negative_lst;
    QVector< Mat > gradient_lst;
    QVector< int > labels;

};

#endif // TRAINER_H
