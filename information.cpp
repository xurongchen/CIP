#include "information.h"
#include "currentuser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>

void send_renew(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("时间")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("接收人")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QString::fromLocal8Bit("内容")));

    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from InfoList where SenderId=%1 and OwnerId=%2")
               .arg(get_currentuser()).arg(get_currentuser()));
    //query.first();
    int nowline = 0;
    while(query.next())
    {
        tb->setItem(nowline, 0, new QStandardItem(query.value(3).toString()));
        if(query.value(2).toInt()!=-1)
        {
            QSqlQuery tmpquery(db);
            tmpquery.exec(QString("select * from UserList where Id=%1")
                          .arg(query.value(2).toInt()));
            tmpquery.first();
            tb->setItem(nowline, 1, new QStandardItem(tmpquery.value(1).toString()));
        }
        else tb->setItem(nowline, 1, new QStandardItem("broadcast"));
        tb->setItem(nowline, 2, new QStandardItem(query.value(5).toString()));
        tb->setItem(nowline, 3, new QStandardItem(query.value(0).toString()));
        nowline++;
    }
    tb->sort(3, Qt::DescendingOrder);
}

void send_delete(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
}

void reci_renew(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("时间")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("发送人")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QString::fromLocal8Bit("内容")));

    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from InfoList where RecipientId=%1 and OwnerId=%2 or RecipientId=-1")
               .arg(get_currentuser()).arg(get_currentuser()));
    //query.first();
    int nowline = 0;
    while(query.next())
    {
        tb->setItem(nowline, 0, new QStandardItem(query.value(3).toString()));
        QSqlQuery tmpquery(db);
        tmpquery.exec(QString("select * from UserList where Id=%1")
                         .arg(query.value(1).toInt()));
        tmpquery.first();
        tb->setItem(nowline, 1, new QStandardItem(tmpquery.value(1).toString()));
        tb->setItem(nowline, 2, new QStandardItem(query.value(5).toString()));
        tb->setItem(nowline, 3, new QStandardItem(query.value(0).toString()));
        nowline++;
    }
    tb->sort(3, Qt::DescendingOrder);
}

void reci_delete(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
}
