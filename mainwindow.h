#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include "searchdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableView_2_clicked(const QModelIndex &index);
    void on_tableView_1_clicked(const QModelIndex &index);
    void on_tableView_1_doubleClicked(const QModelIndex &index);
    void on_tableView_2_doubleClicked(const QModelIndex &index);
    void on_actionExit_triggered();
    void on_actionAbout_file_manager_triggered();
    void on_actionTeam_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionNew_File_triggered();
    void on_actionNew_Folder_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionDefault_Zoom_triggered();
    void on_actionRename_File_triggered();
    void on_actionRename_Directory_triggered();
    void on_actionDelete_File_triggered();
    void on_actionDelete_Directory_triggered();
    void on_actionCopy_File_triggered();
    void on_actionCopy_Directory_triggered();
    void on_actionMove_File_triggered();
    void on_actionMove_Directory_triggered();
    void on_copyButton_clicked();
    void on_moveButton_clicked();
    void on_renameButon_clicked();
    void on_deleteButton_clicked();

    void on_actionSearch_triggered();

    void on_actionExtract_triggered();

    void on_actionExtract_to_triggered();

    void on_actionZip_triggered();

    void on_viewButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirmodel_1;
    QFileSystemModel *dirmodel_2;
    QString right_part_path;
    QString left_part_path;
    int8_t current_font_size;
    bool right_main;
    QScopedPointer<SearchDialog> searchDialog;
};
#endif // MAINWINDOW_H
