#include "simulation.h"
#include "ui_simulation.h"
#include "QFileDialog"
#include "QMessageBox"
#include "stack"
#include "set"
#include "cfloat"
#include "QTimer"

#define Y_AXIS 20
#define X_AXIS 20

std::vector<std::tuple<int, int, int>> objects, objectsCpy, objectsShortest;
std::vector<std::pair<int, int>> offset{{-1, -1}, {1, 0},  {0, 1}, {-1, 0}, {0, -1},  {-1, 1}, {1, 1}, {1, -1}};
std::vector<std::pair<int, int>> outerBox{{-2, 0}, {-2, -1},  {-2, 1}, {2, 0}, {2, -1},  {2, 1}, {1, 2}, {-1, 2}, {0, 2}, {1, -2}, {-1, -2}, {0, -2}, {-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
std::vector<char> steps;
int currDir = 1;
int robotDir = 1;
std::pair<int, int> currPos{18, 1};
int globalN = 0;
bool haveRoute = false;

struct cell {
    int pi, pj;
    double f, g, h;
};

Simulation::Simulation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Simulation)
{
    ui->setupUi(this);
    ui->g19_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g19_1->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g19_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_1->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g17_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g17_1->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
    ui->g17_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
}

Simulation::~Simulation()
{
    delete ui;
}

void Simulation::moveF(std::pair<int, int> curr) {
    if (robotDir == 1) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first - 2); btn10 += "_"; btn10 += QString::number(curr.second - 1);
        QString btn11 = "g"; btn11 += QString::number(curr.first - 2); btn11 += "_"; btn11 += QString::number(curr.second);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 2); btn12 += "_"; btn12 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn8)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn1)->setStyleSheet("");
        this->findChild<QPushButton *>(btn2)->setStyleSheet("");
        this->findChild<QPushButton *>(btn3)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.first -= 1;
    } else if (robotDir == 2) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 1); btn10 += "_"; btn10 += QString::number(curr.second + 2);
        QString btn11 = "g"; btn11 += QString::number(curr.first); btn11 += "_"; btn11 += QString::number(curr.second + 2);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 1); btn12 += "_"; btn12 += QString::number(curr.second + 2);
        this->findChild<QPushButton *>(btn6)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn1)->setStyleSheet("");
        this->findChild<QPushButton *>(btn4)->setStyleSheet("");
        this->findChild<QPushButton *>(btn7)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.second += 1;
    } else if (robotDir == 3) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 2); btn10 += "_"; btn10 += QString::number(curr.second - 1);
        QString btn11 = "g"; btn11 += QString::number(curr.first + 2); btn11 += "_"; btn11 += QString::number(curr.second);
        QString btn12 = "g"; btn12 += QString::number(curr.first + 2); btn12 += "_"; btn12 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn2)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn7)->setStyleSheet("");
        this->findChild<QPushButton *>(btn8)->setStyleSheet("");
        this->findChild<QPushButton *>(btn9)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.first += 1;
    } else if (robotDir == 4) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 1); btn10 += "_"; btn10 += QString::number(curr.second - 2);
        QString btn11 = "g"; btn11 += QString::number(curr.first); btn11 += "_"; btn11 += QString::number(curr.second - 2);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 1); btn12 += "_"; btn12 += QString::number(curr.second - 2);
        this->findChild<QPushButton *>(btn4)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn3)->setStyleSheet("");
        this->findChild<QPushButton *>(btn6)->setStyleSheet("");
        this->findChild<QPushButton *>(btn9)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.second -= 1;
    }
}

