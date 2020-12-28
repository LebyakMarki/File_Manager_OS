#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include "file_info_functions.h"
#include "archives.h"
#include <archive.h>
#include <archive_entry.h>
#include <string.h>

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
    ui->available_storage->setAlignment(Qt::AlignCenter);
    ui->available_storage->setEnabled(false);


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
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Ignoring table numbers
    ui->tableView_1->verticalHeader()->setVisible(false);
    ui->tableView_2->verticalHeader()->setVisible(false);

    // Removing Line separator
    ui->tableView_1->setShowGrid(false);
    ui->tableView_2->setShowGrid(false);

    // Different colors for lines
    ui->tableView_1->setAlternatingRowColors(true);
    ui->tableView_2->setAlternatingRowColors(true);

    // Setting paths
    right_part_path = startPath;
    left_part_path = startPath;

    // Showing paths
    ui->left_path->setText(startPath);
    ui->right_path->setText(startPath);
    ui->right_path->setAlignment(Qt::AlignCenter);
    ui->left_path->setAlignment(Qt::AlignCenter);

    // Hotkeys
    ui->renameButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F2));
    ui->editButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F3));
    ui->viewButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F4));
    ui->copyButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
    ui->moveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F6));
    ui->deleteButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F9));
    ui->newFileButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F7));
    ui->newDirButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F8));
    ui->quitButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F10));
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
    if (file_name.isEmpty() && file_name.isNull()) {
         return;
    }
    QFile file(file_name);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    } else {
        QMessageBox::about(this, "File creation", "File already exists.");
    }
}

void MainWindow::on_actionNew_Folder_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select parent directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name.isEmpty() && dir_name.isNull()) {
        return;
    }
    QDir directory(dir_name);
    bool got_text;
    QString new_dir_name = QInputDialog::getText(this, "Creating new directory", "Enter name:", QLineEdit::Normal, "untitled", &got_text);
    if (got_text && !new_dir_name.isEmpty()) {
        if (!directory.exists(new_dir_name)) {
            directory.mkdir(new_dir_name);
            return;
        } else {
            QMessageBox::about(this, "Directory creation", "Directory already exists.");
        }
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
    if (file_name.isEmpty() && file_name.isNull()) {
         return;
    }
    QFile file(file_name);
    QFileInfo file_info(file_name);
    bool got_text;
    QString new_file_name = QInputDialog::getText(this, "Renaming file", "Enter new name:", QLineEdit::Normal, file_info.fileName(), &got_text);
    if (got_text && !new_file_name.isEmpty()) {
        QString new_name = QString("%1/%2").arg(file.fileName().section("/",0,-2), new_file_name);
        if(file.rename(new_name)) {
            return;
        } else {
            QMessageBox::about(this, "File renaming", "File not renamed.");
        }
    }
}

void MainWindow::on_actionRename_Directory_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory to rename", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name.isEmpty() && dir_name.isNull()) {
         return;
    }
    QDir directory(dir_name);
    bool got_text;
    QString new_dir_name = QInputDialog::getText(this, "Renaming directory", "Enter new name:", QLineEdit::Normal, directory.dirName(), &got_text);
    if (got_text && !new_dir_name.isEmpty()) {
        QString new_name = QString("../%1").arg(new_dir_name);
        if (directory.rename(directory.path(), new_name)) {
            return;
        } else {
            QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
        }
    }
}

void MainWindow::on_actionDelete_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select file to remove", "");
    if (file_name.isEmpty() && file_name.isNull()) {
         return;
    }
    QFileInfo file_info(file_name);
    QMessageBox::StandardButton delete_box;
    delete_box = QMessageBox::question(this, "Delete procedure", QString("You want to delete %1?").arg(file_info.fileName()),
                                       QMessageBox::No | QMessageBox::Yes);
    if (delete_box != QMessageBox::No) {
        if (QFile::remove(file_name)) {
            return;
        } else {
            QMessageBox::about(this, "File deleting", "File not deleted.");
        }
    }
}

