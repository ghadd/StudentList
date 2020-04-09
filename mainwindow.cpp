#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Student_db.hpp"
#include <QMessageBox>

const short NGRADES = 5;
std::list<Student> students;
bool running = true;

#define MEMCHECK 0

void closeDB(std::list<Student>&);

template<typename T>
double avg(std::vector<T> vec) {
    return (double)std::accumulate(vec.begin(), vec.end(), 0) / vec.size();
}

int countOf2(std::vector<int> vec) {
    return std::count(vec.begin(), vec.end(), 2);
}

void delete_losers(std::list<Student>& students) {
    for (std::list<Student>::iterator it = students.begin(); it != students.end(); it = ++it) {
        if (countOf2((*it).get_grades()) == 2) {
            std::list<Student>::iterator to_delete = it++;
            students.erase(to_delete);
            it--;
        }
    }
}

void insert_sorted(std::list<Student>& students, std::string name, \
    std::string surname, std::string str_date, \
    std::vector<int> grades) {
    Student to_insert;
    to_insert.set_credentials(name, surname);
    to_insert.set_date(str_date);
    to_insert.set_grades(grades);

    if (std::find_if(grades.begin(), grades.end(), [](int x) -> bool {return x > 5 || x < 1; }) != grades.end()) {
        std::cout << "Skipping student with wrong grades set" << std::endl;
        return;
    }

    if (students.empty()) {
        students.push_back(to_insert);
    }
    else if (avg(grades) > avg(students.front().get_grades())) {
        students.push_front(to_insert);
    }
    else {
        unsigned pos;
        for (pos = 0; pos < students.size(); pos++) {
            auto it = std::next(students.begin(), pos);
            std::vector<int>& it_grades = (*it).get_grades();
            if (avg(it_grades) < avg(grades)) break;
        }
        students.insert(std::next(students.begin(), pos), to_insert);
    }
}

template <typename T, typename U>
void parseNumeralInput(const T& from, U& to) {
    try {
        to = boost::lexical_cast<U>(from);
    }
    catch (const boost::bad_lexical_cast&) {
        std::cout << "Invalid numeral. Commiting previous changes..." << std::endl;
        closeDB(students);
        exit(7);
    }
}

void readStudents(std::list<Student>& students, const std::string filename) {
    std::string strbuffer;
    char chrbuffer;
    std::string name, surname, str_date;
    if (filename != "kb") {
        int numberOfLines = getLines(filename);
        std::fstream file(filename, std::ios_base::in);
        if (!file.is_open()){
            throw(123);
            return;
        }
        for (int i = 0; i < numberOfLines; i++) {
            file >> surname >> name >> str_date;
            std::vector<int> buffer_grades(5);
            int grade;
            for (int j = 0; j < 5; j++) {
                file >> grade;
                buffer_grades[j] = grade;
            }
            insert_sorted(students, name, surname, str_date, buffer_grades);
        }
        file.close();
    }
    else {
        unsigned nStudents;
        std::cout << "Enter number of students to add: ";
        std::cin >> strbuffer;
        parseNumeralInput(strbuffer, nStudents);
        for (unsigned i = 0; i < nStudents; i++) {
            std::cin >> surname >> name >> str_date;
            std::vector<int> buffer_grades(5);
            int grade;
            for (int j = 0; j < 5; j++) {
                std::cin >> chrbuffer;
                if (chrbuffer == ' ') {
                    j--;
                    continue;
                }
                parseNumeralInput(chrbuffer, grade);
                buffer_grades[j] = grade;
            }

            insert_sorted(students, name, surname, str_date, buffer_grades);
        }
    }
}

void deleteStudentFromListAndDB(std::list<Student>& students, std::string name, std::string surname) {
    std::list<Student>::iterator it = students.begin();
    while ((*it).get_credentials().name != name && (*it).get_credentials().surname != surname) {
        it++;
    }
    students.erase(it); // from the list
    deleteStudent(name, surname); // from the db
}