void Simulation::moveB(std::pair<int, int> curr) {
    if (robotDir == 1) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 2); btn10 += "_"; btn10 += QString::number(curr.second - 1);
        QString btn11 = "g"; btn11 += QString::number(curr.first + 2); btn11 += "_"; btn11 += QString::number(curr.second);
        QString btn12 = "g"; btn12 += QString::number(curr.first + 2); btn12 += "_"; btn12 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn5)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn7)->setStyleSheet("");
        this->findChild<QPushButton *>(btn8)->setStyleSheet("");
        this->findChild<QPushButton *>(btn9)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.first += 1;
    } else if (robotDir == 2) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 1); btn10 += "_"; btn10 += QString::number(curr.second - 2);
        QString btn11 = "g"; btn11 += QString::number(curr.first); btn11 += "_"; btn11 += QString::number(curr.second - 2);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 1); btn12 += "_"; btn12 += QString::number(curr.second - 2);
        this->findChild<QPushButton *>(btn5)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn3)->setStyleSheet("");
        this->findChild<QPushButton *>(btn6)->setStyleSheet("");
        this->findChild<QPushButton *>(btn9)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.second -= 1;
    } else if (robotDir == 3) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first - 2); btn10 += "_"; btn10 += QString::number(curr.second - 1);
        QString btn11 = "g"; btn11 += QString::number(curr.first - 2); btn11 += "_"; btn11 += QString::number(curr.second);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 2); btn12 += "_"; btn12 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn5)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn1)->setStyleSheet("");
        this->findChild<QPushButton *>(btn2)->setStyleSheet("");
        this->findChild<QPushButton *>(btn3)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.first -= 1;
    } else if (robotDir == 4) {
        QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
        QString btn10 = "g"; btn10 += QString::number(curr.first + 1); btn10 += "_"; btn10 += QString::number(curr.second + 2);
        QString btn11 = "g"; btn11 += QString::number(curr.first); btn11 += "_"; btn11 += QString::number(curr.second + 2);
        QString btn12 = "g"; btn12 += QString::number(curr.first - 1); btn12 += "_"; btn12 += QString::number(curr.second + 2);
        this->findChild<QPushButton *>(btn5)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn1)->setStyleSheet("");
        this->findChild<QPushButton *>(btn4)->setStyleSheet("");
        this->findChild<QPushButton *>(btn7)->setStyleSheet("");
        this->findChild<QPushButton *>(btn10)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn11)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn12)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        currPos.second += 1;
    }
}

void Simulation::moveL(std::pair<int, int> curr) {
    if (robotDir == 1) {
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        this->findChild<QPushButton *>(btn8)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn4)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        robotDir = 4;
    } else if (robotDir == 2) {
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        this->findChild<QPushButton *>(btn8)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn6)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 1;
    } else if (robotDir == 3) {
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn6)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn2)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 2;
    } else if (robotDir == 4) {
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        this->findChild<QPushButton *>(btn4)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        this->findChild<QPushButton *>(btn2)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        robotDir = 3;
    }
}

void Simulation::moveR(std::pair<int, int> curr) {
    if (robotDir == 1) {

        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn6)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn8)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 2;
    } else if (robotDir == 2) {
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
        this->findChild<QPushButton *>(btn2)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn6)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 3;
    } else if (robotDir == 3) {
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
        this->findChild<QPushButton *>(btn4)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn2)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 4;
    } else if (robotDir == 4) {
        QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
        QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
        this->findChild<QPushButton *>(btn8)->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
        this->findChild<QPushButton *>(btn4)->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
        robotDir = 1;
    }
}

