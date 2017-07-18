#include "infomation.h"
#include "currentuser.h"
#include <QSqlDatabase>
#include <QSqlQuery>

void send_renew(QStandardItemModel *tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Time")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Reci")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Content")));

    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from InfoList where SenderId=%1").arg(get_currentuser()));
    query.last();
    int nowline = 0;
    while(query.previous())
    {
        tb->setItem(nowline, 0, new QStandardItem(query.value(3).toString()));
        if(query.value(2).toInt()!=-1)
        {
            QSqlQuery tmpquery(db);
            tmpquery.exec(QString("select * from UserList where Id=%1").arg(query.value(2).toInt()));
            tb->setItem(nowline, 1, new QStandardItem(tmpquery.value(1).toString()));
        }
        else tb->setItem(nowline, 1, new QStandardItem("broadcast"));
        tb->setItem(nowline, 2, new QStandardItem(query.value(4).toString()));
        nowline++;
    }
}
