#include "procdialog.h"
#include "ui_procdialog.h"
#include "mainwindow.h"
#include "QtInclude.h"

ProcDialog::ProcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcDialog)
{
    ui->setupUi(this);

    //Setup Process Dialog
    ui->PshBtnCalibFinish->setEnabled(false);
}

ProcDialog::~ProcDialog()
{
    delete ui;
}

void ProcDialog::fnStatusWindowUpdate(QString qsMesg)
{
    QDateTime qtStatusTime = QDateTime::currentDateTime();
    QString qsStatusTime = qtStatusTime.toString("h:mm:ss AP");

    ui->pteCalibProcWindow->appendPlainText("["+qsStatusTime+"]: "+qsMesg);
    qApp->processEvents();
}

void ProcDialog::fnStatusWindowFinish()
{
    ui->PshBtnCalibFinish->setEnabled(true);
}

void ProcDialog::fnStatusBarChange(int iBarLevel)
{
    ui->PrgBarCalibStatus->setValue(iBarLevel);
}

void ProcDialog::on_PshBtnCalibFinish_clicked()
{
    qApp->exit();
}
