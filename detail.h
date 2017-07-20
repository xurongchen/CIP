#ifndef DETAIL_H
#define DETAIL_H

#include <QDialog>

namespace Ui {
    class Detail;
}

class Detail : public QDialog
{
    Q_OBJECT

public:
    explicit Detail(QWidget *parent = 0);
    ~Detail();

private:
    Ui::Detail *ui;
};

#endif // DETAIL_H