void loadDB(std::list<Student>& students) {
    readStudents(students, DB_PATH);
}

void closeDB(std::list<Student>& students) {

    MEMCHECK;

    std::fstream file(DB_PATH, std::ios_base::out);

    for (auto& st : students) {
        if (!containsStudent(st.get_credentials().name, st.get_credentials().surname)) {
            addStudent(&st);
        }
    }

    file.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lwStudents->resize(this->size().width() * (0.5 - 0.0026 * (this->size().width() - 600) / 10), this->height());
    ui->lwStudents->setEnabled(false);
    this->setMinimumSize(QSize(690, 400));
    this->setMaximumSize(QSize(800, 500));

    ui->label->setText("Lorem ipsum dolor sit amet, \n\n"
                       "consectetur adipiscing elit, \n\n"
                       "sed do eiusmod tempor incididunt \n\n"
                       "ut labore et dolore magna aliqua.\n\n"
                       "Ut enim ad minim veniam, quis \n\n"
                       "nostrud exercitation ullamco \n\n"
                       "laboris nisi ut aliquip ex ea \n\n"
                       "commodo consequat. Duis aute irure\n\n"
                       "dolor in reprehenderit in voluptate\n\n"
                       "velit esse cillum dolore eu fugiat\n\n"
                       "nulla pariatur. Excepteur sint\n\n"
                       "occaecat cupidatat non proident,\n\n"
                       "sunt in culpa qui officia deserunt\n\n"
                       "mollit anim id est laborum.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << event->size();
    ui->lwStudents->resize(event->size().width() * (0.5 - 0.0026 * (event->size().width() - 600) / 10), this->height());
    ui->label->setGeometry(ui->lwStudents->geometry().x() + 15 + ui->lwStudents->width(), 50, this->width() - 15 - ui->lwStudents->width(), this->height() - 50);
    ui->lLogo->setGeometry(ui->lwStudents->geometry().x() + 15 + ui->lwStudents->width(), 5, this->width() - 15 - ui->lwStudents->width(), ui->lLogo->height());
    update();
}

void MainWindow::displayStudents()
{
    qDebug() << students.size();
    for (auto& student : students){
        qDebug() << student.toQString();
        ui->lwStudents->addItem(student.toQString());
    }
}

void MainWindow::on_actionLoad_Database_triggered()
{
    ui->lwStudents->setEnabled(true);
    loadDB(students);
    displayStudents();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionLoad_triggered()
{
    if (!ui->lwStudents->isEnabled()){
        QMessageBox::warning(this, "Hey, there", "Seems like you didn't load database");
        return;
    }
    QFileDialog *dialog = new QFileDialog();
    dialog->setNameFilter(tr("Text (*.txt, *.rtf, *.csv)"));
    QString filename = dialog->getOpenFileName();

    qDebug() << filename;

    if (!filename.isNull()){
        try {
            readStudents(students, filename.toStdString());
            ui->lwStudents->clear();
            displayStudents();
        } catch (...) {
            QMessageBox::warning(this, "Hey, there", "Seems like your file is garbage. Try some other format.");
        }
    }
}

void MainWindow::on_actionFind_triggered()
{
    dialog = new FindStudentDialog();
    dialog->show();

    connect(dialog, SIGNAL(accepted()), this, SLOT(displayres()));
}

void MainWindow::displayres()
{
    QMessageBox::information(this, "Query result", getStudent(dialog->name, dialog->surname));
}

void MainWindow::on_actionDelete_losers_triggered()
{
    delete_losers(students);
    ui->lwStudents->clear();
    displayStudents();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    qDebug() << "saving db";
    this->setWindowTitle(this->windowTitle() + "/" + "Saving db...");
    if (ui->lwStudents->isEnabled()){
        closeDB(students);
    }
    QApplication::quit();
}
