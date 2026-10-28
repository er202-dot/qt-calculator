#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNumberClicked(QString num);
    void onDotClicked();
    void onClearClicked();
    void onBackspaceClicked();
    // ===== 新增四则运算槽 =====
    void onOpClicked(QString op);
    void onEqClicked();

private:
    Ui::MainWindow *ui;
    QString m_displayText;
    bool m_hasDot;
    bool m_isFresh;

    // ===== 双操作数运算状态 =====
    double m_op1;          // 第一个操作数
    double m_op2;          // 第二个操作数
    QString m_op;          // 运算符 "+" "-" "*" "/"，空字符串代表无运算符
    bool m_waitSecondOp;   // true = 已经选好运算符，正在等待输入第二个操作数
};
#endif // MAINWINDOW_H
