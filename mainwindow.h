#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configurationwindow.h"
#include "classifier.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_configure_clicked();

    void on_selectPicture_clicked();

    void on_search_clicked();

private:
    Ui::MainWindow *ui;
    ConfigurationWindow *configurationWindow;
    QString imageToSearch;
    Classifier classifier;

};

#endif // MAINWINDOW_H
