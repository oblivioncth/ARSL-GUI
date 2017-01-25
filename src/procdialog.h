#ifndef PROCDIALOG_H
#define PROCDIALOG_H

#include <QDialog>

namespace Ui {
class ProcDialog;
}

class ProcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcDialog(QWidget *parent = 0);
    ~ProcDialog();
    void fnStatusWindowUpdate(QString qsMesg);
    void fnStatusWindowFinish();
    void fnStatusBarChange(int iBarLevel);


private slots:

    void on_PshBtnCalibFinish_clicked();

private:
    Ui::ProcDialog *ui;

};

#endif // PROCDIALOG_H
