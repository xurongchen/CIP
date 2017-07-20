#ifndef DETAIL_H
#define DETAIL_H

#include <QDialog>
#include <QMouseEvent>
#include <QStandardItemModel>

namespace Ui {
    class Detail;
}

class Detail : public QDialog
{
    Q_OBJECT

public:
    explicit Detail(QWidget *parent = 0);
    ~Detail();
    void set_policyid(int _policyid);
    QStandardItemModel *insurancelist;

private slots:
    void on_PBOk_clicked();

    void on_PBClose_clicked();

private:
    int policyid;
    Ui::Detail *ui;
    int flag, flag2;
    QPoint last;
    QPoint judge;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

void call_detail(int policyid);

#endif // DETAIL_H