void tracePath(std::vector<std::vector<cell>> details, std::pair<int, int> dest, int dir) {
  int row = dest.first;
  int col = dest.second;

  std::stack<std::pair<int, int>> Path;

  while (!(details[row][col].pi == row && details[row][col].pj == col)) {
    Path.push({row, col});
    int temp_row = details[row][col].pi;
    int temp_col = details[row][col].pj;
    row = temp_row;
    col = temp_col;
  }

  Path.push({row, col});
  std::pair<int, int> start{0, 0};
  while (!Path.empty()) {
    std::pair<int, int> p = Path.top();
    Path.pop();
    std::pair<int, int> direction{p.first - start.first,
                                  p.second - start.second};
    if (direction == offset[3]) {
      if (currDir == 1) {
        steps.push_back('f');
      } else if (currDir == 2) {
        currDir = 1;
        steps.push_back('l');
        steps.push_back('f');
      } else if (currDir == 3) {
        steps.push_back('b');
      } else if (currDir == 4) {
        currDir = 1;
        steps.push_back('r');
        steps.push_back('f');
      }
    } else if (direction == offset[2]) {
      if (currDir == 1) {
        currDir = 2;
        steps.push_back('r');
        steps.push_back('f');
      } else if (currDir == 2) {
        steps.push_back('f');
      } else if (currDir == 3) {
        currDir = 2;
        steps.push_back('l');
        steps.push_back('f');
      } else if (currDir == 4) {
        steps.push_back('b');
      }
    } else if (direction == offset[1]) {
      if (currDir == 1) {
        steps.push_back('b');
      } else if (currDir == 2) {
        currDir = 3;
        steps.push_back('r');
        steps.push_back('f');
      } else if (currDir == 3) {
        steps.push_back('f');
      } else if (currDir == 4) {
        currDir = 3;
        steps.push_back('l');
        steps.push_back('f');
      }
    } else if (direction == offset[4]) {
      if (currDir == 1) {
        currDir = 4;
        steps.push_back('l');
        steps.push_back('f');
      } else if (currDir == 2) {
        steps.push_back('b');
      } else if (currDir == 3) {
        currDir = 4;
        steps.push_back('r');
        steps.push_back('f');
      } else if (currDir == 4) {
        steps.push_back('f');
      }
    }
    start = p;
    if (Path.empty()) {
      switch (dir) {
        case 1:
          if (currDir == 1) {
            steps.push_back('r');
            steps.push_back('r');
          } else if (currDir == 2) {
            steps.push_back('r');
          } else if (currDir == 4) {
            steps.push_back('l');
          }
          currDir = 3;
          break;
        case 2:
          if (currDir == 1) {
            steps.push_back('l');
          } else if (currDir == 2) {
            steps.push_back('l');
            steps.push_back('l');
          } else if (currDir == 3) {
            steps.push_back('r');
          }
          currDir = 4;
          break;
        case 3:
          if (currDir == 2) {
            steps.push_back('l');
          } else if (currDir == 3) {
            steps.push_back('l');
            steps.push_back('l');
          } else if (currDir == 4) {
            steps.push_back('r');
          }
          currDir = 1;
          break;
        case 4:
          if (currDir == 1) {
            steps.push_back('r');
          } else if (currDir == 4) {
            steps.push_back('r');
            steps.push_back('r');
          } else if (currDir == 3) {
            steps.push_back('l');
          }
          currDir = 2;
          break;
      }
    }
  }
}

bool isValid(int x, int y) {
  return (x >= 0) && (x < X_AXIS) && (y >= 0) && (y < Y_AXIS);
}

bool doesNotHit(std::vector<std::vector<char>>& grid, int x, int y) {
  int n = offset.size();
  bool trigger = false;
  for (int i = 0; i < n; i++) {
    int newX = x + offset[i].first;
    int newY = y + offset[i].second;
    if (!isValid(newX, newY) || grid[newX][newY] == 'O') {
      trigger = true;
      break;
    }
  }
  return (trigger ? false : true);
}

bool additionalSpace(std::vector<std::vector<char>>& grid, int x, int y) {
    int n = outerBox.size();
    bool trigger = false;
    for (int i = 0; i < n; i++) {
        int newX = x + outerBox[i].first;
        int newY = y + outerBox[i].second;
        if (isValid(newX, newY) && grid[newX][newY] == 'O') {
            trigger = true;
            break;
        }
    }
    return (trigger ? false : true);
}

bool isDestination(int x, int y, std::pair<int, int> dest) {
  std::pair<int, int> temp{x, y};
  if (temp == dest) return true;
  return false;
}

