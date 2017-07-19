#include "insurance.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>

void insurancelist_renew(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("PolicyId")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Time")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("CarNum")));
    tb->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("Insured")));
    tb->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("Operator")));
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from PolicyList"));
    //query.first();
    int nowline = 0;
    while(query.next())
    {
        tb->setItem(nowline, 0, new QStandardItem(query.value(1).toString()));
        tb->setItem(nowline, 1, new QStandardItem(query.value(7).toString()));
        tb->setItem(nowline, 2, new QStandardItem(query.value(4).toString()));
        tb->setItem(nowline, 3, new QStandardItem(query.value(2).toString()));
        QSqlQuery tmpquery(db);
        tmpquery.exec(QString("select * from UserList where Id=%1")
                          .arg(query.value(10).toString()));
        tmpquery.first();
        tb->setItem(nowline, 4, new QStandardItem(tmpquery.value(1).toString()));
        tb->setItem(nowline, 5, new QStandardItem(query.value(0).toInt()));
        nowline++;
    }
    tb->sort(1, Qt::DescendingOrder);
}

void insurancelist_delete(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
}
