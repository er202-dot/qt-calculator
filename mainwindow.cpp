#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 状态初始化
    m_displayText = "0";
    m_hasDot = false;
    m_isFresh = true;
    ui->display->setText(m_displayText);

    // ========== 绑定所有数字按钮（lambda方式，共用onNumberClicked） ==========
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

    // 功能按钮绑定
    connect(ui->btn_dot, &QPushButton::clicked, this, &MainWindow::onDotClicked);
    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btn_backspace, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 数字输入
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

// 小数点：禁止小数点开头 + 禁止同一个数字多个小数点
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

// C清除
void MainWindow::onClearClicked()
{
    m_displayText = "0";
    m_hasDot = false;
    m_isFresh = true;
    ui->display->setText(m_displayText);
}

// 退格
void MainWindow::onBackspaceClicked()
{
    if(m_isFresh)
        return;

    QChar lastChar = m_displayText.back();
    m_displayText.chop(1);

    // 如果删掉的是小数点，标记重置
    if(lastChar == '.')
    {
        m_hasDot = false;
    }

    // 删空了，回到0初始状态
    if(m_displayText.isEmpty())
    {
        m_displayText = "0";
        m_hasDot = false;
        m_isFresh = true;
    }
    ui->display->setText(m_displayText);
}
