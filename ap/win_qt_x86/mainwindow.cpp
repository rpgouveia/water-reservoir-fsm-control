#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Passo 4: Conectando o signal com o slot
    // Devemos passar os endereços
    connect(&ui_timer, &QTimer::timeout, this, &MainWindow::update_ui);
    ui_timer.start(33); // Tempo vai ser 1/30 = 0.0333333

    // Passo 8: Conectando novo timer
    connect(&physical_process_timer, &QTimer::timeout, this, &MainWindow::update_physical_process);
    physical_process_timer.start(10);    // 10 milisegundos

    // Conectar botões de cenários
    connect(ui->pushButton_scenarioA, &QPushButton::clicked, this, &MainWindow::load_scenario_A);
    connect(ui->pushButton_scenarioB, &QPushButton::clicked, this, &MainWindow::load_scenario_B);
    connect(ui->pushButton_scenarioC, &QPushButton::clicked, this, &MainWindow::load_scenario_C);
    connect(ui->pushButton_scenarioD, &QPushButton::clicked, this, &MainWindow::load_scenario_D);
    connect(ui->pushButton_scenarioE, &QPushButton::clicked, this, &MainWindow::load_scenario_E);

    // Passo 12: adicionando o behavior para acionar a máquina de estados da cisterna
    // É necessário usar threads para evitar o travamento da simulação, crie uma classe BehaviorThread
    // Passo 16: iniciar a thread behavior.
    bt.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Passo 2: Fazer implementação do UI Update
// Fase 1
extern bool pin_sensor11;
extern bool pin_sensor12;
extern bool pin_valve1;
extern float level_tank1;
extern bool pin_valve1_led_error;
// Fase 2
extern bool pin_sensor21;
extern bool pin_sensor22;
extern bool pin_pump1;
extern float level_tank2;
extern bool pin_pump1_led_error;
// Fase 3
extern bool pin_sensor31;
extern bool pin_sensor32;
extern bool pin_valve2;
extern float level_tank3;
extern float temp_tank3;
extern bool pin_valve2_led_error;
extern bool pin_resistor1;
extern bool pin_resistor1_led_error;

void MainWindow::update_ui(){
    // Fase 1
    ui->toolButton_sensor11->setPower(pin_sensor11);
    ui->toolButton_sensor12->setPower(pin_sensor12);
    ui->toolButton_valve1->setPower(pin_valve1);
    ui->toolButton_tank1->setValue(level_tank1);
    ui->toolButton_sm1_led_error->setPower(pin_valve1_led_error);

    // Fase 2
    ui->toolButton_sensor21->setPower(pin_sensor21);
    ui->toolButton_sensor22->setPower(pin_sensor22);
    ui->toolButton_pump1->setPower(pin_pump1);
    ui->toolButton_tank2->setValue(level_tank2);
    ui->toolButton_sm2_led_error->setPower(pin_pump1_led_error);

    // Fase 3
    ui->toolButton_sensor31->setPower(pin_sensor31);
    ui->toolButton_sensor32->setPower(pin_sensor32);
    ui->toolButton_valve2->setPower(pin_valve2);
    ui->toolButton_tank3->setValue(level_tank3);
    ui->toolButton_resistor1->setPower(pin_resistor1);
    ui->toolButton_sm3_led_error->setPower(pin_valve2_led_error);
    ui->toolButton_sm4_led_error->setPower(pin_resistor1_led_error);

    // Display de Temperatura e Indicador Visual
    ui->lcdNumber_temp->display(temp_tank3);
    ui->progressBar_temp->setValue((int)temp_tank3);
    if(temp_tank3 < 49.0) {
        ui->progressBar_temp->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
    } else if(temp_tank3 <= 51.0) {
        ui->progressBar_temp->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    } else {
        ui->progressBar_temp->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    }

    // Mostrar valores numéricos no console
    qDebug() << "T1:" << level_tank1 << "% | T2:" << level_tank2 << "% | T3:" << level_tank3 << "%";
}

