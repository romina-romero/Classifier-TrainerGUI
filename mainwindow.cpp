#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->message->setText("Iniciando buscador");
    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);
    QString folder = settings.value("rootFolder",QVariant(".")).toString();
    classifier.init(folder);
    configurationWindow = new ConfigurationWindow(0,&classifier);

    ui->message->setText("Buscador preparado");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_configure_clicked()
{
    configurationWindow->exec();
}

void MainWindow::on_selectPicture_clicked()
{
    //get image fname
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Image files (*.png *.xpm *.jpg *.bmp *.jpeg)");
    if(dialog.exec()==0 || dialog.selectedFiles().size()==0 ) return;
    imageToSearch = dialog.selectedFiles()[0];

    //display
    QGraphicsScene *scn = new QGraphicsScene(ui->imageView);
    scn->setSceneRect(QRect(1,1,ui->imageView->rect().width()-2,ui->imageView->rect().height()-2));
    ui->imageView->setScene(scn);
    QPixmap pix(imageToSearch);
    scn->addPixmap(pix);
}

void MainWindow::on_search_clicked()
{
    //validate
    if(imageToSearch.isEmpty()){
        ui->message->setText("Debe cargar una imagen para la búsqueda");
        return;
    }

    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);
    QString folder = settings.value("rootFolder",QVariant(".")).toString();

    ui->message->setText("detectando coincidencias");
    classifier.detect(imread(imageToSearch.toStdString().c_str()));
    QVector<QString> matches = classifier.getBestMatches();

    //mostrar resultados
    //limpiar
    QLayoutItem *wItem;
    while ((wItem = ui->resultsLayout->takeAt(0)) != 0)
        delete wItem;
    //mostrar
    foreach(QString match, matches){
        //show results
        QLabel* label = new QLabel;
        label->setText(match.toUpper());
        ui->resultsLayout->addWidget(label);

        QGraphicsView* graphics = new QGraphicsView;
        QGraphicsScene *scn = new QGraphicsScene( graphics );
        scn->setSceneRect(QRect(1,1,198,98));
        graphics->setScene( scn );
        graphics->setFixedSize(200, 100 );
        QPixmap pix( folder+QDir::separator()+match);
        scn->addPixmap( pix );
        ui->resultsLayout->addWidget(graphics);

        QSpacerItem* space = new QSpacerItem(100,20);
        ui->resultsLayout->addSpacerItem(space);

    }
    ui->message->setText("Búsqueda finalizada");
    ui->message->setText("Buscador preparado");


}