void MainWindow::on_actionDelete_Directory_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory to delete", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name.isEmpty() && dir_name.isNull()) {
         return;
    }
    QDir directory(dir_name);
    QMessageBox::StandardButton delete_box;
    delete_box = QMessageBox::question(this, "Delete procedure", QString("You want to delete %1?").arg(directory.dirName()),
                                       QMessageBox::No | QMessageBox::Yes);
    if (delete_box != QMessageBox::No) {
        if (directory.removeRecursively()) {
            return;
        } else {
            QMessageBox::about(this, "Directory deleting", "Directory not deleted.");
        }
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
        return;
    } else {
        QMessageBox::about(this, "File copying", "File not copied.");
    }
}

void MainWindow::copyDir(QString &from, QString &to) {
    QDir from_dir(from);
    QDir to_dir(to);

    if (!to_dir.exists()) {
        to_dir.mkdir(to);
    }
    foreach (const QString &file, from_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        QString fromDirName = from + QDir::separator() + file;
        QString toDirName = to + QDir::separator() + file;
        copyDir(fromDirName, toDirName);
    }
    foreach (const QString &file, from_dir.entryList(QDir::Files)) {
        QString fromDirName = from + QDir::separator() + file;
        QString toDirName = to + QDir::separator() + file;
        QFile::copy(fromDirName, toDirName);
    }
}

void MainWindow::on_actionCopy_Directory_triggered()
{
    QString dir_name_from = QFileDialog::getExistingDirectory(this, "Select source directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dir_name_to = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir from_dir(dir_name_from);
    QString to_fullpath = dir_name_to + QDir::separator() + from_dir.dirName();
    QDir to_dir(to_fullpath);
    if (!to_dir.exists()) {
        to_dir.mkdir(to_fullpath);
    }
    copyDir(dir_name_from, to_fullpath);
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
            return;
        } else {
            QMessageBox::about(this, "File moving", "File not moved.");
        }
    }
}


void MainWindow::on_actionMove_Directory_triggered()
{
    QString dir_name_from = QFileDialog::getExistingDirectory(this, "Select source directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dir_name_to = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir from_dir(dir_name_from);
    QString to_fullpath = dir_name_to + QDir::separator() + from_dir.dirName();
    QDir to_dir(to_fullpath);
    if (!to_dir.exists()) {
        to_dir.mkdir(to_fullpath);
    }
    copyDir(dir_name_from, to_fullpath);
    if (from_dir.removeRecursively()) {
        return;
    } else {
        QMessageBox::about(this, "Directory deleting", "Source directory not deleted.");
    }
}



void MainWindow::on_copyButton_clicked()
{
    if (right_main) {
        QFile file(left_part_path);
        QFileInfo file_info(file);
        QFileInfo dir_info(right_part_path);
        if (file_info.isFile() && dir_info.isDir()) {
            QString file_copy_path = QString("%1/%2").arg(right_part_path, file.fileName().section("/", -1, -1));
            if (QFile::exists(file_copy_path)){
                QFile::remove(file_copy_path);
            }
            if (QFile::copy(left_part_path, file_copy_path)) {
                return;
            } else {
                QMessageBox::about(this, "File copying", "Something bad happened.");
            }
        } else if (file_info.isDir() && dir_info.isDir()){
            QDir from_dir(left_part_path);
            QString to_fullpath = right_part_path + QDir::separator() + from_dir.dirName();
            QDir to_dir(to_fullpath);
            if (!to_dir.exists()) {
                to_dir.mkdir(to_fullpath);
            }
            copyDir(left_part_path, to_fullpath);
        }
    } else {
        QFile file(right_part_path);
        QFileInfo file_info(file);
        QFileInfo dir_info(left_part_path);
        if (file_info.isFile() && dir_info.isDir()) {
            QString file_copy_path = QString("%1/%2").arg(left_part_path, file.fileName().section("/", -1, -1));
            if (QFile::exists(file_copy_path)){
                QFile::remove(file_copy_path);
            }
            if (QFile::copy(right_part_path, file_copy_path)) {
                return;
            } else {
                QMessageBox::about(this, "File copying", "Something bad happened.");
            }
        } else if (file_info.isDir() && dir_info.isDir()){
            QDir from_dir(right_part_path);
            QString to_fullpath = left_part_path + QDir::separator() + from_dir.dirName();
            QDir to_dir(to_fullpath);
            if (!to_dir.exists()) {
                to_dir.mkdir(to_fullpath);
            }
            copyDir(right_part_path, to_fullpath);
        }
    }

}

void MainWindow::on_moveButton_clicked()
{
    if (right_main) {
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
                   return;
                } else {
                    QMessageBox::about(this, "File moving", "Something bad happened.");
                }
            }
        } else if (file_info.isDir() && dir_info.isDir()){
            QDir from_dir(left_part_path);
            QString to_fullpath = right_part_path + QDir::separator() + from_dir.dirName();
            QDir to_dir(to_fullpath);
            if (!to_dir.exists()) {
                to_dir.mkdir(to_fullpath);
            }
            copyDir(left_part_path, to_fullpath);
            if (from_dir.removeRecursively()) {
                return;
            } else {
                QMessageBox::about(this, "Directory deleting", "Source directory not deleted.");
            }
        }
    } else {
        QFile file(right_part_path);
        QFileInfo file_info(file);
        QFileInfo dir_info(left_part_path);
        if (file_info.isFile() && dir_info.isDir()) {
            QString file_copy_path = QString("%1/%2").arg(left_part_path, file.fileName().section("/", -1, -1));
            if (QFile::exists(file_copy_path)){
                QFile::remove(file_copy_path);
            }
            if (QFile::copy(right_part_path, file_copy_path)) {
                if (QFile::remove(right_part_path)) {
                   return;
                } else {
                    QMessageBox::about(this, "File moving", "Something bad happened.");
                }
            }
        } else if (file_info.isDir() && dir_info.isDir()){
            QDir from_dir(right_part_path);
            QString to_fullpath = left_part_path + QDir::separator() + from_dir.dirName();
            QDir to_dir(to_fullpath);
            if (!to_dir.exists()) {
                to_dir.mkdir(to_fullpath);
            }
            copyDir(right_part_path, to_fullpath);
            if (from_dir.removeRecursively()) {
                return;
            } else {
                QMessageBox::about(this, "Directory deleting", "Source directory not deleted.");
            }
        }
    }

}

