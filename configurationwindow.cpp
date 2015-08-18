#include "configurationwindow.h"
#include "ui_configurationwindow.h"
ConfigurationWindow::ConfigurationWindow(QWidget *parent, Classifier *classifier) :
    QDialog(parent),
    classifier(classifier),
    ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);
    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);

    ui->rootFolderLabel->setText(settings.value("rootFolder",QVariant(".")).toString());

    updateClassLayout();
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::updateClassLayout(){
    //mostrar resultados
    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);
    QString folder = settings.value("rootFolder",QVariant(".")).toString();
    //limpiar
    QLayoutItem *wItem;
    while ((wItem = ui->classLayout->takeAt(0)) != 0){
        ui->classLayout->removeItem(wItem);
        delete wItem;}

    //mostrar
    foreach(QString className, classifier->getClassesList()){
        //show results
        QLabel* label = new QLabel;
        label->setText(className.toUpper());
        ui->classLayout->addWidget(label);

        QGraphicsView* graphics = new QGraphicsView;
        QGraphicsScene *scn = new QGraphicsScene( graphics );
        scn->setSceneRect(QRect(1,1,198,98));
        graphics->setScene( scn );
        graphics->setFixedSize(200, 100 );
        QPixmap pix( folder+QDir::separator()+className);
        scn->addPixmap( pix );
        ui->classLayout->addWidget(graphics);

        QSpacerItem* space = new QSpacerItem(100,20);
        ui->classLayout->addSpacerItem(space);

    }
}

void ConfigurationWindow::on_selectRootFolder_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.exec();

    if(dialog.selectedFiles().size()==0) return;
    QString newFolder = dialog.selectedFiles()[0];
    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);
    settings.setValue("rootFolder",QVariant(newFolder));
    ui->rootFolderLabel->setText(newFolder);

    classifier->init(newFolder);
    updateClassLayout();
}

void ConfigurationWindow::on_selectPresentationImage_clicked()
{
    //get image fname
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Image files (*.png *.xpm *.jpg *.bmp)");
    if(dialog.exec()==0 || dialog.selectedFiles().size()==0 ) return;
    trainingPresentationFn = dialog.selectedFiles()[0];

    //display
    QGraphicsScene *scn = new QGraphicsScene(ui->presentationImage);
    scn->setSceneRect(ui->presentationImage->rect());
    ui->presentationImage->setScene(scn);
    QPixmap pix(trainingPresentationFn);
    scn->addPixmap(pix);
}

void ConfigurationWindow::on_selectPositivesFolder_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.exec();

    if(dialog.selectedFiles().size()==0) return;
    QString newFolder = dialog.selectedFiles()[0];
    ui->positivesFolderLabel->setText(newFolder);
    positiveFolderFn = newFolder;
}

void ConfigurationWindow::on_selectNegativesFolder_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.exec();

    if(dialog.selectedFiles().size()==0) return;
    QString newFolder = dialog.selectedFiles()[0];
    ui->negativesFolderLabel->setText(newFolder);
    negativeFolderFn = newFolder;
}

void ConfigurationWindow::on_train_clicked()
{
    //validate
    if(     positiveFolderFn.isEmpty()   ||   negativeFolderFn.isEmpty() ||
            !QDir(positiveFolderFn).exists() ||
            !QDir(negativeFolderFn).exists() ){
        ui->message->setText("Debe seleccionar carpetas válidas");
        return;
    }
    if(positiveFolderFn==negativeFolderFn){
        ui->message->setText("Las carpetas no pueden ser iguales");
        return;
    }
    QString className = ui->newObjName->toPlainText().trimmed();
    if(className.isEmpty()){
        ui->message->setText("El nombre del nuevo objeto no puede estar vacío.");
        return;
    }

    if(trainingPresentationFn.isEmpty()){
        ui->message->setText("Debe seleccionar una imagen de presentación");
        return;
    }

    ui->message->setText("Preparandose para entrenar");

    QSettings settings(SETTINGS_FNAME,QSettings::IniFormat);
    QString folder = settings.value("rootFolder",QVariant(".")).toString();
    QFile img(trainingPresentationFn);
    img.copy(folder+QDir::separator()+className);

    Trainer trainer;
    trainer.loadPositiveImages(positiveFolderFn);
    trainer.loadNegativeImages(negativeFolderFn);


    ui->message->setText("Entrenando...");
    trainer.train(folder + QDir::separator() + className+".yml");
    ui->message->setText("Entrenamiento completo");

    classifier->init(folder);
    updateClassLayout();

}
