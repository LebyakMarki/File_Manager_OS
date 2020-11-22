#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

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

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
