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

private:
    int policyid;
    Ui::Detail *ui;
};

void call_detail(int policyid);

#endif // DETAIL_H