void MainWindow::on_renameButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
     QFile file(file_path);
     QFileInfo file_info(file);
     QFileInfo dir_info(left_part_path);
     bool got_text;
     if (file_info.isFile()) {
         QString new_file_name = QInputDialog::getText(this, "Renaming file", "Enter new name:", QLineEdit::Normal, file_info.fileName(), &got_text);
         if (got_text && !new_file_name.isEmpty()) {
             QString new_name = QString("%1/%2").arg(file.fileName().section("/",0,-2), new_file_name);
             if(file.rename(new_name)) {
                 return;
             } else {
                 QMessageBox::about(this, "File renaming", "File not renamed.");
             }
         }
     } else if (dir_info.isDir()) {
         QDir dir(file_path);
         QString new_dir_name = QInputDialog::getText(this, "Renaming directory", "Enter new name:", QLineEdit::Normal, dir.dirName(), &got_text);
         if (got_text && !new_dir_name.isEmpty()) {
             QString new_name = QString("../%1").arg(new_dir_name);
             QDir directory(left_part_path);
             if (directory.rename(directory.path(), new_name)) {
                 return;
             } else {
                 QMessageBox::about(this, "Directory renaming", "Directory not renamed.");
             }
         }
     }
}

void MainWindow::on_deleteButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    QFile file(file_path);
    QFileInfo file_info(file);
    QFileInfo dir_info(file_path);
    if (file_info.isFile()) {
        QMessageBox::StandardButton delete_box;
        delete_box = QMessageBox::question(this, "Delete procedure", QString("You want to delete %1?").arg(file_info.fileName()),
                                           QMessageBox::No | QMessageBox::Yes);
        if (delete_box != QMessageBox::No) {
            if (QFile::remove(file_path)) {
                return;
            } else {
                QMessageBox::about(this, "File deleting", "File not deleted.");
            }
        }

    } else if (dir_info.isDir()) {
        QMessageBox::StandardButton delete_box;
        delete_box = QMessageBox::question(this, "Delete procedure", QString("You want to delete %1?").arg(file_info.fileName()),
                                           QMessageBox::No | QMessageBox::Yes);
        if (delete_box != QMessageBox::No) {
            QDir directory(file_path);
            if (directory.removeRecursively()) {
                return;
            } else {
                QMessageBox::about(this, "Directory deleting", "Directory not deleted.");
            }
        }

    }
}

