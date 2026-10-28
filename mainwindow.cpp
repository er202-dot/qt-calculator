#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 输入模块状态初始化
    m_displayText = "0";
    m_hasDot = false;
    m_isFresh = true;
    ui->display->setText(m_displayText);

    // ===== 运算状态初始化 =====
    m_op1 = 0.0;
    m_op2 = 0.0;
    m_op = "";
    m_waitSecondOp = false;

    // 数字、小数点、清除、退格绑定（保留你原来的）
    connect(ui->btn_0, &QPushButton::clicked, this, [=](){onNumberClicked("0");});
    connect(ui->btn_1, &QPushButton::clicked, this, [=](){onNumberClicked("1");});
    connect(ui->btn_2, &QPushButton::clicked, this, [=](){onNumberClicked("2");});
    connect(ui->btn_3, &QPushButton::clicked, this, [=](){onNumberClicked("3");});
    connect(ui->btn_4, &QPushButton::clicked, this, [=](){onNumberClicked("4");});
    connect(ui->btn_5, &QPushButton::clicked, this, [=](){onNumberClicked("5");});
    connect(ui->btn_6, &QPushButton::clicked, this, [=](){onNumberClicked("6");});
    connect(ui->btn_7, &QPushButton::clicked, this, [=](){onNumberClicked("7");});
    connect(ui->btn_8, &QPushButton::clicked, this, [=](){onNumberClicked("8");});
    connect(ui->btn_9, &QPushButton::clicked, this, [=](){onNumberClicked("9");});

    connect(ui->btn_dot, &QPushButton::clicked, this, &MainWindow::onDotClicked);
    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btn_backspace, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);

    // ========== 新增：四则运算符绑定 ==========
    connect(ui->btn_add, &QPushButton::clicked, this, [=](){onOpClicked("+");});
    connect(ui->btn_sub, &QPushButton::clicked, this, [=](){onOpClicked("-");});
    connect(ui->btn_mul, &QPushButton::clicked, this, [=](){onOpClicked("*");});
    connect(ui->btn_div, &QPushButton::clicked, this, [=](){onOpClicked("/");});
    // 等于按钮绑定
    connect(ui->btn_eq, &QPushButton::clicked, this, &MainWindow::onEqClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==========【原来的4个函数，完全不变，直接保留】==========
void MainWindow::onNumberClicked(QString num)
{
    if(m_isFresh)
    {
        m_displayText = num;
        m_hasDot = false;
        m_isFresh = false;
    }
    else
    {
        m_displayText += num;
    }
    ui->display->setText(m_displayText);
}

void MainWindow::onDotClicked()
{
    if(m_isFresh)
    {
        return;
    }
    if(!m_hasDot)
    {
        m_displayText += ".";
        m_hasDot = true;
    }
    ui->display->setText(m_displayText);
}

void MainWindow::onClearClicked()
{
    m_displayText = "0";
    m_hasDot = false;
    m_isFresh = true;
    ui->display->setText(m_displayText);

    // 清除时，同时重置全部运算状态！
    m_op1 = 0.0;
    m_op2 = 0.0;
    m_op = "";
    m_waitSecondOp = false;
}

void MainWindow::onBackspaceClicked()
{
    if(m_isFresh)
        return;

    QChar lastChar = m_displayText.back();
    m_displayText.chop(1);

    if(lastChar == '.')
    {
        m_hasDot = false;
    }

    if(m_displayText.isEmpty())
    {
        m_displayText = "0";
        m_hasDot = false;
        m_isFresh = true;
    }
    ui->display->setText(m_displayText);
}

// ===================== 新增：运算符点击函数 =====================
void MainWindow::onOpClicked(QString op)
{
    // 场景1：还没有运算符，现在保存第一个操作数，记录运算符，等待第二个数
    if(!m_waitSecondOp)
    {
        m_op1 = m_displayText.toDouble();
        m_op = op;
        m_waitSecondOp = true;
        m_isFresh = true;   // 标记：接下来输入新数字，作为op2
    }
    else
    {
        // 场景2：连续按运算符（例如 1 + 2 +）：先算出上一次结果，结果作为新op1，切换运算符
        m_op2 = m_displayText.toDouble();
        double res = 0;
        if(m_op == "+") res = m_op1 + m_op2;
        else if(m_op == "-") res = m_op1 - m_op2;
        else if(m_op == "*") res = m_op1 * m_op2;
        else if(m_op == "/")
        {
            if(qAbs(m_op2) < 1e-9)
            {
                QMessageBox::warning(this,"错误","除零错误！");
                onClearClicked();
                return;
            }
            res = m_op1 / m_op2;
        }
        // 结果赋值给op1，切换成新运算符，等待下一个数字
        m_op1 = res;
        m_op = op;
        m_displayText = QString::number(res);
        ui->display->setText(m_displayText);
        m_isFresh = true;
    }
}

// ===================== 新增：等于号计算 =====================
void MainWindow::onEqClicked()
{
    // 没有选运算符，直接按等于，不做计算
    if(m_op.isEmpty())
        return;

    m_op2 = m_displayText.toDouble();
    double res = 0;

    if(m_op == "+")
        res = m_op1 + m_op2;
    else if(m_op == "-")
        res = m_op1 - m_op2;
    else if(m_op == "*")
        res = m_op1 * m_op2;
    else if(m_op == "/")
    {
        // 除零异常处理
        if(qAbs(m_op2) < 1e-9)
        {
            QMessageBox::warning(this,"错误","不能除以0");
            onClearClicked();
            return;
        }
        res = m_op1 / m_op2;
    }

    // 显示结果
    m_displayText = QString::number(res);
    ui->display->setText(m_displayText);

    // 计算完成：结果作为新的第一个操作数，支持连续运算（结果再和别的数计算）
    m_op1 = res;
    m_op = "";
    m_waitSecondOp = false;
    m_isFresh = true;
    m_hasDot = false;
}
