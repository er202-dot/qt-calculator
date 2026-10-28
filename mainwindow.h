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
    void onOpClicked(QString op);
    void onEqClicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    QString m_displayText;
    bool m_hasDot;
    bool m_isFresh;

    double m_op1;
    double m_op2;
    QString m_op;
    bool m_waitSecondOp;
};
#endif // MAINWINDOW_H
