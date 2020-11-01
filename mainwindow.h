#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

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

    void on_actionExit_triggered();

    void on_actionAbout_file_manager_triggered();

    void on_actionTeam_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionNew_File_triggered();

    void on_actionNew_Folder_triggered();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirmodel_1;
    QFileSystemModel *dirmodel_2;
};
#endif // MAINWINDOW_H
