#ifndef FINDSTUDENTDIALOG_H
#define FINDSTUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class FindStudentDialog;
}

class FindStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindStudentDialog(QWidget *parent = nullptr);
    QString name = "";
    QString surname = "";
    ~FindStudentDialog();

signals:
    void accept() override;


private:
    Ui::FindStudentDialog *ui;
};

#endif // FINDSTUDENTDIALOG_H