void MainWindow::on_actionSearch_triggered()
{
    searchDialog->show();
    searchDialog->activateWindow();
}

void MainWindow::on_actionExtract_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select archive to unzip", "");
    QFileInfo file_info(file_name);
    extract(file_name, file_info.absolutePath() + QDir::separator());
}

void MainWindow::on_actionExtract_to_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select archive to unzip", "");
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select destination directory", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    extract(file_name, dir_name + QDir::separator());
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

void MainWindow::on_actionShow_in_Terminal_triggered()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    QProcess* proc = new QProcess(this);
    proc->setWorkingDirectory(file_path);
#ifdef __APPLE__
    proc->setProgram("/Applications/Utilities/Terminal.app");
#else
    proc->setProgram("/usr/bin/gnome-terminal");
#endif
    proc->start();
    proc->waitForStarted();
}

void MainWindow::on_editButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    currUrl = QUrl::fromLocalFile(QFileInfo(file_path).absoluteFilePath());
    QDesktopServices::openUrl(currUrl);
}

void MainWindow::on_newFileButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    bool got_text;
    QString new_file_name = QInputDialog::getText(this, "Creating file", "Enter name:", QLineEdit::Normal, "untitled.txt", &got_text);
    if (new_file_name.isEmpty()) {
         return;
    }
    file_path += "/";
    file_path += new_file_name;
    QFile file(file_path);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    } else {
        QMessageBox::about(this, "File creation", "File already exists.");
    }
}

void MainWindow::on_newDirButton_clicked()
{
    QString file_path;
    if (right_main) {
        file_path = right_part_path;
    } else {
        file_path = left_part_path;}
    QDir directory(file_path);
    bool got_text;
    QString new_dir_name = QInputDialog::getText(this, "Creating directory", "Enter name:", QLineEdit::Normal, "untitled", &got_text);
    if (new_dir_name.isEmpty()) {
         return;
    }
    if (!directory.exists(new_dir_name)) {
        directory.mkdir(new_dir_name);
    }
}

void MainWindow::on_quitButton_clicked()
{
    QMessageBox::StandardButton exit_box;
    exit_box = QMessageBox::question(this, "Exit procedure", "You want to exit this beautiful file manager?", QMessageBox::No | QMessageBox::Yes);
    if (exit_box != QMessageBox::No) {
        QApplication::instance()->quit();
    }
}


void MainWindow::on_actionZip_Directory_triggered()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory to zip", "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir dir(dir_name);
    bool got_text;
    QString archive_name = QInputDialog::getText(this, "Compressing file/directory", "Enter new name:", QLineEdit::Normal,
                                                 dir.dirName() + ".zip", &got_text);
    if (got_text) {
        archive_folder(dir.absolutePath().section("/", 0, -2) + QDir::separator() + archive_name, dir_name + QDir::separator());
    }
}

void MainWindow::on_actionZip_File_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select fileto zip", "");
    QFile file(file_name);
    QFileInfo file_info(file);
    bool got_text;
    QString archive_name = QInputDialog::getText(this, "Compressing file/directory", "Enter new name:", QLineEdit::Normal,
                                                 file_info.fileName() + ".zip", &got_text);
    if (got_text) {
         archive_folder(file_info.absolutePath() + QDir::separator() + archive_name, file_name);
    }
}


void MainWindow::on_left_path_editingFinished()
{
    QString current_path = ui->left_path->text();
    right_main = false;
    QDir dir(current_path);
    if (dir.exists()) {
        ui->tableView_1->setRootIndex(dirmodel_1->setRootPath(current_path));
        ui->left_path->setText(current_path);
        left_part_path = current_path;
    }
}

void MainWindow::on_right_path_editingFinished()
{
    QString current_path = ui->right_path->text();
    right_main = true;
    QDir dir(current_path);
    if (dir.exists()) {
        ui->tableView_2->setRootIndex(dirmodel_2->setRootPath(current_path));
        ui->right_path->setText(current_path);
        right_part_path = current_path;
    }
}