double calHueristic(std::pair<int, int> cell, std::pair<int, int> dest) {
  double first = abs(cell.first - dest.first);
  double second = abs(cell.second - dest.second);
  return first + second;
}

void search(std::vector<std::vector<char>>& grid, std::pair<int, int> src,
            std::pair<int, int> dest, int dir) {
  if (!isValid(src.first, src.second)) return;
  if (!isValid(dest.first, dest.second)) return;
  std::vector<std::vector<bool>> closedList(20, std::vector<bool>(20, false));
  std::vector<std::vector<cell>> details(20, std::vector<cell>(20));
  int i, j;
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 20; j++) {
      details[i][j].f = FLT_MAX;
      details[i][j].g = FLT_MAX;
      details[i][j].h = FLT_MAX;
      details[i][j].pi = -1;
      details[i][j].pj = -1;
    }
  }
  i = src.first, j = src.second;
  details[i][j].f = 0.0;
  details[i][j].g = 0.0;
  details[i][j].h = 0.0;
  details[i][j].pi = i;
  details[i][j].pj = j;

  std::set<std::pair<double, std::pair<int, int>>> openList;
  openList.insert({0.0, {i, j}});

  while (!openList.empty()) {
    std::pair<double, std::pair<int, int>> p = *openList.begin();
    openList.erase(openList.begin());

    i = p.second.first;
    j = p.second.second;
    closedList[i][j] = true;

    double gNew, hNew, fNew;
    if (isValid(i - 1, j) && doesNotHit(grid, i - 1, j) && additionalSpace(grid, i - 1, j)) {
      if (isDestination(i - 1, j, dest)) {
        details[i - 1][j].pi = i;
        details[i - 1][j].pj = j;
        tracePath(details, dest, dir);
        return;
      } else if (!closedList[i - 1][j]) {
        gNew = details[i][j].g + 1.0;
        hNew = calHueristic({i - 1, j}, dest);
        fNew = gNew + hNew;

        if (details[i - 1][j].f == FLT_MAX || details[i - 1][j].f > fNew) {
          openList.insert({fNew, {i - 1, j}});

          details[i - 1][j].f = fNew;
          details[i - 1][j].g = gNew;
          details[i - 1][j].h = hNew;
          details[i - 1][j].pi = i;
          details[i - 1][j].pj = j;
        }
      }
    }

    if (isValid(i + 1, j) && doesNotHit(grid, i + 1, j) && additionalSpace(grid, i + 1, j)) {
      if (isDestination(i + 1, j, dest)) {
        details[i + 1][j].pi = i;
        details[i + 1][j].pj = j;
        tracePath(details, dest, dir);
        return;
      } else if (!closedList[i + 1][j]) {
        gNew = details[i][j].g + 1.0;
        hNew = calHueristic({i + 1, j}, dest);
        fNew = gNew + hNew;

        if (details[i + 1][j].f == FLT_MAX || details[i + 1][j].f > fNew) {
          openList.insert({fNew, {i + 1, j}});

          details[i + 1][j].f = fNew;
          details[i + 1][j].g = gNew;
          details[i + 1][j].h = hNew;
          details[i + 1][j].pi = i;
          details[i + 1][j].pj = j;
        }
      }
    }

    if (isValid(i, j - 1) && doesNotHit(grid, i, j - 1) && additionalSpace(grid, i, j - 1)) {
      if (isDestination(i, j - 1, dest)) {
        details[i][j - 1].pi = i;
        details[i][j - 1].pj = j;
        tracePath(details, dest, dir);
        return;
      } else if (!closedList[i][j - 1]) {
        gNew = details[i][j].g + 1.0;
        hNew = calHueristic({i, j - 1}, dest);
        fNew = gNew + hNew;

        if (details[i][j - 1].f == FLT_MAX || details[i][j - 1].f > fNew) {
          openList.insert({fNew, {i, j - 1}});

          details[i][j - 1].f = fNew;
          details[i][j - 1].g = gNew;
          details[i][j - 1].h = hNew;
          details[i][j - 1].pi = i;
          details[i][j - 1].pj = j;
        }
      }
    }

    if (isValid(i, j + 1) && doesNotHit(grid, i, j + 1) && additionalSpace(grid, i, j + 1)) {
      if (isDestination(i, j + 1, dest)) {
        details[i][j + 1].pi = i;
        details[i][j + 1].pj = j;
        tracePath(details, dest, dir);
        return;
      } else if (!closedList[i][j + 1]) {
        gNew = details[i][j].g + 1.0;
        hNew = calHueristic({i, j + 1}, dest);
        fNew = gNew + hNew;

        if (details[i][j + 1].f == FLT_MAX || details[i][j + 1].f > fNew) {
          openList.insert({fNew, {i, j + 1}});

          details[i][j + 1].f = fNew;
          details[i][j + 1].g = gNew;
          details[i][j + 1].h = hNew;
          details[i][j + 1].pi = i;
          details[i][j + 1].pj = j;
        }
      }
    }
  }
}

