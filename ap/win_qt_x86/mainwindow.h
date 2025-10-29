#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Passo 1: Colocar o timer e o slot
#include <QMainWindow>
#include <QTimer>
#include "behaviorthread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Passo 3: Criação do signal
    QTimer ui_timer;                    // Timer de atualização da UI (30x por segundo)

    // Passo 6: Processo físico
    QTimer physical_process_timer;      // Timer do processo físico (100x por segundo)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Passo 15: definir a thread behavior na main window
    BehaviorThread bt;

private:
    Ui::MainWindow *ui;

public slots:
    void update_ui();                   // Slot chamado (30x por segundo)
    void update_physical_process();     // Slot chamado (100x por segundo)

    // Cenários de teste
    void load_scenario_A();
    void load_scenario_B();
    void load_scenario_C();
    void load_scenario_D();
    void load_scenario_E();
};
#endif // MAINWINDOW_H
