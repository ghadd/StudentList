#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include <fstream>
#include <QDebug>
#include <QResizeEvent>
#include <QFileDialog>
#include "findstudentdialog.h"

enum mode {
    _displayAll,
    _deleteStudent,
    _findStudent,
    _deleteLosers,
    _readFromFile,
    _readFromKb,
    _commitChanges
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void displayStudents();

private slots:
    void on_actionLoad_Database_triggered();

    void on_actionExit_triggered();

    void on_actionLoad_triggered();

    void on_actionFind_triggered();

    void displayres();

    void on_actionDelete_losers_triggered();

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::MainWindow *ui;
    FindStudentDialog *dialog;
};
#endif // MAINWINDOW_H