std::string getLeastTurns() {
  std::vector<std::vector<std::tuple<int, int, int>>> permutations;
  std::vector<std::string> paths;
  int objectCount = objects.size();
  for (int i = 0; i < objectCount; i++) {
    int zero = std::get<0>(objects[i]);
    int one = std::get<1>(objects[i]);
    int two = std::get<2>(objects[i]);
    if (two == 1) {
      objects[i] = {zero - 4, one, 1};
    } else if (two == 2) {
      objects[i] = {zero, one + 4, 2};
    } else if (two == 3) {
      objects[i] = {zero + 4, one, 3};
    } else if (two == 4) {
      objects[i] = {zero, one - 4, 4};
    }
  }
  sort(objects.begin(), objects.end());
  do {
    permutations.push_back(objects);
  } while (std::next_permutation(objects.begin(), objects.end()));
  int n = permutations.size();
  std::vector<std::vector<char>> grid(20, std::vector<char>(20, ' '));
  for (int i = 0; i < objectCount; i++) {
    grid[std::get<0>(objectsCpy[i])][std::get<1>(objectsCpy[i])] = 'O';
  }
  for (int j = 0; j < n; j++) {
    std::vector<std::tuple<int, int, int>> currObject = permutations[j];
    std::pair<int, int> start{18, 1}, end;
    for (int i = 0; i < objectCount; i++) {
      int zero = std::get<0>(currObject[i]);
      int one = std::get<1>(currObject[i]);
      int two = std::get<2>(currObject[i]);
      end = std::make_pair(zero, one);
      search(grid, start, end, two);
      steps.push_back('s');
      start = end;
    }
    std::string path = "";
    for (char x : steps) path += x;
    currDir = 1;
    steps.clear();
    paths.push_back(path);
  }
  int currLowestTurn = INT_MAX;
  std::string lowestPath = "";
  int index = 0;
  for (int i = 0; i < (int)paths.size(); i++) {
    int currPathCount = paths[i].size();
    int turns = 0;
    for (int j = 0; j < currPathCount; j++) {
      if (paths[i][j] == 'r' || paths[i][j] == 'l') turns++;
    }
    if (turns < currLowestTurn) {
      currLowestTurn = turns;
      lowestPath = paths[i];
      index = i;
    }
  }
  objectsShortest = permutations[index];
  return lowestPath;
}

