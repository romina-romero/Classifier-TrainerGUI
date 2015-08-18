#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

#include "trainer.h"
#include "classifier.h"

#define SETTINGS_FNAME "settings.ini"

namespace Ui {
class ConfigurationWindow;
}

class ConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationWindow(QWidget *parent = 0, Classifier *classifier=0);
    ~ConfigurationWindow();

private slots:
    void on_selectRootFolder_clicked();
    void on_selectPresentationImage_clicked();

    void on_selectPositivesFolder_clicked();

    void on_selectNegativesFolder_clicked();

    void on_train_clicked();

private:
    Ui::ConfigurationWindow *ui;

    QString trainingPresentationFn;
    QString positiveFolderFn;
    QString negativeFolderFn;
    Classifier * classifier;

    void updateClassLayout();
};

#endif // CONFIGURATIONWINDOW_H
