#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentInput("0")
    , m_firstOperand(0.0)
    , m_pendingOp()
    , m_waitingForOperand(false)
    , m_justCalculated(false)
{
    ui->setupUi(this);
    connectButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 连接所有按钮：把按钮点击信号统一转发到对应的处理函数
void MainWindow::connectButtons()
{
    // 数字按钮 0~9
    connect(ui->btn0, &QPushButton::clicked, this, [this]() { digitClicked(0); });
    connect(ui->btn1, &QPushButton::clicked, this, [this]() { digitClicked(1); });
    connect(ui->btn2, &QPushButton::clicked, this, [this]() { digitClicked(2); });
    connect(ui->btn3, &QPushButton::clicked, this, [this]() { digitClicked(3); });
    connect(ui->btn4, &QPushButton::clicked, this, [this]() { digitClicked(4); });
    connect(ui->btn5, &QPushButton::clicked, this, [this]() { digitClicked(5); });
    connect(ui->btn6, &QPushButton::clicked, this, [this]() { digitClicked(6); });
    connect(ui->btn7, &QPushButton::clicked, this, [this]() { digitClicked(7); });
    connect(ui->btn8, &QPushButton::clicked, this, [this]() { digitClicked(8); });
    connect(ui->btn9, &QPushButton::clicked, this, [this]() { digitClicked(9); });

    // 四则运算符
    connect(ui->btnAdd, &QPushButton::clicked, this, [this]() { operatorClicked('+'); });
    connect(ui->btnSub, &QPushButton::clicked, this, [this]() { operatorClicked('-'); });
    connect(ui->btnMul, &QPushButton::clicked, this, [this]() { operatorClicked('*'); });
    connect(ui->btnDiv, &QPushButton::clicked, this, [this]() { operatorClicked('/'); });

    // 功能按钮
    connect(ui->btnEq, &QPushButton::clicked, this, &MainWindow::equalClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearClicked);
    connect(ui->btnBackspace, &QPushButton::clicked, this, &MainWindow::backspaceClicked);
    connect(ui->btnDot, &QPushButton::clicked, this, &MainWindow::dotClicked);
    connect(ui->btnNeg, &QPushButton::clicked, this, &MainWindow::negClicked);
}

void MainWindow::updateDisplay()
{
    ui->display->setText(m_currentInput);
}

void MainWindow::digitClicked(int digit)
{
    Q_UNUSED(digit);
}

void MainWindow::operatorClicked(QChar op)
{
    Q_UNUSED(op);
}

void MainWindow::equalClicked()
{
}

void MainWindow::clearClicked()
{
}

void MainWindow::backspaceClicked()
{
}

void MainWindow::dotClicked()
{
}

void MainWindow::negClicked()
{
}

bool MainWindow::calculate(double &result)
{
    Q_UNUSED(result);
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}