std::string getClosestObjectBasedOnOriginal() {
  int objectCount = objects.size();
  std::vector<std::vector<char>> grid(20, std::vector<char>(20, ' '));
  for (int i = 0; i < objectCount; i++) {
    grid[std::get<0>(objectsCpy[i])][std::get<1>(objectsCpy[i])] = 'O';
  }
  std::vector<std::tuple<int, int, int>> currObject;
  std::pair<int, int> curr{18, 1};
  while ((int) currObject.size() != objectCount) {
    int closest = INT_MAX, candidate;
    for (int i = 0; i < objectCount; i++) {
      if (std::find(currObject.begin(), currObject.end(), objectsCpy[i]) !=
          currObject.end())
        continue;
      int x_dist = abs(std::get<0>(objectsCpy[i]) - curr.first);
      int y_dist = abs(std::get<1>(objectsCpy[i]) - curr.second);
      if ((x_dist + y_dist) < closest) {
        closest = x_dist + y_dist;
        candidate = i;
      }
    }
    curr = {std::get<0>(objectsCpy[candidate]),
            std::get<1>(objectsCpy[candidate])};
    currObject.push_back(objectsCpy[candidate]);
  }
  for (int i = 0; i < objectCount; i++) {
    int zero = std::get<0>(currObject[i]);
    int one = std::get<1>(currObject[i]);
    int two = std::get<2>(currObject[i]);
    if (two == 1) {
      currObject[i] = {zero - 4, one, 1};
    } else if (two == 2) {
      currObject[i] = {zero, one + 4, 2};
    } else if (two == 3) {
      currObject[i] = {zero + 4, one, 3};
    } else if (two == 4) {
      currObject[i] = {zero, one - 4, 4};
    }
  }
  std::pair<int, int> start{18, 1}, end;
  for (int i = 0; i < objectCount; i++) {
    int zero = std::get<0>(currObject[i]);
    int one = std::get<1>(currObject[i]);
    int two = std::get<2>(currObject[i]);
    end = std::make_pair(zero, one);
    search(grid, start, end, two);
    steps.push_back('s');
    start = end;
  }
  std::string path = "";
  for (char x : steps) path += x;
  currDir = 1;
  steps.clear();
  objectsShortest = currObject;
  return path;
}

std::string getClosestObjectBasedOnOffset() {
  int objectCount = objects.size();
  for (int i = 0; i < objectCount; i++) {
    int zero = std::get<0>(objects[i]);
    int one = std::get<1>(objects[i]);
    int two = std::get<2>(objects[i]);
    if (two == 1) {
      objects[i] = {zero - 4, one, 1};
    } else if (two == 2) {
      objects[i] = {zero, one + 4, 2};
    } else if (two == 3) {
      objects[i] = {zero + 4, one, 3};
    } else if (two == 4) {
      objects[i] = {zero, one - 4, 4};
    }
  }
  std::vector<std::vector<char>> grid(20, std::vector<char>(20, ' '));
  for (int i = 0; i < objectCount; i++) {
    grid[std::get<0>(objectsCpy[i])][std::get<1>(objectsCpy[i])] = 'O';
  }
  std::vector<std::tuple<int, int, int>> currObject;
  std::pair<int, int> curr{18, 1};
  while ((int) currObject.size() != objectCount) {
    int closest = INT_MAX, candidate;
    for (int i = 0; i < objectCount; i++) {
      if (std::find(currObject.begin(), currObject.end(), objects[i]) !=
          currObject.end())
        continue;
      int x_dist = abs(std::get<0>(objects[i]) - curr.first);
      int y_dist = abs(std::get<1>(objects[i]) - curr.second);
      if ((x_dist + y_dist) < closest) {
        closest = x_dist + y_dist;
        candidate = i;
      }
    }
    curr = {std::get<0>(objects[candidate]), std::get<1>(objects[candidate])};
    currObject.push_back(objects[candidate]);
  }
  std::pair<int, int> start{18, 1}, end;
  for (int i = 0; i < objectCount; i++) {
    int zero = std::get<0>(currObject[i]);
    int one = std::get<1>(currObject[i]);
    int two = std::get<2>(currObject[i]);
    end = std::make_pair(zero, one);
    search(grid, start, end, two);
    steps.push_back('s');
    start = end;
  }
  std::string path = "";
  for (char x : steps) path += x;
  currDir = 1;
  steps.clear();
  objectsShortest = currObject;
  return path;
}

