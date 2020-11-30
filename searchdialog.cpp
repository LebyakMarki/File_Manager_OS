#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include <QDirIterator>
#include <QMimeDatabase>
#include <QStringList>
#include <QtCore>
#include <QtGui>


QString SearchDialog::get_permission(QFileInfo &info) {
    QString result;
    if (info.permission(QFile::ReadOwner)) {
        result += "r";}
    if (!info.permission(QFile::ReadOwner)) {
        result += "-";}
    if(info.permission(QFile::WriteOwner)) {
        result += "w";}
    if(!info.permission(QFile::WriteOwner)) {
        result += "-";}
    if(info.permission(QFile::ExeOwner)) {
        result += "x";}
    if(!info.permission(QFile::ExeOwner)) {
        result += "-";}
    if(info.permission(QFile::ReadGroup)) {
        result += "r";}
    if(!info.permission(QFile::ReadGroup)) {
        result += "-";}
    if(info.permission(QFile::WriteGroup)) {
        result += "w";}
    if(!info.permission(QFile::WriteGroup)) {
        result += "-";}
    if(info.permission(QFile::ExeGroup)) {
        result += "x";}
    if(!info.permission(QFile::ExeGroup)) {
        result += "-";}
    if(info.permission(QFile::ReadOther)) {
        result += "r";}
    if(!info.permission(QFile::ReadOther)) {
        result += "-";}
    if(info.permission(QFile::WriteOther)) {
        result += "w";}
    if(!info.permission(QFile::WriteOther)) {
        result += "-";}
    if(info.permission(QFile::ExeOther)) {
        result += "x";}
    if(!info.permission(QFile::ExeOther)) {
        result += "-";
    }
    return result;
}


SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    ui->fileNameBox->setCurrentText("");
    ui->containing_text_box->setCurrentText("");
    ui->resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->resultsTable->verticalHeader()->setVisible(false);
    ui->resultsTable->setShowGrid(false);
    ui->resultsTable->setAlternatingRowColors(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_start_searching_button_clicked()
{
    ui->resultsTable->setRowCount(0);
    QStringList filter;
    QString filenameSearch = ui->fileNameBox->currentText();
    if (filenameSearch == "" || filenameSearch.isEmpty()) {
        return;
    } else {
        ui->fileNameBox->addItem(filenameSearch);
        filter << "*" + filenameSearch + "*";
    }
    QString textSearch = ui->containing_text_box->currentText();
    QStringList foundFiles;
    QString type = ui->fileTypeBox->currentText();

    if (browsePaths.isEmpty()) {
        QString homeDir = QDir::homePath();
        QString path = QDir::cleanPath(homeDir);
        currentDir = QDir(path);
        QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            if (ui->caseSensitiveFilenameBox->isChecked()) {
                if (it.next().contains(filenameSearch, Qt::CaseSensitive)) {
                    if (type == "All") {
                        foundFiles << it.next();
                    }
                }
            } else {
                if (type == "All") {
                    foundFiles << it.next();
                }

            }
        }
    } else {
        for (const auto&dir : browsePaths) {
            QString path = QDir::cleanPath(dir);
            currentDir = QDir(path);

            QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                if (ui->caseSensitiveFilenameBox->isChecked()) {
                    if (it.next().contains(filenameSearch, Qt::CaseSensitive)) {
                        if (type == "All") {
                            foundFiles << it.next();
                        }
                    }
                } else {
                    if (type == "All") {
                        foundFiles << it.next();
                    }
                }
            }
        }
    }
    if (textSearch != "" && !textSearch.isEmpty()) {
        foundFiles = findFilesByText(foundFiles, textSearch, ui->caseSensitiveContainingTextBox->isChecked());
        ui->containing_text_box->addItem(textSearch);
    }
    foundFiles.toSet().toList();
    QSet<QString> setFiles = QSet<QString>::fromList(foundFiles);
    foundFiles = setFiles.toList();
    foundFiles.sort();
    showFoundFiles(foundFiles);
}

enum { absoluteFileNameRole = Qt::UserRole + 1 };
void SearchDialog::showFoundFiles(const QStringList &foundPaths) {
    for (const QString &filePath : foundPaths) {
           const QString toolTip = QDir::toNativeSeparators(filePath);
           const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
           const qint64 size = QFileInfo(filePath).size();

           QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
           fileNameItem->setData(absoluteFileNameRole, QVariant(filePath));
           fileNameItem->setToolTip(toolTip);
           fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);

           QTableWidgetItem *sizeItem = new QTableWidgetItem(QLocale().formattedDataSize(size));
           sizeItem->setData(absoluteFileNameRole, QVariant(filePath));
           sizeItem->setToolTip(toolTip);
           sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
           sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

           QFileInfo info(filePath);
           QString last_modified = info.lastModified().toString();
           QTableWidgetItem *lastModifiedItem = new QTableWidgetItem(last_modified);
           lastModifiedItem->setData(absoluteFileNameRole, QVariant(filePath));
           lastModifiedItem->setToolTip(toolTip);
           lastModifiedItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);

           QString rwx_permissions = get_permission(info);
           QTableWidgetItem *rwxItem = new QTableWidgetItem(rwx_permissions);
           rwxItem->setData(absoluteFileNameRole, QVariant(filePath));
           rwxItem->setToolTip(toolTip);
           rwxItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);

           int row = ui->resultsTable->rowCount();
           ui->resultsTable->insertRow(row);
           ui->resultsTable->setItem(row, 0, fileNameItem);
           ui-> resultsTable->setItem(row, 1, sizeItem);
           ui-> resultsTable->setItem(row, 2, rwxItem);
           ui-> resultsTable->setItem(row, 3, lastModifiedItem);
       }
    ui->how_many_files_found->setText(QString("%1 file(-s) found.").arg(foundPaths.size()));
    ui->how_many_files_found->setWordWrap(true);
}


QStringList SearchDialog::findFilesByText(const QStringList &files, const QString &text, const bool &caseSensitive) {
    QMimeDatabase mimeData;
    QStringList filesContainingText;
    for (int i = 0; i < files.size(); i++) {
        QCoreApplication::processEvents();
        const QString filename = files.at(i);
        const QMimeType mimeType = mimeData.mimeTypeForFile(filename);
        if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
            continue;
        }
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            QString line;
            QTextStream in(&file);
            while (!in.atEnd()) {
                line = in.readLine();
                if (caseSensitive) {
                    if (line.contains(text, Qt::CaseSensitive)) {
                        filesContainingText << files[i];
                        break;
                    }
                } else {
                    if (line.contains(text, Qt::CaseInsensitive)) {
                        filesContainingText << files[i];
                        break;
                    }
                }
            }
        }
    }
    return filesContainingText;
}

void SearchDialog::showBrowsedPaths() {
    model = new QStringListModel(this);
    model->setStringList(browsePaths);
    ui->search_in_list->setModel(model);
    ui->search_in_list->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
}

void SearchDialog::on_browse_search_in_button_clicked()
{
    QString dir_name = QFileDialog::getExistingDirectory(this, "Select directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name != "") {
        browsePaths.append(dir_name);
    }
    showBrowsedPaths();
}

void SearchDialog::on_reset_search_in_button_clicked()
{
    browsePaths = QStringList();
    showBrowsedPaths();
}

void SearchDialog::on_resultsTable_cellClicked(int row, int column)
{
    currentUrl = QUrl::fromLocalFile(ui->resultsTable->item(row, 0)->data(absoluteFileNameRole).toString());
}

void SearchDialog::on_open_found_file_clicked()
{
    QDesktopServices::openUrl(currentUrl);
}
