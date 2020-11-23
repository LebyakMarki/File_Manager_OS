#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "file_info_functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchDialog(new SearchDialog)
{
    ui->setupUi(this);
    // Setting default application font size
    current_font_size = QApplication::font().pointSize();

    // Showing amount of available free space
    QStorageInfo storage = QStorageInfo::root();
    qint64 available_bytes = storage.bytesAvailable()/1024/1024/1024;
    qint64 total_bytes = storage.bytesTotal()/1024/1024/1024;
    qint8 available_percent = available_bytes * 100 / total_bytes;
    QString storage_name = storage.fileSystemType();
    QString storage_info = QString("%1 GiB available out of %2 GiB (%3%) on [%4]").arg(QString::number(available_bytes), QString::number(total_bytes),
                                                                                       QString::number(available_percent), QString(storage_name));
    ui->available_storage->setText(storage_info);
    ui->available_storage->setStyleSheet("QLabel { background-color : white; }");

    // Setting up models and initial path for them
    QString startPath = QDir::homePath();
    dirmodel_1= new QFileSystemModel(this);
    dirmodel_2= new QFileSystemModel(this);
    ui->tableView_1->setModel(dirmodel_1);
    ui->tableView_2->setModel(dirmodel_2);
    dirmodel_1->setRootPath(startPath);
    dirmodel_2->setRootPath(startPath);
    dirmodel_1->setFilter(QDir::AllEntries | QDir::NoDot);
    dirmodel_2->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->tableView_1->setRootIndex(dirmodel_1->index(startPath));
    ui->tableView_2->setRootIndex(dirmodel_2->index(startPath));

    // Set resizing options for two table views
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Ignoring table numbers
    ui->tableView_1->verticalHeader()->setVisible(false);
    ui->tableView_2->verticalHeader()->setVisible(false);

    // Removing Line separator
    ui->tableView_1->setShowGrid(false);
    ui->tableView_2->setShowGrid(false);

    // Setting paths
    right_part_path = startPath;
    left_part_path = startPath;

    // Showing paths
    ui->left_path->setText(startPath);
    ui->right_path->setText(startPath);
    ui->left_path->setStyleSheet("QLabel { background-color : white; }");
    ui->right_path->setStyleSheet("QLabel { background-color : white; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_1_clicked(const QModelIndex &index)
{
    right_main = false;
    QString sPath = dirmodel_1->fileInfo(index).absoluteFilePath();
    if (sPath == "/..") {
        return;
    }
    ui->left_path->setText(sPath);
    left_part_path = sPath;
}

void MainWindow::on_tableView_1_doubleClicked(const QModelIndex &index)
{
    right_main = false;
    QString sPath = dirmodel_1->fileInfo(index).absoluteFilePath();
    if (sPath == "/..") {
        return;
    }
    ui->tableView_1->setRootIndex(dirmodel_1->setRootPath(sPath));
    ui->left_path->setText(sPath);
    left_part_path = sPath;
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    right_main = true;
    QString sPath = dirmodel_2->fileInfo(index).absoluteFilePath();
    if (sPath == "/..") {
        return;
    }
    ui->right_path->setText(sPath);
    right_part_path = sPath;
}

void MainWindow::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    right_main = true;
    QString sPath = dirmodel_2->fileInfo(index).absoluteFilePath();
    if (sPath == "/..") {
        return;
    }
    ui->tableView_2->setRootIndex(dirmodel_2->setRootPath(sPath));
    ui->right_path->setText(sPath);
    right_part_path = sPath;
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton exit_box;
    exit_box = QMessageBox::question(this, "Exit procedure", "You want to exit this beautiful file manager?", QMessageBox::No | QMessageBox::Yes);
    if (exit_box != QMessageBox::No) {
        QApplication::instance()->quit();
    } else {
        QMessageBox::about(this, "Exit procedure", "Thanks for remaining.");
    }
}

void MainWindow::on_actionAbout_file_manager_triggered()
{
    QMessageBox::about(this, "KM File manager", "This is file manager build with Qt5 and C++ skills.");
}

void MainWindow::on_actionTeam_triggered()
{
    QMessageBox::about(this, "Our team:", "Katya Detsyk - best software engineer\nMarki Lebyak - just ok in programming");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionNew_File_triggered()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::AnyFile);
    QString file_name = file_dialog.getSaveFileName(this, "Creating file", "untitled.txt", "");
    if (file_name.isEmpty()) {
         QMessageBox::about(this, "File creation", "File not created.");
         return;
    }
    QFile file(file_name);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
        QMessageBox::about(this, "File creation", "File created successfully.");
    } else {
        QMessageBox::about(this, "File creation", "File already exists.");
    }
}

