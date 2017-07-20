#include "policyinsurance.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>


std::set<int> get_id_set;

void insuranceget_init()
{
    get_id_set.clear();
}

void insuranceget_insert(int _id)
{
    get_id_set.insert(_id);
}

void insuranceget_remove(int _id)
{
    get_id_set.erase(_id);
}

bool insuranceget_count(int _id)
{
    if(get_id_set.count(_id)==1)
        return true;
    else return false;
}

void insuranceget_renew(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Name")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("FixCost")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("FloatCost(%)")));

    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    int nowline = 0;
    for(std::set<int>::iterator it=get_id_set.begin();it!=get_id_set.end();it++)
    {
        query.exec(QString("select * from InsuranceList where Id=%1").arg(*it));
        if(query.first())
        {
            tb->setItem(nowline, 0, new QStandardItem(query.value(1).toString()));
            tb->setItem(nowline, 1, new QStandardItem(query.value(2).toString()));
            tb->setItem(nowline, 2, new QStandardItem(query.value(3).toString()));
            tb->setItem(nowline, 3, new QStandardItem(query.value(0).toString()));
            nowline++;
        }
    }
    tb->sort(3, Qt::DescendingOrder);

}

void insuranceget_delete(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
}
void insuranceall_renew(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
    tb = new QStandardItemModel();
    tb->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Name")));
    tb->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("FixCost")));
    tb->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("FloatCost(%)")));

    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    int nowline = 0;
    query.exec(QString("select * from InsuranceList"));
    while(query.next())
    {
        if(!insuranceget_count(query.value(0).toInt())&&query.value(4).toInt()==1)
        {
            tb->setItem(nowline, 0, new QStandardItem(query.value(1).toString()));
            tb->setItem(nowline, 1, new QStandardItem(query.value(2).toString()));
            tb->setItem(nowline, 2, new QStandardItem(query.value(3).toString()));
            tb->setItem(nowline, 3, new QStandardItem(query.value(0).toString()));
            nowline++;
        }
    }
    tb->sort(3, Qt::DescendingOrder);
}

void insuranceall_delete(QStandardItemModel *&tb)
{
    if(tb!=NULL)
        delete tb;
}
