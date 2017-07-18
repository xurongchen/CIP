#ifndef MANAGEROPER_H
#define MANAGEROPER_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
    class ManagerOper;
}

class ManagerOper : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerOper(QWidget *parent = 0);
    ~ManagerOper();
    void closeEvent(QCloseEvent *);

private slots:
    bool closeWidget();

    void on_PBReturn_clicked();

    void on_PBMin_clicked();

    void on_PBView_clicked();

    void on_PBAdd_clicked();

    void on_PBMessage_clicked();

    void on_PBSend1_clicked();

    void on_PBSend2_clicked();

    void on_PBSend3_clicked();

    void on_PBMy1_clicked();

    void on_PBMy2_clicked();

    void on_PBMy3_clicked();

    void on_PBGet1_clicked();

    void on_PBGet2_clicked();

    void on_PBGet3_clicked();

private:
    Ui::ManagerOper *ui;
    int flag, flag2;
    QPoint last;
    QPoint judge;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

};

#endif // MANAGEROPER_H