// Passo 7: Implementação do processo de abastecimento
void MainWindow::update_physical_process() {
    // Tanque 1
    if(pin_valve1) {
        // Com vazão de 0.1 deve abastecer o tanque da cisterna em 10s
        // Á vazão de 0.04 será menor que consumo da bomba (0.05)
        level_tank1 += 0.04;
        if(level_tank1 > 100){
            level_tank1 = 100;      // Transborda o tanque
            ui->toolButton_tank1_overflow->setPower(true);
        } else {
            ui->toolButton_tank1_overflow->setPower(false);
        }
    }

    // Tanque 2
    if(pin_pump1) {
        if(level_tank1 > 0.05) {
            level_tank1 -= 0.05; // Consumo do Tanque 1
            level_tank2 += 0.05; // Abastecimento do Tanque 2

            if (level_tank2 > 100) {
                level_tank2 = 100;
                ui->toolButton_tank2_overflow->setPower(true);
            } else {
                ui->toolButton_tank2_overflow->setPower(false);
            }
        }
    }

    // Tanque 3 - Bidirecional
    if(pin_valve2) {
        float flow_rate = 0.025;

        // Calcular diferença de altura
        float diff = level_tank3 - (level_tank2 * 2); // T3 em 100% é igual a 50% de T2

        if(diff > 0) {
            // t3 está mais alto => água retorna de t3 para t2
            float transfer = flow_rate * 4;
            if(level_tank3 >= transfer) {
                level_tank3 -= transfer;
                level_tank2 += flow_rate;

                if(level_tank2 > 100) {
                    float overflow = level_tank2 - 100;
                    level_tank2 = 100;
                    level_tank3 += overflow * 4;
                }
            }
        } else if(diff < 0) {
            // t2 está mais alto => água vai de t2 para t3
            float transfer = flow_rate;
            if(level_tank2 >= transfer) {
                level_tank2 -= transfer;
                level_tank3 += flow_rate * 4;

                if(level_tank3 > 100) {
                    float overflow = level_tank3 - 100;
                    level_tank3 = 100;
                    level_tank2 += overflow * 0.25;
                }
            }
        }
    }

    // Controle térmico
    if(pin_resistor1 && pin_sensor31) {
        // Aquecimento: +2°C por segundo = +0.02°C por ciclo
        temp_tank3 += 0.02;
        if(temp_tank3 > 100.0) temp_tank3 = 100.0;
    } else {
        // Resfriamento natural: -0.5°C por segundo = -0.005°C por ciclo
        temp_tank3 -= 0.005;
        if(temp_tank3 < 25.0) temp_tank3 = 25.0;  // Temperatura ambiente mínima
    }

    // Consumo de água fria (Tanque 2)
    if(ui->checkBox_tank2_consume->isChecked()){
        level_tank2 -= 0.08;
        if(level_tank2 < 0) {
            level_tank2 = 0;
        }
    }

    // Consumo de água quente (Tanque 3)
    if(ui->checkBox_tank3_consume->isChecked()) {
        level_tank3 -= 0.08;
        if(level_tank3 < 0) level_tank3 = 0;
    }

    // Atualizar sensores
    // Tanque 1
    pin_sensor11 = (level_tank1 >= 10);     // Liga pin_sensor11 se chegar em 10%
    pin_sensor12 = (level_tank1 >= 90);     // Liga pin_sensor12 se chegar em 90%

    // Tanque 2
    pin_sensor21 = (level_tank2 >= 10);
    pin_sensor22 = (level_tank2 >= 90);

    // Tanque 3
    pin_sensor31 = (level_tank3 >= 10);
    pin_sensor32 = (level_tank3 >= 90);
}


// Cenários de Teste
void MainWindow::load_scenario_A() {
    // Cenário A: Enchimento Completo
    // Inicial: t2 = 100%, t3 = 0%, Temp = 25°C
    level_tank1 = 100;
    level_tank2 = 100;
    level_tank3 = 0;
    temp_tank3 = 25.0;
    ui->checkBox_tank2_consume->setChecked(false);
    ui->checkBox_tank3_consume->setChecked(false);
}

void MainWindow::load_scenario_B() {
    // Cenário B: Equilíbrio Parcial
    // Inicial: t2 = 50%, t3 = 0%, Temp = 25°C
    level_tank1 = 100;
    level_tank2 = 50;
    level_tank3 = 0;
    temp_tank3 = 25.0;
    ui->checkBox_tank2_consume->setChecked(false);
    ui->checkBox_tank3_consume->setChecked(false);
}

void MainWindow::load_scenario_C() {
    // Cenário C: Proteção contra Refluxo
    // Inicial: t2 = 0%, t3 = 100%, Temp = 25°C
    level_tank1 = 100;
    level_tank2 = 0;
    level_tank3 = 100;
    temp_tank3 = 25.0;
    ui->checkBox_tank2_consume->setChecked(false);
    ui->checkBox_tank3_consume->setChecked(false);
}

void MainWindow::load_scenario_D() {
    // Cenário D: Decisão com Informação Limitada
    // Inicial: t2 = 10%, t3 = 89%, Temp = 25°C
    level_tank1 = 100;
    level_tank2 = 10;
    level_tank3 = 89;
    temp_tank3 = 25.0;
    ui->checkBox_tank2_consume->setChecked(false);
    ui->checkBox_tank3_consume->setChecked(false);
}

void MainWindow::load_scenario_E() {
    // Cenário E: Proteção contra Aquecimento a Seco
    // Inicial: t2 = 50%, t3 = 15%, Temp = 50°C, dreno t3 ligado
    level_tank1 = 100;
    level_tank2 = 50;
    level_tank3 = 15;
    temp_tank3 = 50.0;
    ui->checkBox_tank2_consume->setChecked(false);
    ui->checkBox_tank3_consume->setChecked(true);  // Liga consumo de t3
}
