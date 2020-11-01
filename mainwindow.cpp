#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Showing amount of available free space
    QStorageInfo storage = QStorageInfo::root();
    qint64 available_bytes = storage.bytesAvailable()/1000000000;
    qint64 total_bytes = storage.bytesTotal()/1000000000;
    qint8 available_percent = available_bytes * 100 / total_bytes;
    QString storage_name = storage.fileSystemType();
    QString storage_info = QString("%1 GiB available out of %2 GiB (%3%) on [%4]").arg(QString::number(available_bytes), QString::number(total_bytes),
                                                                                       QString::number(available_percent), QString(storage_name));
    ui->available_storage->setText(storage_info);
    ui->available_storage->setStyleSheet("QLabel { background-color : white; }");

    // Setting up models and initial path for them
    QString sPath = QDir::homePath();
    dirmodel_1= new QFileSystemModel(this);
    dirmodel_2= new QFileSystemModel(this);
    dirmodel_1->setFilter(QDir::AllEntries | QDir::NoDot);
    dirmodel_1->setRootPath(sPath);
    dirmodel_2->setFilter(QDir::AllEntries | QDir::NoDot);
    dirmodel_2->setRootPath(sPath);
    ui->tableView_1->setModel(dirmodel_1);
    ui->tableView_2->setModel(dirmodel_2);

    // Set resizing options for two table views
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Ignoring table numbers
    ui->tableView_1->verticalHeader()->setVisible(false);
    ui->tableView_2->verticalHeader()->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_1_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel_1->fileInfo(index).absoluteFilePath();
    ui->tableView_1->setRootIndex(dirmodel_1->setRootPath(sPath));
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel_2->fileInfo(index).absoluteFilePath();
    ui->tableView_2->setRootIndex(dirmodel_2->setRootPath(sPath));
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton exit_box;
    exit_box = QMessageBox::question(this, "Exit procedure", "You want to exit this beautiful file manager?", QMessageBox::No | QMessageBox::Yes);
    if (exit_box != QMessageBox::No) {
        QApplication::instance()->quit();
    }
}

void MainWindow::on_actionAbout_file_manager_triggered()
{
    QMessageBox::about(this, "KM File manager", "This is file manager build with Qt5");
}

void MainWindow::on_actionTeam_triggered()
{
    QMessageBox::about(this, "Our team:", "Katya Detsyk\nMarki Lebyak");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionNew_File_triggered()
{

}

void MainWindow::on_actionNew_Folder_triggered()
{
    bool created;
    QString folder_name = QInputDialog::getText(this, "New Folder creation", "Enter folder name: ",
                                                QLineEdit::Normal, "no name", &created);

}
