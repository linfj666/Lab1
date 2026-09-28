#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // 键盘事件：鼠标与键盘输入复用同一套处理逻辑
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void digitClicked(int digit);      // 数字键 0~9
    void operatorClicked(QChar op);    // 四则运算符 + - * /
    void equalClicked();               // 等号 =
    void clearClicked();               // 清除 C
    void backspaceClicked();           // 退格
    void dotClicked();                 // 小数点
    void negClicked();                 // 正负号 ±

private:
    Ui::MainWindow *ui;

    // 计算状态
    QString m_currentInput;      // 当前正在输入的字符串
    double  m_firstOperand;      // 第一个操作数
    QString m_pendingOp;         // 待执行的运算符
    bool    m_waitingForOperand; // 是否在等待输入第二个操作数（刚按过运算符）
    bool    m_justCalculated;    // 刚完成一次计算（按过等号）

    void connectButtons();       // 连接所有按钮信号到处理逻辑
    void applyStyle();           // 应用界面样式表
    void updateDisplay();        // 刷新显示屏
    bool calculate(double &result); // 执行一次运算，返回是否成功
};
#endif // MAINWINDOW_H
