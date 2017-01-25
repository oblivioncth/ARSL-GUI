#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtInclude.h"
#include "procdialog.h"

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

    void on_PshBtnImgSel_clicked();

    void on_imgTable_cellClicked(int row, int column);

    void on_PshBtnCalibYMLSel_clicked();

    void on_PshBtnOutCrssSel_clicked();

    void on_PshBtnOutDecModSel_clicked();

    void on_actionStart_Calibration_triggered();

    void on_actionLoad_Configuration_triggered();

    void on_actionSave_Configuration_triggered();

    void on_actionModel_Viewer_triggered();

private:
    Ui::MainWindow *ui;

    void fnImgSelBtnTextSwitch(int BtnNum);
    int fnCompleteCheck();


    QPushButton * PshBtnImgSel[5] = {new QPushButton("Add"),new QPushButton("Add"),new QPushButton("Add"),new QPushButton("Add"),new QPushButton("Add")};
    QString qsImgSelPaths[5];
    QImage qimgImgSelPreview;
    QString qsCalibYMLSelPath;
    QString qsOutFileSelPaths[2];
};

#endif // MAINWINDOW_H
