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
    // 刚按过运算符或刚算完，重新开始输入新操作数
    if (m_waitingForOperand || m_justCalculated)
    {
        m_currentInput.clear();
        m_waitingForOperand = false;
        m_justCalculated = false;
    }
    // 去掉前导0："0" -> "5" 而不是 "05"
    if (m_currentInput == "0")
        m_currentInput.clear();
    // 限制输入长度，避免显示溢出
    if (m_currentInput.length() >= 15)
        return;
    m_currentInput.append(QString::number(digit));
    updateDisplay();
}

void MainWindow::dotClicked()
{
    if (m_waitingForOperand || m_justCalculated)
    {
        m_currentInput = "0";
        m_waitingForOperand = false;
        m_justCalculated = false;
    }
    // 防止同一操作数输入重复小数点
    if (m_currentInput.contains('.'))
        return;
    m_currentInput.append('.');
    updateDisplay();
}

void MainWindow::backspaceClicked()
{
    // 等待第二个操作数或刚算完时不允许退格
    if (m_waitingForOperand || m_justCalculated)
        return;
    // 只剩一位（或负号+一位）时退格归零
    if (m_currentInput.length() <= 1
        || (m_currentInput.length() == 2 && m_currentInput.startsWith('-')))
        m_currentInput = "0";
    else
        m_currentInput.chop(1);
    updateDisplay();
}

void MainWindow::clearClicked()
{
    // 清除后重新计算：所有状态复位
    m_currentInput = "0";
    m_firstOperand = 0.0;
    m_pendingOp.clear();
    m_waitingForOperand = false;
    m_justCalculated = false;
    updateDisplay();
}

void MainWindow::negClicked()
{
    // 正负号切换，仅对非零输入生效
    if (m_currentInput == "0" || m_currentInput.isEmpty())
        return;
    if (m_currentInput.startsWith('-'))
        m_currentInput.remove(0, 1);
    else
        m_currentInput.prepend('-');
    updateDisplay();
}

void MainWindow::operatorClicked(QChar op)
{
    Q_UNUSED(op);
}

void MainWindow::equalClicked()
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
