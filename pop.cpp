#include "pop.h"
#include "ui_pop.h"
#include <QDebug>
#include <QBitmap>
#include <QPainter>

Pop::Pop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pop)
{
    ui->setupUi(this);
    this->setFixedSize(190,117); //���ù̶���С

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //���ر���������С���ɼ�

    //Բ��
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),5,5);
    setMask(objBitmap);
}

void Pop::mousePressEvent(QMouseEvent *e) //���������
{
    judge.setX(0), judge.setY(0);
    judge = e->pos();
    if(judge.x() == 0 && judge.y() == 0)
    {
        flag = 0;
        return ;
    }
    flag = 1;
    last = e->globalPos();
    //qDebug() << judge.rx() << ".." << judge.ry();
}
void Pop::mouseMoveEvent(QMouseEvent *e) //�����������ƶ�
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void Pop::mouseReleaseEvent(QMouseEvent *e) //����ͷ�
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

Pop::~Pop()
{
    delete ui;
}

void Pop::on_pushButton_clicked()
{
    this->close();
}

void Pop::get_word(QString Title, QString Word)
{
    ui->Title->setText(Title);
    ui->Word->setText(Word);
    ui->Title->setAlignment(Qt::AlignCenter);
    ui->Word->setAlignment(Qt::AlignCenter);
}

void show_word(QString Title, QString Word)
{
    Pop p;
    p.get_word(Title, Word);
    p.exec();
}
