#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QDir>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchDialog;
}
QT_END_NAMESPACE

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_start_searching_button_clicked();
    void on_browse_search_in_button_clicked();
    void on_reset_search_in_button_clicked();
    void showBrowsedPaths();
    QStringList findFilesByText(const QStringList &files, const QString &text, const bool &caseSensitive);
    void showFoundFiles(const QStringList &foundPaths);
    QString get_permission(QFileInfo &info);

    void on_resultsTable_cellClicked(int row, int column);

    void on_open_found_file_clicked();

private:
    Ui::SearchDialog *ui;
    QStringListModel *model;
    QStringList browsePaths;
    QDir currentDir;
    QUrl currentUrl;
};

#endif // SEARCHDIALOG_H
