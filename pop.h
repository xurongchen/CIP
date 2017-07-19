#ifndef POP_H
#define POP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
    class Pop;
}

class Pop : public QDialog
{
    Q_OBJECT

public:
    explicit Pop(QWidget *parent = 0);
    ~Pop();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Pop *ui;
    int flag;
    QPoint last;
    QPoint judge;

public:
    QString word;
    void get_word(QString, QString);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

};

void show_word(QString, QString);

#endif // POP_H
