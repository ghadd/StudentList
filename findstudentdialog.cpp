#include "findstudentdialog.h"
#include "ui_findstudentdialog.h"
#include <QDebug>
#include <QIcon>

FindStudentDialog::FindStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindStudentDialog)
{
    ui->setupUi(this);
    this->windowTitle() = "Finding student...";
    this->setWindowIcon(QIcon(tr("C:\\Users\\User\\Desktop\\logo.bmp")));
}

FindStudentDialog::~FindStudentDialog()
{
    delete ui;
}

void FindStudentDialog::accept(){
    this->name = ui->lineEdit->text();
    this->surname = ui->lineEdit_2->text();
    QDialog::accept();
}
