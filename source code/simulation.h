#ifndef SIMULATION_H
#define SIMULATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Simulation; }
QT_END_NAMESPACE

class Simulation : public QMainWindow
{
    Q_OBJECT

public:
    Simulation(QWidget *parent = nullptr);
    ~Simulation();

private slots:
    void on_loadObject_clicked();

    void on_simulate_clicked();

    void on_resetRobot_clicked();


    void on_leastTurns_clicked();

    void on_closestObject_clicked();

    void on_closestOffset_clicked();

private:
    Ui::Simulation *ui;
    void moveF(std::pair<int, int> curr);
    void moveB(std::pair<int, int> curr);
    void moveR(std::pair<int, int> curr);
    void moveL(std::pair<int, int> curr);
};
#endif // SIMULATION_H
