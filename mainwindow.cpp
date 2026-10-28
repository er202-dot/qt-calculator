#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 输入状态初始化
    m_displayText = "0";
    m_hasDot = false;
    m_isFresh = true;
    ui->display->setText(m_displayText);

    // 运算状态初始化
    m_op1 = 0.0;
    m_op2 = 0.0;
    m_op = "";
    m_waitSecondOp = false;

    // 数字按钮绑定
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

    // 四则运算符绑定
    connect(ui->btn_add, &QPushButton::clicked, this, [=](){onOpClicked("+");});
    connect(ui->btn_sub, &QPushButton::clicked, this, [=](){onOpClicked("-");});
    connect(ui->btn_mul, &QPushButton::clicked, this, [=](){onOpClicked("*");});
    connect(ui->btn_div, &QPushButton::clicked, this, [=](){onOpClicked("/");});
    connect(ui->btn_eq, &QPushButton::clicked, this, &MainWindow::onEqClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

    m_op1 = 0.0;
    m_op2 = 0.0;
    m_op = "";
    m_waitSecondOp = false;
    ui->display->setText(m_displayText);
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

void MainWindow::onOpClicked(QString op)
{
    if(!m_waitSecondOp)
    {
        m_op1 = m_displayText.toDouble();
        m_op = op;
        m_waitSecondOp = true;
        m_isFresh = true;
    }
    else
    {
        m_op2 = m_displayText.toDouble();
        double res = 0;
        if(m_op == "+") res = m_op1 + m_op2;
        else if(m_op == "-") res = m_op1 - m_op2;
        else if(m_op == "*") res = m_op1 * m_op2;
        else if(m_op == "/")
        {
            if(fabs(m_op2) < 1e-9)
            {
                QMessageBox::warning(this,"错误","不能除以0");
                onClearClicked();
                return;
            }
            res = m_op1 / m_op2;
        }
        m_op1 = res;
        m_op = op;
        m_displayText = QString::number(res);
        ui->display->setText(m_displayText);
        m_isFresh = true;
    }
}

void MainWindow::onEqClicked()
{
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
        if(fabs(m_op2) < 1e-9)
        {
            QMessageBox::warning(this,"错误","不能除以0");
            onClearClicked();
            return;
        }
        res = m_op1 / m_op2;
    }

    m_displayText = QString::number(res);
    ui->display->setText(m_displayText);

    m_op1 = res;
    m_op = "";
    m_waitSecondOp = false;
    m_isFresh = true;
    m_hasDot = false;
}

// 键盘事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch(key)
    {
    case Qt::Key_0: onNumberClicked("0"); break;
    case Qt::Key_1: onNumberClicked("1"); break;
    case Qt::Key_2: onNumberClicked("2"); break;
    case Qt::Key_3: onNumberClicked("3"); break;
    case Qt::Key_4: onNumberClicked("4"); break;
    case Qt::Key_5: onNumberClicked("5"); break;
    case Qt::Key_6: onNumberClicked("6"); break;
    case Qt::Key_7: onNumberClicked("7"); break;
    case Qt::Key_8: onNumberClicked("8"); break;
    case Qt::Key_9: onNumberClicked("9"); break;

    case Qt::Key_Period: onDotClicked(); break;

    case Qt::Key_Plus: onOpClicked("+"); break;
    case Qt::Key_Minus: onOpClicked("-"); break;
    case Qt::Key_Asterisk: onOpClicked("*"); break;
    case Qt::Key_Slash: onOpClicked("/"); break;

    case Qt::Key_Enter:
    case Qt::Key_Return: onEqClicked(); break;

    case Qt::Key_Backspace: onBackspaceClicked(); break;
    case Qt::Key_Escape: onClearClicked(); break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