void MainWindow::on_actionNew_Folder_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select parent directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir directory(dir_name);
    bool got_text;
    QString new_dir_name = QInputDialog::getText(this, "Creating new directory", "Enter name:", QLineEdit::Normal, "untitled", &got_text);
    if (got_text && !new_dir_name.isEmpty()) {
        if (!directory.exists(new_dir_name)) {
            directory.mkdir(new_dir_name);
            QMessageBox::about(this, "Directory creation", "Directory created successfully.");
        } else {
            QMessageBox::about(this, "Directory creation", "Directory already exists.");
        }
    } else {
        QMessageBox::about(this, "Directory creation", "Directory not created.");
    }
}

void MainWindow::on_actionZoom_In_triggered()
{
    QFont font;
    font.setPointSize(current_font_size + 1);
    current_font_size += 1;
    ui->tableView_1->setFont(font);
    ui->tableView_2->setFont(font);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    QFont font;
    font.setPointSize(current_font_size - 1);
    current_font_size -= 1;
    ui->tableView_1->setFont(font);
    ui->tableView_2->setFont(font);
}

void MainWindow::on_actionDefault_Zoom_triggered()
{
    QFont font;
    font.setPointSize(QApplication::font().pointSize());
    current_font_size = QApplication::font().pointSize();
    ui->tableView_1->setFont(font);
    ui->tableView_2->setFont(font);
}



void MainWindow::on_actionRename_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select file to rename", "");
    QFile file(file_name);
    bool got_text;
    QString new_file_name = QInputDialog::getText(this, "Renaming file", "Enter new name:", QLineEdit::Normal, "untitled.txt", &got_text);
    if (got_text && !new_file_name.isEmpty()) {
        QString new_name = QString("%1/%2").arg(file.fileName().section("/",0,-2), new_file_name);
        if(file.rename(new_name)) {
            QMessageBox::about(this, "File renaming", "File renamed.");
        } else {
            QMessageBox::about(this, "File renaming", "File not renamed.");
        }
    } else {
        QMessageBox::about(this, "File renaming", "File not renamed.");
    }
}

void MainWindow::on_actionRename_Directory_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory to rename", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir directory(dir_name);
    bool got_text;
    QString new_dir_name = QInputDialog::getText(this, "Renaming directory", "Enter new name:", QLineEdit::Normal, "untitled", &got_text);
    if (got_text && !new_dir_name.isEmpty()) {
        QString new_name = QString("../%1").arg(new_dir_name);
        if (directory.rename(directory.path(), new_name)) {
            QMessageBox::about(this, "Directory renaming", "Directory renamed.");
        } else {
            QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
        }
    } else {
        QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
    }
}

void MainWindow::on_actionDelete_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select file to rename", "");
    if (QFile::remove(file_name)) {
        QMessageBox::about(this, "File deleting", "File deleted.");
    } else {
        QMessageBox::about(this, "File deleting", "File not deleted.");
    }
}

void MainWindow::on_actionDelete_Directory_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory to delete", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir directory(dir_name);
    if (directory.removeRecursively()) {
        QMessageBox::about(this, "Directory deleting", "Directory deleted.");
    } else {
        QMessageBox::about(this, "Directory deleting", "Directory not deleted.");
    }
}


void MainWindow::on_actionCopy_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select file to copy", "");
    QFile file(file_name);
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString file_copy_path = QString("%1/%2").arg(dir_name, file.fileName().section("/", -1, -1));
    if (QFile::exists(file_copy_path)){
        QFile::remove(file_copy_path);
    }
    if (QFile::copy(file_name, file_copy_path)) {
        QMessageBox::about(this, "File copying", "File copied.");
    } else {
        QMessageBox::about(this, "File copying", "File not copied.");
    }
}

void MainWindow::on_actionCopy_Directory_triggered()
{
    QString dir_name_from = QFileDialog::getExistingDirectory(this, "Select source directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dir_name_to = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

}

void MainWindow::on_actionMove_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select file to move", "");
    QFile file(file_name);
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString file_copy_path = QString("%1/%2").arg(dir_name, file.fileName().section("/", -1, -1));
    if (QFile::exists(file_copy_path)){
        QFile::remove(file_copy_path);
    }
    if (QFile::copy(file_name, file_copy_path)) {
        if (QFile::remove(file_name)) {
            QMessageBox::about(this, "File moving", "File moved.");
        } else {
            QMessageBox::about(this, "File moving", "File not moved.");
        }
    } else {
        QMessageBox::about(this, "File copying", "File not moved.");
    }
}