void Simulation::on_loadObject_clicked()
{
    objects.clear();
    steps.clear();
    haveRoute = false;
    int nn = objectsCpy.size();
    for (int i = 0; i < nn; i++) {
        int zero = std::get<0>(objectsCpy[i]);
        int one = std::get<1>(objectsCpy[i]);
        QString btn = "g";
        btn += QString::number(zero);
        btn += "_";
        btn += QString::number(one);
        this->findChild<QPushButton *>(btn)->setStyleSheet("");
    }
    objectsCpy.clear();
    objectsShortest.clear();
    currDir = 1;
    robotDir = 1;
    currPos = {18, 1};
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open input file"), "", tr("Text Files (*.txt)"));

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "", "File cannot be opened");
        return;
    }
    QTextStream in(&file);
    QString n = in.readLine();
    globalN = n.toInt();
    while (!in.atEnd()) {
        QStringList line = in.readLine().split(" ");
        objects.push_back({19 - (line[1].toInt() - 1), line[0].toInt() - 1, line[2].toInt()});
    }
    objectsCpy = objects;
    for (int i = 0; i < n.toInt(); i++) {
        int zero = std::get<0>(objectsCpy[i]);
        int one = std::get<1>(objectsCpy[i]);
        QString btn = "g";
        btn += QString::number(zero);
        btn += "_";
        btn += QString::number(one);
        this->findChild<QPushButton *>(btn)->setStyleSheet("background-color: rgb(0, 255, 0);\nborder: none;");
    }
//    std::string path = getLeastTurns();
//    std::string path = getClosestObjectBasedOnOriginal();
//    std::string path = getClosestObjectBasedOnOffset();
    file.close();
}

std::string selectedAlgo(int n, std::string path) {
    std::vector<int> positions;
    for (int i = 0; i < n; i++) {
        int zero = std::get<0>(objectsShortest[i]);
        int one = std::get<1>(objectsShortest[i]);
        int two = std::get<2>(objectsShortest[i]);
        if (two == 1) {
          objectsShortest[i] = {zero + 4, one, 1};
        } else if (two == 2) {
          objectsShortest[i] = {zero, one - 4, 2};
        } else if (two == 3) {
          objectsShortest[i] = {zero - 4, one, 3};
        } else if (two == 4) {
          objectsShortest[i] = {zero, one + 4, 4};
        }
      }
    for (int i = 0; i < n; i++) {
      std::pair<int, int> curr{std::get<0>(objectsShortest[i]), std::get<1>(objectsShortest[i])};
      for (int j = 0; j < n; j++) {
        std::pair<int, int> find{std::get<0>(objectsCpy[j]), std::get<1>(objectsCpy[j])};
        if (curr == find) {
          positions.push_back(j + 1);
          break;
        }
      }
    }
    int index = 0;
    std::string finalPath = "";
    steps.clear();
    for (int i = 0; i < (int) path.length(); i++) {
        if (path[i] == 's') {
            finalPath += path[i];
            finalPath += positions[index] + '0';
            index++;
        } else {
            finalPath += path[i];
        }
        steps.push_back(path[i]);
    }
    return finalPath;
}

void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}


void Simulation::on_simulate_clicked()
{
    if (!haveRoute) {
        QMessageBox::warning(this, "", "No algo has been selected!");
        return;
    }
    int i = 0;
    for (char x : steps) {
        if (x == 'f') {
            moveF(currPos);
        } else if (x == 'b') {
            moveB(currPos);
        } else if (x == 'r') {
            moveR(currPos);
        } else if (x == 'l') {
            moveL(currPos);
        } else if (x == 's') {
            int zero = std::get<0>(objectsShortest[i]);
            int one = std::get<1>(objectsShortest[i]);
            QString btn = "g"; btn += QString::number(zero); btn += "_"; btn += QString::number(one);
            this->findChild<QPushButton *>(btn)->setStyleSheet("background-color: rgb(0, 255, 255);\nborder: none;");
            i++;
        }
        delay(500);
    }
}

