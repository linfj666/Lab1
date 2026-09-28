#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QtMath>

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
    applyStyle();
}

// 应用样式表：深色主题计算器风格
void MainWindow::applyStyle()
{
    setStyleSheet(R"(
        QMainWindow { background-color: #2b2b2b; }
        QLabel#display { background-color: #1f1f1f; color: #ffffff;
                         font-size: 42px; font-weight: bold;
                         border-radius: 12px; padding: 0 16px; }
        QPushButton { font-size: 20px; font-weight: bold;
                      border: none; border-radius: 10px; }
        QPushButton:hover { opacity: 0.9; }
        QPushButton:pressed { opacity: 0.7; }
        QPushButton#btnClear, QPushButton#btnBackspace, QPushButton#btnNeg {
            background-color: #a5a5a5; color: #111111; }
        QPushButton#btnAdd, QPushButton#btnSub,
        QPushButton#btnMul, QPushButton#btnDiv {
            background-color: #f09a3e; color: #ffffff; }
        QPushButton#btnEq { background-color: #2f7cf6; color: #ffffff; }
        QPushButton { background-color: #4a4a4a; color: #ffffff; }
        QPushButton#btn0 { background-color: #4a4a4a; color: #ffffff; }
    )");
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
    // 第一次输入运算符：保存第一个操作数
    if (m_pendingOp.isEmpty())
    {
        m_firstOperand = m_currentInput.toDouble();
        m_pendingOp = op;
        m_waitingForOperand = true;
        updateDisplay();
        return;
    }

    // 已处于等待状态（连续输入运算符）：用新运算符替换旧运算符
    if (m_waitingForOperand)
    {
        m_pendingOp = op;
        return;
    }

    // 已有完整表达式（连续运算）：先算中间结果，再用新运算符继续
    double result = 0.0;
    if (calculate(result))
    {
        m_firstOperand = result;
        m_currentInput = QString::number(result);
        m_pendingOp = op;
        m_waitingForOperand = true;
        updateDisplay();
    }
}

void MainWindow::equalClicked()
{
    // 没有待执行的运算符时按等号无效
    if (m_pendingOp.isEmpty())
        return;
    // 等待第二个操作数时按等号，直接用第一个操作数作为第二个操作数
    if (m_waitingForOperand)
        m_currentInput = QString::number(m_firstOperand);

    double result = 0.0;
    if (calculate(result))
    {
        m_currentInput = QString::number(result);
        m_firstOperand = result;          // 结果可作为下一次连续运算的操作数
        m_pendingOp.clear();
        m_waitingForOperand = false;
        m_justCalculated = true;          // 标记刚算完，下次输入数字开始新运算
        updateDisplay();
    }
}

bool MainWindow::calculate(double &result)
{
    double second = m_currentInput.toDouble();

    if (m_pendingOp == '+')
        result = m_firstOperand + second;
    else if (m_pendingOp == '-')
        result = m_firstOperand - second;
    else if (m_pendingOp == '*')
        result = m_firstOperand * second;
    else if (m_pendingOp == '/')
    {
        // 除数为0：提示错误并复位运算状态，允许重新输入
        if (qFuzzyIsNull(second))
        {
            m_currentInput = "除数不能为0";
            m_pendingOp.clear();
            m_waitingForOperand = true;
            updateDisplay();
            return false;
        }
        result = m_firstOperand / second;
    }
    return true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    // 数字：主键盘 0x30-0x39，小键盘 0x80-0x89（Qt6 键值兼容）
    if (key >= 0x30 && key <= 0x39)
        digitClicked(key - 0x30);
    else if (key >= 0x80 && key <= 0x89)
        digitClicked(key - 0x80);

    // 四则运算符（兼容小键盘键值 0x8a~0x8e）
    else if (key == Qt::Key_Plus || key == 0x8b)
        operatorClicked('+');
    else if (key == Qt::Key_Minus || key == 0x8c)
        operatorClicked('-');
    else if (key == Qt::Key_Asterisk || key == 0x8a)
        operatorClicked('*');
    else if (key == Qt::Key_Slash || key == 0x8e)
        operatorClicked('/');

    // 等号：= 或 回车（含小键盘回车 0x8f）
    else if (key == Qt::Key_Equal || key == Qt::Key_Enter
             || key == Qt::Key_Return || key == 0x8f)
        equalClicked();

    // 小数点（主键盘 . 和 , ，小键盘小数点 0x8d）
    else if (key == Qt::Key_Period || key == Qt::Key_Comma || key == 0x8d)
        dotClicked();

    // 退格
    else if (key == Qt::Key_Backspace)
        backspaceClicked();

    // 清除：C 或 Esc
    else if (key == Qt::Key_C || key == Qt::Key_Escape)
        clearClicked();

    else
        QMainWindow::keyPressEvent(event);
}