void MainWindow::on_actionMove_Directory_triggered()
{
    QString dir_name_from = QFileDialog::getExistingDirectory(this, "Select source directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dir_name_to = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}



void MainWindow::on_copyButton_clicked()
{
    QFile file(left_part_path);
    QFileInfo file_info(file);
    QFileInfo dir_info(right_part_path);
    if (file_info.isFile() && dir_info.isDir()) {
        QString file_copy_path = QString("%1/%2").arg(right_part_path, file.fileName().section("/", -1, -1));
        if (QFile::exists(file_copy_path)){
            QFile::remove(file_copy_path);
        }
        if (QFile::copy(left_part_path, file_copy_path)) {
            QMessageBox::about(this, "File copying", "File copied.");
        } else {
            QMessageBox::about(this, "File copying", "File not copied.");
        }
    } else {
        QMessageBox::about(this, "File copying", "On left window select file, on right directory.");
    }

}

void MainWindow::on_moveButton_clicked()
{
    QFile file(left_part_path);
    QFileInfo file_info(file);
    QFileInfo dir_info(right_part_path);
    if (file_info.isFile() && dir_info.isDir()) {
        QString file_copy_path = QString("%1/%2").arg(right_part_path, file.fileName().section("/", -1, -1));
        if (QFile::exists(file_copy_path)){
            QFile::remove(file_copy_path);
        }
        if (QFile::copy(left_part_path, file_copy_path)) {
            if (QFile::remove(left_part_path)) {
                QMessageBox::about(this, "File moving", "File moved.");
            } else {
                QMessageBox::about(this, "File moving", "File not moved.");
            }
        } else {
            QMessageBox::about(this, "File copying", "File not copied.");
        }
    } else {
        QMessageBox::about(this, "File copying", "On left window select file, on right directory.");
    }
}

void MainWindow::on_renameButon_clicked()
{
     QFile file(left_part_path);
     QFileInfo file_info(file);
     QFileInfo dir_info(left_part_path);
     bool got_text;
     if (file_info.isFile()) {
         QString new_file_name = QInputDialog::getText(this, "Renaming file", "Enter new name:", QLineEdit::Normal, "untitled.txt", &got_text);
         if (got_text && !new_file_name.isEmpty()) {
             QString new_name = QString("%1/%2").arg(file.fileName().section("/",0,-2), new_file_name);
             if(file.rename(new_name)) {
                 QMessageBox::about(this, "File renaming", "File renamed.");
             } else {
                 QMessageBox::about(this, "File renaming", "File not renamed.");
             }
         } else {
             QMessageBox::about(this, "File renaming", "File not renamed.");
         }
     } else if (dir_info.isDir()) {
         QString new_dir_name = QInputDialog::getText(this, "Renaming directory", "Enter new name:", QLineEdit::Normal, "untitled", &got_text);
         if (got_text && !new_dir_name.isEmpty()) {
             QString new_name = QString("../%1").arg(new_dir_name);
             QDir directory(left_part_path);
             if (directory.rename(directory.path(), new_name)) {
                 QMessageBox::about(this, "Directory renaming", "Directory renamed.");
             } else {
                 QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
             }
         } else {
             QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
         }
     } else {
         QMessageBox::about(this, "Renaming", "Only can rename dirs and files.");
     }

}

void MainWindow::on_deleteButton_clicked()
{
    QFile file(left_part_path);
    QFileInfo file_info(file);
    QFileInfo dir_info(left_part_path);
    if (file_info.isFile()) {
        if (QFile::remove(left_part_path)) {
            QMessageBox::about(this, "File deleting", "File deleted.");
        } else {
            QMessageBox::about(this, "File deleting", "File not deleted.");
        }
    } else if (dir_info.isDir()) {
        QDir directory(left_part_path);
        if (directory.removeRecursively()) {
            QMessageBox::about(this, "Directory deleting", "Directory deleted.");
        } else {
            QMessageBox::about(this, "Directory deleting", "Directory not deleted.");
        }
    } else {
        QMessageBox::about(this, "Deleting", "Only can delete dirs and files.");
    }
}

void MainWindow::on_actionSearch_triggered()
{
    searchDialog->show();
    searchDialog->activateWindow();
}

void MainWindow::on_actionExtract_triggered()
{

}

void MainWindow::on_actionExtract_to_triggered()
{

}

void MainWindow::on_actionZip_triggered()
{
    QFile file(left_part_path);
    QFileInfo file_info(file);
    QFileInfo dir_info(left_part_path);
    if (file_info.isFile()) {

    } else if (dir_info.isDir()) {

    } else {
        QMessageBox::about(this, "Zipping", "Only can zip dirs and files.");
    }
}


void MainWindow::on_viewButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    QFileInfo info(file_path);
    QString last_modified = info.lastModified().toString();
    QMessageBox msgBox;
    msgBox.setWindowTitle("File info");
    QString file_info_text = QString("File name: %1\nType: %2\nParent folder: %3\nLast modified: %4\nPermissions: %5").arg(info.fileName(),
                                                                                                                           get_file_type(info),
                                                                                                                           info.canonicalPath(),
                                                                                                                           last_modified,
                                                                                                                           get_file_permission(info));
    msgBox.setText(file_info_text);
    msgBox.exec();
}