void Simulation::on_resetRobot_clicked()
{
    haveRoute = false;
    objects = objectsCpy;
    steps.clear();
    std::pair<int, int> curr = currPos;
    QString btn1 = "g"; btn1 += QString::number(curr.first + 1); btn1 += "_"; btn1 += QString::number(curr.second - 1);
    QString btn2 = "g"; btn2 += QString::number(curr.first + 1); btn2 += "_"; btn2 += QString::number(curr.second);
    QString btn3 = "g"; btn3 += QString::number(curr.first + 1); btn3 += "_"; btn3 += QString::number(curr.second + 1);
    QString btn4 = "g"; btn4 += QString::number(curr.first); btn4 += "_"; btn4 += QString::number(curr.second - 1);
    QString btn5 = "g"; btn5 += QString::number(curr.first); btn5 += "_"; btn5 += QString::number(curr.second);
    QString btn6 = "g"; btn6 += QString::number(curr.first); btn6 += "_"; btn6 += QString::number(curr.second + 1);
    QString btn7 = "g"; btn7 += QString::number(curr.first - 1); btn7 += "_"; btn7 += QString::number(curr.second - 1);
    QString btn8 = "g"; btn8 += QString::number(curr.first - 1); btn8 += "_"; btn8 += QString::number(curr.second);
    QString btn9 = "g"; btn9 += QString::number(curr.first - 1); btn9 += "_"; btn9 += QString::number(curr.second + 1);
    this->findChild<QPushButton *>(btn1)->setStyleSheet("");
    this->findChild<QPushButton *>(btn2)->setStyleSheet("");
    this->findChild<QPushButton *>(btn3)->setStyleSheet("");
    this->findChild<QPushButton *>(btn4)->setStyleSheet("");
    this->findChild<QPushButton *>(btn5)->setStyleSheet("");
    this->findChild<QPushButton *>(btn6)->setStyleSheet("");
    this->findChild<QPushButton *>(btn7)->setStyleSheet("");
    this->findChild<QPushButton *>(btn8)->setStyleSheet("");
    this->findChild<QPushButton *>(btn9)->setStyleSheet("");
    ui->g19_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g19_1->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g19_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_1->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g18_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g17_0->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    ui->g17_1->setStyleSheet("background-color: rgb(255, 0, 0);\nborder: none;");
    ui->g17_2->setStyleSheet("background-color: rgb(0, 0, 255);\nborder: none;");
    for (int i = 0; i < (int) objectsCpy.size(); i++) {
        QString btn = "g";
        btn += QString::number(std::get<0>(objectsCpy[i]));
        btn += "_";
        btn += QString::number(std::get<1>(objectsCpy[i]));
        this->findChild<QPushButton *>(btn)->setStyleSheet("background-color: rgb(0, 255, 0);\nborder: none;");
    }
    ui->label->setText("");
    currDir = 1;
    robotDir = 1;
    currPos = {18, 1};
}

void Simulation::on_leastTurns_clicked()
{
    haveRoute = true;
    std::string path = getLeastTurns();
    std::string finalPath = selectedAlgo(globalN, path);
    ui->label->setText("Least turns pathing: " + QString::fromStdString(finalPath));
}


void Simulation::on_closestObject_clicked()
{
    haveRoute = true;
    std::string path = getClosestObjectBasedOnOriginal();
    std::string finalPath = selectedAlgo(globalN, path);
    ui->label->setText("Closest object pathing: " + QString::fromStdString(finalPath));
}


void Simulation::on_closestOffset_clicked()
{
    haveRoute = true;
    std::string path = getClosestObjectBasedOnOffset();
    std::string finalPath = selectedAlgo(globalN, path);
    ui->label->setText("Closest offset pathing: " + QString::fromStdString(finalPath));
}

