#ifndef SELLEROPER_H
#define SELLEROPER_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
    class SellerOper;
}

class SellerOper : public QDialog
{
    Q_OBJECT

public:
    explicit SellerOper(QWidget *parent = 0);
    ~SellerOper();
    void closeEvent(QCloseEvent *);

private slots:
    bool closeWidget();

    void on_PBMin_clicked();

    void on_PBReturn_clicked();

    void on_PBWork_clicked();

    void on_PBMessage_clicked();

    void on_PBSend1_clicked();

    void on_PBSend1_2_clicked();

    void on_PBSend1_3_clicked();

    void on_PBMy1_clicked();

    void on_PBMy1_2_clicked();

    void on_PBMy1_3_clicked();

    void on_PBGet1_clicked();

    void on_PBGet1_2_clicked();

    void on_PBGet1_3_clicked();

    void on_PBA1_clicked();

    void on_PBB1_clicked();

    void on_PBC1_clicked();

    void on_PBA2_clicked();

    void on_PBB2_clicked();

    void on_PBC2_clicked();

    void on_PBA3_clicked();

    void on_PBB3_clicked();

    void on_PBC3_clicked();

    void on_PBWork2_clicked();

    void on_PBMessage2_clicked();

private:
    Ui::SellerOper *ui;
    int flag, flag2;
    QPoint last;
    QPoint judge;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

};

#endif // SELLEROPER_H
