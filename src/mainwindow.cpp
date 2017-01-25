#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "procdialog.h"
#include "AR_Main.h"
#include "QtInclude.h"
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
       ui->setupUi(this); //Perpare mainwindow.ui

       //Setup Main Window
       this->statusBar()->setSizeGripEnabled(false);
       this->setFixedSize(this->size().width(), this->size().height());


       //Place/Setup Buttons
       for(int i = 0; i < 5; i++)
       {
           ui->imgTable->setCellWidget( i, 0, PshBtnImgSel[i]);
       }
       QObject::connect(PshBtnImgSel[0], SIGNAL(clicked()),this, SLOT(on_PshBtnImgSel_clicked()));
       QObject::connect(PshBtnImgSel[1], SIGNAL(clicked()),this, SLOT(on_PshBtnImgSel_clicked()));
       QObject::connect(PshBtnImgSel[2], SIGNAL(clicked()),this, SLOT(on_PshBtnImgSel_clicked()));
       QObject::connect(PshBtnImgSel[3], SIGNAL(clicked()),this, SLOT(on_PshBtnImgSel_clicked()));
       QObject::connect(PshBtnImgSel[4], SIGNAL(clicked()),this, SLOT(on_PshBtnImgSel_clicked()));

       //Modify Table
       ui->imgTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
       ui->imgTable->setColumnWidth(0,70);
       ui->imgTable->setColumnWidth(1,90);
       ui->imgTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
       ui->imgTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
       ui->imgTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

       //Setup Picture Preview
       ui->imgSelPreview->setScaledContents(true);
       ui->imgSelPreview->setStyleSheet("border: 1px solid rgb(122,122,122);background: white");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::fnImgSelBtnTextSwitch(int BtnNum)
{
    if("Add" == PshBtnImgSel[BtnNum]->text())
    {
        if(BtnNum == 0)
            qsImgSelPaths[BtnNum] = QFileDialog::getOpenFileName(this,tr("Select an image for 'Color Grid'"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        if(BtnNum == 1)
            qsImgSelPaths[BtnNum] = QFileDialog::getOpenFileName(this,tr("Select an image for 'Pattern'"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        if(BtnNum == 2)
            qsImgSelPaths[BtnNum] = QFileDialog::getOpenFileName(this,tr("Select an image for 'Puri Grid'"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        if(BtnNum == 3)
            qsImgSelPaths[BtnNum] = QFileDialog::getOpenFileName(this,tr("Select an image for 'Skeleton Mask'"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        if(BtnNum == 4)
            qsImgSelPaths[BtnNum] = QFileDialog::getOpenFileName(this,tr("Select an image for 'Water Mask'"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        if(!qsImgSelPaths[BtnNum].isEmpty())
        {
            ui->imgTable->item(BtnNum,2)->setText(QDir::toNativeSeparators(qsImgSelPaths[BtnNum]));
            PshBtnImgSel[BtnNum]->setText("Remove");

            QModelIndex newIndex  = ui->imgTable->model()->index(BtnNum,1);
            ui->imgTable->selectionModel()->select(newIndex, QItemSelectionModel::Select);
            ui->imgTable->setCurrentIndex(newIndex);
            ui->imgTable->setFocus();
            on_imgTable_cellClicked(BtnNum, 1);
        }
    }
    else if("Remove" == PshBtnImgSel[BtnNum]->text())
    {
        ui->imgTable->item(BtnNum,2)->setText("[NO IMAGE SELECTED]");
        PshBtnImgSel[BtnNum]->setText("Add");
        if(QImage(qsImgSelPaths[BtnNum])== qimgImgSelPreview)
            ui->imgSelPreview->setText("NO IMAGE SELECTED");
        qsImgSelPaths[BtnNum].clear();
    }
}

int MainWindow::fnCompleteCheck()
{
    int iErrorPrint = 0;

    for(int i = 0;i < 5; i++)
    {
      if(qsImgSelPaths[i].isEmpty())
          iErrorPrint = 1;
    }
    if(qsCalibYMLSelPath.isEmpty())
        iErrorPrint = 1;
    for(int i = 0;i < 2; i++)
    {
      if(qsOutFileSelPaths[i].isEmpty())
          iErrorPrint = 1;
    }
    if(iErrorPrint == 1)
    {
        QMessageBox msgPathMiss;
        QString qsMissPaths = "";
        msgPathMiss.setText("One or more paths have not be assigned during setup. Please ensure that all files have a location associated with them.");
        msgPathMiss.setIcon(QMessageBox::Critical);
        msgPathMiss.setInformativeText("Click \"Show Details\" for more information.");
        msgPathMiss.setStandardButtons(QMessageBox::Ok);
        msgPathMiss.setDefaultButton(QMessageBox::Ok);
        msgPathMiss.setWindowTitle("Error: Setup not complete");

        if(qsCalibYMLSelPath.isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Calibration YAML\n";
        if(qsImgSelPaths[0].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Color Grid Image\n";
        if(qsImgSelPaths[1].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Pattern Image\n";
        if(qsImgSelPaths[2].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Puri Grid Image\n";
        if(qsImgSelPaths[3].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Skeleton Mask Image\n";
        if(qsImgSelPaths[4].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Input: Water Mask Image\n";
        if(qsOutFileSelPaths[0].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Output: Crosses Vector\n";
        if(qsOutFileSelPaths[1].isEmpty())
           qsMissPaths = qsMissPaths + "MISSING - Output: 3D Reconstruction Model\n";
        msgPathMiss.setDetailedText(qsMissPaths);
        msgPathMiss.exec();
        return 0;
    }
    else
        return 1;
}


void MainWindow::on_PshBtnImgSel_clicked()
{

    QObject* oButtonIden = sender();
    if( oButtonIden == PshBtnImgSel[0] )
        fnImgSelBtnTextSwitch(0);
    if( oButtonIden == PshBtnImgSel[1] )
        fnImgSelBtnTextSwitch(1);
    if( oButtonIden == PshBtnImgSel[2] )
        fnImgSelBtnTextSwitch(2);
    if( oButtonIden == PshBtnImgSel[3] )
        fnImgSelBtnTextSwitch(3);
    if( oButtonIden == PshBtnImgSel[4] )
        fnImgSelBtnTextSwitch(4);
}

void MainWindow::on_imgTable_cellClicked(int row, int column)
{
    if(column == 1)
    {
        if(qsImgSelPaths[row].isEmpty())
        {
            ui->imgSelPreview->setText("NO IMAGE SELECTED");
            ui->LblImgInfoType->setText("");
            ui->LblImgInfoRes->setText("");
            ui->LblImgInfoSize->setText("");
        }
        else
        {
            qimgImgSelPreview = QImage(qsImgSelPaths[row]);
            ui->imgSelPreview->setPixmap(QPixmap::fromImage(qimgImgSelPreview));
            QFileInfo fiImg(qsImgSelPaths[row]);
            ui->LblImgInfoType->setText("  Format: " + fiImg.completeSuffix().toUpper());
            QImage imTempLoad;
            imTempLoad.load(qsImgSelPaths[row]);
            ui->LblImgInfoRes->setText("Dimmensions: " + QString::number(imTempLoad.width()) + " x " + QString::number(imTempLoad.height()));
            ui->LblImgInfoSize->setText("Size: " + QString::number(fiImg.size()/1024) + "KB");
        }
    }
}
void MainWindow::on_PshBtnCalibYMLSel_clicked()
{
    qsCalibYMLSelPath = QFileDialog::getOpenFileName(this,tr("Select Calibration YAML File"), QDir::homePath(), tr("YAML Text (*.yml)"));
    ui->LnEditCalbYMLPath->setText(QDir::toNativeSeparators(qsCalibYMLSelPath));
}
void MainWindow::on_PshBtnOutCrssSel_clicked()
{
    qsOutFileSelPaths[0] = QFileDialog::getSaveFileName(this,tr("Select name/directory for output Crosses Vector"), QDir::homePath(), tr("YAML Text (*.yml)"));
    ui->LnEditOutCrssPath->setText(QDir::toNativeSeparators(qsOutFileSelPaths[0]));
}

void MainWindow::on_PshBtnOutDecModSel_clicked()
{
    qsOutFileSelPaths[1] = QFileDialog::getSaveFileName(this,tr("Select name/directory for output 3D Reconstruction Model"), QDir::homePath(), tr("3D Model (*.obj)"));
    ui->LnEditOutDecModPath->setText(QDir::toNativeSeparators(qsOutFileSelPaths[1]));
}

void MainWindow::on_actionStart_Calibration_triggered()
{
    if(fnCompleteCheck() == 1)
    {
        ProcDialog *CalibPD = new ProcDialog(this);
        MainWindow *pMWRef = &MainWindow(this);
        CalibPD->setModal(true);
        CalibPD->setAttribute(Qt::WA_DeleteOnClose,true);
        CalibPD->show();
        qApp->processEvents();
        AR_Main(CalibPD,pMWRef,qsCalibYMLSelPath,qsImgSelPaths,qsOutFileSelPaths);
    }
}

void MainWindow::on_actionLoad_Configuration_triggered()
{
    QString qsINIPath = QFileDialog::getOpenFileName(this,tr("Select file directory configuration"), QDir::homePath(), tr("ARSL Config (*.ini)"));
    if(!qsINIPath.isEmpty())
    {
        QFile qfINI(qsINIPath);
        qfINI.open(QIODevice::ReadOnly);
        QTextStream tsVarLoad(&qfINI);
        QString qsTempRead;

        qsTempRead = tsVarLoad.readLine();
        qsCalibYMLSelPath = qsTempRead.remove(0,11);
        qsTempRead = tsVarLoad.readLine();
        qsImgSelPaths[0] = qsTempRead.remove(0,12);
        qsTempRead = tsVarLoad.readLine();
        qsImgSelPaths[1] = qsTempRead.remove(0,9);
        qsTempRead = tsVarLoad.readLine();
        qsImgSelPaths[2] = qsTempRead.remove(0,11);
        qsTempRead = tsVarLoad.readLine();
        qsImgSelPaths[3] = qsTempRead.remove(0,12);
        qsTempRead = tsVarLoad.readLine();
        qsImgSelPaths[4] = qsTempRead.remove(0,12);
        qsTempRead = tsVarLoad.readLine();
        qsOutFileSelPaths[0] = qsTempRead.remove(0,15);
        qsTempRead = tsVarLoad.readLine();
        qsOutFileSelPaths[1] = qsTempRead.remove(0,12);

        for(int i = 0; i < 5; i++)
        {
            ui->imgTable->item(i,2)->setText(QDir::toNativeSeparators(qsImgSelPaths[i]));
            PshBtnImgSel[i]->setText("Remove");
        }
        ui->LnEditCalbYMLPath->setText(QDir::toNativeSeparators(qsCalibYMLSelPath));
        ui->LnEditOutCrssPath->setText(QDir::toNativeSeparators(qsOutFileSelPaths[0]));
        ui->LnEditOutDecModPath->setText(QDir::toNativeSeparators(qsOutFileSelPaths[1]));

        ui->imgSelPreview->setText("Select an image type to see its preview");
        ui->LblImgInfoType->setText("");
        ui->LblImgInfoRes->setText("");
        ui->LblImgInfoSize->setText("");
        ui->imgTable->clearSelection();
        ui->imgTable->setCurrentCell(-1,-1); //Set to invalid index so that previous selection outline disapears

        qfINI.close();
    }
}

void MainWindow::on_actionSave_Configuration_triggered()
{
    if(fnCompleteCheck() == 1)
    {
        QString qsINIPath = QFileDialog::getSaveFileName(this,tr("Select name/directory to save the filepath configuration"), QDir::homePath(), tr("ARSL Config (*.ini)"));
        if(!qsINIPath.isEmpty())
        {
            QFile qfINI(qsINIPath);
            qfINI.open(QIODevice::WriteOnly|QFile::Text);
            QTextStream tsVarSave(&qfINI);
            tsVarSave << "Calib_YML: " << qsCalibYMLSelPath << endl;
            tsVarSave << "Color_Grid: " << qsImgSelPaths[0] << endl;
            tsVarSave << "Pattern: " << qsImgSelPaths[1] << endl;
            tsVarSave << "Puri_Grid: " << qsImgSelPaths[2] << endl;
            tsVarSave << "Skele_Mask: " << qsImgSelPaths[3] << endl;
            tsVarSave << "Water_Mask: " << qsImgSelPaths[4] << endl;
            tsVarSave << "Out_Cross_Vec: " << qsOutFileSelPaths[0] << endl;
            tsVarSave << "Out_3D_Obj: " << qsOutFileSelPaths[1] << endl;
        }
        QMessageBox qmbSaveConfirm;
        qmbSaveConfirm.setText("The configuration was saved succesfully.");
        qmbSaveConfirm.exec();
     }
}

void MainWindow::on_actionModel_Viewer_triggered()
{
    QProcess *ModelViewer = new QProcess(this);
    ModelViewer->start("ModelViewer.exe");
}
